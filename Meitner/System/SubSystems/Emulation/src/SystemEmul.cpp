//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : Emulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "SystemEmul.h"
#include "SystemMemory.h"
#include "NativeMemory.h"

SystemEmul::SystemEmul(void) : m_ProcessLock(0)
{
	m_pMemImp			= NULL;
	m_bMustExit			= FALSE;
	m_sSystemDesc		= "Emulation Simplified";
	m_sSystemDesc.insert(0, ",");
	m_sSystemDesc.insert(0, SystemName());
}

SystemEmul::~SystemEmul(void)
{
	KillThread();
	m_ISR.KillThread();
	// release memory implementation
	SAFE_RELEASE(m_pMemImp);
}

const char* SystemEmul::GetDescription(void)
{
	return m_sSystemDesc.c_str();
}

BOOL SystemEmul::Initialize(IMemoryImp* pMem)
{
	if(!g_SystemMemory.IsInitialized()) return FALSE;

	// memory heap initialization
	m_pMemImp	= pMem;
	pMem->Initialize(g_SystemMemory.GetPointer(g_SystemMemory.BaseAddress()), g_SystemMemory.BaseAddress(), g_SystemMemory.ByteSize(), this);
	// run simulation thread
	m_ISR.RunThread();
	EnableInterrupt(FALSE);
	RunThread();
	return TRUE;
}

void SystemEmul::Release(void)
{
	delete this;
}

DWORD SystemEmul::RegRead(DWORD dwAddress)
{
	DWORD dwData;
	SlaveRead(dwAddress, dwData);
	return dwData;
}

void SystemEmul::RegWrite(DWORD dwAddress, DWORD dwData)
{
	SlaveWrite(dwAddress, dwData);
}

void SystemEmul::RegisterInterruptService(INTRRUPT_SERVICE routine)
{
	m_ISR.RegisterService(routine);
}

void SystemEmul::EnableInterrupt(BOOL bEnable)
{
	m_ISR.Enable(bEnable);
}

void SystemEmul::ClearInterruptPending(void)
{
	m_ISR.ClearPending();
}

// Memory interface
DWORD SystemEmul::GetMemoryBase(void)
{
	return g_SystemMemory.BaseAddress();
}

void* SystemEmul::GetMemoryPointer(DWORD dwPhyAddress, DWORD dwByteSize)
{
	return g_SystemMemory.GetPointer(dwPhyAddress, dwByteSize);
}

void SystemEmul::MonitorThread(void)
{
	for(;;) {
		m_ProcessLock.Down();

		if(m_bMustExit || !MainLoop()) break;
	}
}

void SystemEmul::OnThreadKill(void)
{
	m_bMustExit	= TRUE;
	m_ProcessLock.Up();
}

void SystemEmul::InvokeMainLoop(void)
{
	m_ProcessLock.Up();
}

void SystemEmul::InvokeISR(void)
{
	m_ISR.Awake();
}

IMemoryNative* SystemEmul::CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA)
{
	return new NativeSystemMemory(dwByteSize);
}
