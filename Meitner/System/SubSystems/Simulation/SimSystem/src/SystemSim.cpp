//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Simulation sub-system
// Rev.  : 11/9/2022 Wed (clonextop@gmail.com)
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
#include <exception>	// for std::terminate

SystemSim::SystemSim(void)
{
	m_pSim				= NULL;
	m_pMemImp			= NULL;
}

SystemSim::~SystemSim(void)
{
}

const char* SystemSim::GetDescription(void)
{
	if(!m_pSim) return NULL;

	return m_pSim->GetSystemDescription();
}

BOOL SystemSim::Initialize(IMemoryImp* pMem)
{
	if(!g_SystemMemory.IsInitialized()) return FALSE;

	m_pSim	= new SimEngine();

	if(!m_pSim->Initialize()) return FALSE;

	// memory heap initialization
	m_pMemImp	= pMem;
	pMem->Initialize(g_SystemMemory.GetPointer(g_SystemMemory.BaseAddress()), g_SystemMemory.BaseAddress(), g_SystemMemory.ByteSize(), this);
	// run simulation thread
	BOOL bRet = m_pSim->Start();

	// wait until for start-up and release the reset.
	if(bRet) while(!SimulationTime() || SimClock::IsReset())Sleep(0);

	// flush initial start-up H/W log.
	fflush(stdout);
	return bRet;
}

void SystemSim::Release(void)
{
	// simulation end.
	if(m_pSim) {
		m_pSim->Terminate();
		SAFE_DELETE(m_pSim);
	}

	// release memory implementation
	if(m_pMemImp) {
		m_pMemImp->Release();
		m_pMemImp	= NULL;
	}

	delete this;
	// terminate all process
	std::set_terminate([]() {
		std::abort();
	});
	std::terminate();
}

DWORD SystemSim::RegRead(DWORD dwAddress)
{
	BusSlave* pSlave	= BusSlave::FindSlave(dwAddress);

	if(pSlave) {
		return pSlave->Read(dwAddress);
	} else {
		LOGE("Invalid 'RegRead' address : 0x%08X\n", dwAddress);
	}

	return 0xDEADC0DE;
}

void SystemSim::RegWrite(DWORD dwAddress, DWORD dwData)
{
	BusSlave* pSlave	= BusSlave::FindSlave(dwAddress);

	if(pSlave) {
		return pSlave->Write(dwAddress, dwData);
	} else {
		LOGE("Invalid 'RegWrite' address : 0x%08X (data : 0x%X)\n", dwAddress, dwData);
	}
}

void SystemSim::RegisterInterruptService(INTRRUPT_SERVICE routine)
{
	m_pSim->Interrupt().RegisterService(routine);
}

void SystemSim::EnableInterrupt(BOOL bEnable)
{
	m_pSim->Interrupt().Enable(bEnable);
}

void SystemSim::ClearInterruptPending(void)
{
	m_pSim->Interrupt().ClearPending();
}

// Memory interface
DWORD SystemSim::GetMemoryBase(void)
{
	return g_SystemMemory.BaseAddress();
}

void* SystemSim::GetMemoryPointer(DWORD dwPhyAddress, DWORD dwByteSize)
{
	return g_SystemMemory.GetPointer(dwPhyAddress, dwByteSize);
}

IMemoryNative* SystemSim::CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA)
{
	return new NativeSystemMemory(dwByteSize);
}
