//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
//
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// Title : Common profiles
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "SimEngine.h"
#include "SystemMemory.h"
#include "SimClock.h"
#include "SimBusy.h"
#include <stdio.h>
#include <assert.h>

//-----------------------------------------------------------------------
// SimResource
//-----------------------------------------------------------------------
SimEngine *SimResource::m_pSim = NULL;

//-----------------------------------------------------------------------
// SimInstance
//-----------------------------------------------------------------------
SimInstance::SimInstance(void)
{
	m_pNext				= m_pSim->m_pInstance;
	m_pSim->m_pInstance = this;
}

SimInstance::~SimInstance(void)
{
	if (m_pSim->m_pInstance == this)
		m_pSim->m_pInstance = m_pNext;
	else {
		SimInstance *pNode = m_pSim->m_pInstance;

		while (pNode) {
			if (pNode->m_pNext == this) {
				pNode->m_pNext = m_pNext;
				break;
			}

			pNode = pNode->m_pNext;
		}
	}
}

//-----------------------------------------------------------------------
// SimEngine
//-----------------------------------------------------------------------
SimEngine::SimEngine(void) : m_bErrorOccured(false)
{
	m_bUpdate		= true;
	m_pInstance		= NULL;
	m_pSim			= this;
	m_pSimHDL		= NULL;
	m_bForceToExit	= false;
	m_DriverCommand = NULL;
	SetSystemDescription("Simulation Simplified");
}

SimEngine::~SimEngine(void)
{
	m_Interrupt.Enable(false);
	m_Interrupt.KillThread();
	KillThread();
	{
		// delete all instances
		SimInstance *pInstance = m_pInstance;

		while (pInstance) {
			SimInstance *pNext = pInstance->m_pNext;
			delete pInstance;
			pInstance = pNext;
		}
	}
	SimClock::ReleaseAll();
	SAFE_RELEASE(m_pSimHDL);
	m_pSim = NULL;
}

bool SimEngine::Initialize(void)
{
	if (!m_pSimHDL) {
		m_pSimHDL = CreateSimHDL(this);

		if (!m_pSimHDL->Initialize()) {
			m_pSimHDL->Release();
			m_pSimHDL = NULL;
		}
	}

	return (m_pSimHDL != NULL);
}

bool SimEngine::Clocking(void)
{
	bool bBusy	  = false;
	bool bSuccess = false;
	// clock tik
	SimClock::Tik();

	// evaluation
	if (m_bForceToExit || !m_pSimHDL->Eval()) {
		// finish operation
		LOGI("Simulation will be forced to shutdown.\n");
		return false;
	}

	// clock tok :calculate & get next clocking
	AdvenceSimulationTime(SimClock::Tok());
	// call instance list
	{
		SimInstance *pInstance = m_pInstance;

		while (pInstance) {
			SimInstance *pNext = pInstance->m_pNext;
			bBusy |= pInstance->OnRun();
			pInstance = pNext;
		}
	}
	// delayed lock implementation
	bBusy |= m_Lock.Check();
	return bBusy;
}

bool SimEngine::AwakeInterrupt(void)
{
	bool bAwake = m_Interrupt.Awake();

	if (bAwake)
		m_Lock.SetDelay(64);

	return bAwake;
}

void SimEngine::MonitorThread(void)
{
	TRACE_LOCK
	m_Lock.Lock(16); // start lock
	LOGI("Simulation H/W daemon is up.\n");

	while (Clocking()); // Execute engine

	m_Interrupt.Enable(false);
	LOGI("Simulation H/W daemon is down.\n");
	TRACE_LOG("End thread")
	VerilatorFlush();

	if (IsGotError()) {
		SAFE_RELEASE(m_pSimHDL);
		exit(1);
	}
}

void SimEngine::OnThreadKill(bool bForced)
{
	TRACE_LOG(bForced ? "Force to exit" : "Try to exit")

	if (bForced)
		m_bForceToExit = true;

	m_Lock.Rlease();
	m_Interrupt.KillThread();
}

bool SimEngine::Start(void)
{
	if (!m_Interrupt.RunThread())
		return false;

	return RunThread();
}

void SimEngine::Terminate(void)
{
	KillThread();
	m_Interrupt.KillThread();
}

void SimEngine::SetMemoryBaseAddress(uint64_t ulAddress)
{
	g_SystemMemory.SetBaseAddress(ulAddress);
}

