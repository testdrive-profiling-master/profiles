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
#include "STDInterface.h"
#include "SystemSim.h"
#include "SimReset.h"
#include "SimBusy.h"
#include "SimClock.h"
#include "BusSlave.h"
#include "SystemMemory.h"
#include "NativeMemory.h"
#include <exception> // for std::terminate

SystemSim::SystemSim(void)
{
	m_pSim	  = NULL;
	m_pMemImp = NULL;
}

SystemSim::~SystemSim(void) {}

const char *SystemSim::GetDescription(void)
{
	if (!m_pSim)
		return NULL;

	return m_pSim->GetSystemDescription();
}

bool SystemSim::Initialize(IMemoryImp *pMem)
{
	if (!g_SystemMemory.IsInitialized())
		return false;

	m_pSim = new SimEngine();

	if (!m_pSim->Initialize())
		return false;

	// memory heap initialization
	m_pMemImp = pMem;
	pMem->Initialize(g_SystemMemory.BaseAddress(), g_SystemMemory.ByteSize(), this);
	// run simulation thread
	bool bRet = m_pSim->Start();

	// wait until for start-up and release the reset.
	if (bRet)
		while (!SimulationTime() || SimClock::IsReset()) Sleep(0);

	// flush initial start-up H/W log.
	fflush(stdout);
	return bRet;
}

void SystemSim::Release(void)
{
	// simulation end.
	if (m_pSim) {
		m_pSim->Terminate();
		SAFE_DELETE(m_pSim);
	}

	// release memory implementation
	if (m_pMemImp) {
		m_pMemImp->Release();
		m_pMemImp = NULL;
	}

	delete this;
}

uint32_t SystemSim::RegRead(uint64_t dwAddress)
{
	BusSlave *pSlave = BusSlave::FindSlave(dwAddress);

	if (pSlave) {
		return pSlave->Read(dwAddress);
	} else {
		LOGE("Invalid 'RegRead' address : 0x%08llX\n", dwAddress);
	}

	return 0xDEADC0DE;
}

void SystemSim::RegWrite(uint64_t dwAddress, uint32_t dwData)
{
	BusSlave *pSlave = BusSlave::FindSlave(dwAddress);

	if (pSlave) {
		pSlave->Write(dwAddress, dwData);
	} else {
		LOGE("Invalid 'RegWrite' address : 0x%08X (data : 0x%llX)\n", dwAddress, dwData);
	}
}

void SystemSim::RegisterInterruptService(INTRRUPT_SERVICE routine, void *pPrivate)
{
	m_pSim->Interrupt().RegisterService(routine, pPrivate);
}

void SystemSim::EnableInterrupt(bool bEnable)
{
	m_pSim->Interrupt().Enable(bEnable);
}

void SystemSim::ClearInterruptPending(void)
{
	m_pSim->Interrupt().ClearPending();
}

uint32_t SystemSim::DriverCommand(void *pCommand)
{
	return m_pSim->DriverCommand(pCommand);
}

// Memory interface
uint64_t SystemSim::GetMemoryBase(void)
{
	return g_SystemMemory.BaseAddress();
}

uint64_t SystemSim::GetMemorySize(void)
{
	return g_SystemMemory.ByteSize();
}

void *SystemSim::GetMemoryPointer(uint64_t lAddress, uint64_t dwByteSize)
{
	return g_SystemMemory.GetPointer(lAddress, dwByteSize);
}

IMemoryNative *SystemSim::CreateMemory(uint64_t dwByteSize, uint64_t dwByteAlignment)
{
	return new NativeSystemMemory(dwByteSize);
}
