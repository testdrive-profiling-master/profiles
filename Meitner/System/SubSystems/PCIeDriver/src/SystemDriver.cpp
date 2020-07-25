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
// Title : Driver(PCIe) sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "SystemDriver.h"
#include "SystemMemory.h"
#include "NativeMemory.h"

const char* __SYSTEM_DESCRIPTION	= "DUT";

SystemDriver::SystemDriver(void)
{
	m_pMemImp			= NULL;
	m_bMustExit			= FALSE;
	SetSystemDescription("System driver");
	SetSystemDescription(__SYSTEM_DESCRIPTION);
}

SystemDriver::~SystemDriver(void)
{
	m_ISR.KillThread();
	m_Driver.Release();
	// release memory implementation
	SAFE_RELEASE(m_pMemImp);
}

const char* SystemDriver::GetDescription(void)
{
	return GetSystemDescription();
}

BOOL SystemDriver::Initialize(IMemoryImp* pMem)
{
	if(!g_SystemMemory.IsInitialized()) return FALSE;

	if(!m_Driver.Initialize()) return FALSE;

	// memory heap initialization
	m_pMemImp	= pMem;
	pMem->Initialize(g_SystemMemory.GetPointer(SYSTEM_MEMORY_BASE), SYSTEM_MEMORY_BASE, g_SystemMemory.ByteSize(), this);
	// run simulation thread
	m_ISR.RunThread();
	EnableInterrupt(FALSE);
	return TRUE;
}

void SystemDriver::Release(void)
{
	delete this;
}

DWORD SystemDriver::RegRead(DWORD dwAddress)
{
	return m_Driver.RegRead(dwAddress);;
}

void SystemDriver::RegWrite(DWORD dwAddress, DWORD dwData)
{
	m_Driver.RegWrite(dwAddress, dwData);
}

void SystemDriver::RegisterInterruptService(INTRRUPT_SERVICE routine)
{
	m_ISR.RegisterService(routine);
}

void SystemDriver::EnableInterrupt(BOOL bEnable)
{
	m_ISR.Enable(bEnable);
}

void SystemDriver::ClearInterruptPending(void)
{
	m_ISR.ClearPending();
}

// Memory interface
DWORD SystemDriver::GetMemoryBase(void)
{
	return SYSTEM_MEMORY_BASE;
}

void* SystemDriver::GetMemoryPointer(DWORD dwPhyAddress, DWORD dwByteSize)
{
	return g_SystemMemory.GetPointer(dwPhyAddress, dwByteSize);
}

void SystemDriver::InvokeISR(void)
{
	m_ISR.Awake();
}

IMemoryNative* SystemDriver::CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA)
{
	return new NativeSystemMemory(dwByteSize);
}
