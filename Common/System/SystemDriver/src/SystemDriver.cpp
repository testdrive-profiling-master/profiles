//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TestDrive System Driver wrapper
// Rev.  : 2/1/2023 Wed (clonextop@gmail.com)
//================================================================================
#include "SystemDriver.h"
#include "NativeMemory.h"

SystemDriver::SystemDriver(void)
{
	m_pNativeDriver		= NULL;
	m_pMemImp			= NULL;
	m_bMustExit			= false;
	SetSystemDescription("System driver");
}

SystemDriver::~SystemDriver(void)
{
	m_ISR.KillThread();
	// release memory implementation
	SAFE_RELEASE(m_pMemImp);
	SAFE_DELETE(m_pNativeDriver);
}

const char* SystemDriver::GetDescription(void)
{
	return GetSystemDescription();
}

bool SystemDriver::Initialize(IMemoryImp* pMem)
{
	SAFE_DELETE(m_pNativeDriver);
	m_pNativeDriver = CreateNativeDriver();

	if(!m_pNativeDriver->Initialize()) return false;

	// memory heap initialization
	m_pMemImp	= pMem;
	pMem->Initialize(m_pNativeDriver->MemBaseAddress(), m_pNativeDriver->MemByteSize(), this);
	// run simulation thread
	m_ISR.RunThread();
	EnableInterrupt(false);
	return true;
}

void SystemDriver::Release(void)
{
	delete this;
}

DWORD SystemDriver::RegRead(UINT64 dwAddress)
{
	return m_pNativeDriver->RegRead(dwAddress);;
}

void SystemDriver::RegWrite(UINT64 dwAddress, DWORD dwData)
{
	m_pNativeDriver->RegWrite(dwAddress, dwData);
}

void SystemDriver::RegisterInterruptService(INTRRUPT_SERVICE routine)
{
	m_ISR.RegisterService(routine);
}

void SystemDriver::EnableInterrupt(bool bEnable)
{
	m_ISR.Enable(bEnable);
}

void SystemDriver::ClearInterruptPending(void)
{
	m_ISR.ClearPending();
}

// Memory interface
UINT64 SystemDriver::GetMemoryBase(void)
{
	return m_pNativeDriver->MemBaseAddress();
}

UINT64 SystemDriver::GetMemorySize(void)
{
	return m_pNativeDriver->MemByteSize();
}

void SystemDriver::InvokeISR(void)
{
	m_ISR.Awake();
}

IMemoryNative* SystemDriver::CreateMemory(UINT64 dwByteSize, UINT64 dwByteAlignment, bool bDMA)
{
	return new NativeMemory(dwByteSize, dwByteAlignment, bDMA);
}