void SimEngine::MemoryRead32(int iID, uint64_t ulAddress, uint32_t &dwData)
{
	uint32_t *pData = (uint32_t *)g_SystemMemory.GetPointer(ulAddress, sizeof(uint32_t), iID);

	if (pData) {
		dwData = *pData;
	} else {
		assert("Memory read assert!" == 0);
	}
}

void SimEngine::MemoryWrite32(int iID, uint64_t ulAddress, uint32_t dwData)
{
	uint32_t *pData = (uint32_t *)g_SystemMemory.GetPointer(ulAddress, sizeof(uint32_t), iID);

	if (pData) {
		*pData = dwData;
	} else {
		assert("Memory write assert!" == 0);
	}
}

void SimEngine::MemoryRead16(int iID, uint64_t ulAddress, uint16_t &dwData)
{
	uint16_t *pData = (uint16_t *)g_SystemMemory.GetPointer(ulAddress, sizeof(uint16_t), iID);

	if (pData) {
		dwData = *pData;
	} else {
		assert("Memory read assert!" == 0);
	}
}

void SimEngine::MemoryWrite16(int iID, uint64_t ulAddress, uint16_t dwData)
{
	uint16_t *pData = (uint16_t *)g_SystemMemory.GetPointer(ulAddress, sizeof(uint16_t), iID);

	if (pData) {
		*pData = dwData;
	} else {
		assert("Memory write assert!" == 0);
	}
}

void SimEngine::MemoryRead8(int iID, uint64_t ulAddress, uint8_t &dwData)
{
	uint8_t *pData = (uint8_t *)g_SystemMemory.GetPointer(ulAddress, sizeof(uint8_t), iID);

	if (pData) {
		dwData = *pData;
	} else {
		assert("Memory read assert!" == 0);
	}
}

void SimEngine::MemoryWrite8(int iID, uint64_t ulAddress, uint8_t dwData)
{
	uint8_t *pData = (uint8_t *)g_SystemMemory.GetPointer(ulAddress, sizeof(uint8_t), iID);

	if (pData) {
		*pData = dwData;
	} else {
		assert("Memory write assert!" == 0);
	}
}

SYSTEM_CONFIG *SimEngine::GetSystemConfig(void)
{
	return g_SystemMemory.GetSystemConfig();
}

DisplayConfig *SimEngine::GetDisplayConfig(void)
{
	return g_SystemMemory.GetDisplayConfig();
}

uint8_t *SimEngine::GetMemoryPointer(uint64_t ulAddress, uint64_t ulSize, bool bDisplay)
{
	return g_SystemMemory.GetPointer(ulAddress, ulSize, bDisplay);
}

uint64_t SimEngine::GetMemoryBaseAddress(void)
{
	return g_SystemMemory.BaseAddress();
}

bool SimEngine::GetMemory(const char *sName, void *&pConfig, void *&pMemory)
{
	ITestDriverMemory *pMem = TestDriver_GetMemory(sName);

	if (!pMem) {
		pConfig = NULL;
		pMemory = NULL;
		return false;
	}

	pConfig = (void *)pMem->GetConfig();
	pMemory = (void *)pMem->GetPointer();
	return true;
}

void SimEngine::SimulationLock(int iDelayTicks)
{
	TRACE_LOCK
	m_Lock.Lock(iDelayTicks);
}

void SimEngine::SimulationUnLock(void)
{
	TRACE_UNLOCK
	m_Lock.UnLock();
}

void SimEngine::SimulationAddBusy(uint8_t *pBusy)
{
	new SimBusy(pBusy);
}

void SimEngine::SimulationDebugMode(bool bDebug)
{
	SetThreadBreakable(!bDebug);

	if (bDebug)
		SimulationFlush();
}

void SimEngine::SetSystemDescription(const char *sDesc)
{
	if (sDesc) {
		if (m_sSystemDesc.size()) {
			if (strstr(m_sSystemDesc.c_str(), sDesc))
				return;

			m_sSystemDesc.insert(0, ",");
		}

		m_sSystemDesc.insert(0, sDesc);
	}
}

void SimEngine::SetError(bool bError)
{
	m_bErrorOccured = bError;
}

void SimEngine::RegisterDriverCommand(DRIVER_COMMAND func)
{
	m_DriverCommand = func;
}

uint32_t SimEngine::DriverCommand(void *pCommand)
{
	if (m_DriverCommand)
		return m_DriverCommand(pCommand);

	return (uint32_t)-1;
}

const char *SimEngine::GetSystemDescription(void)
{
	return m_sSystemDesc.c_str();
}
