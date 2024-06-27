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
// Title : TestDrive System Driver wrapper
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "SystemDriver.h"
#include "NativeMemory.h"

SystemDriver::SystemDriver(void)
{
	m_pNativeDriver = NULL;
	m_pMemImp		= NULL;
	m_bMustExit		= false;
	SetSystemDescription("System driver");
}

SystemDriver::~SystemDriver(void)
{
	m_ISR.KillThread();
	// release memory implementation
	SAFE_RELEASE(m_pMemImp);
	SAFE_DELETE(m_pNativeDriver);
}

const char *SystemDriver::GetDescription(void)
{
	return GetSystemDescription();
}

bool SystemDriver::Initialize(IMemoryImp *pMem)
{
	SAFE_DELETE(m_pNativeDriver);
	m_pNativeDriver = CreateNativeDriver();

	if (!m_pNativeDriver->Initialize())
		return false;

	// memory heap initialization
	m_pMemImp = pMem;
	pMem->Initialize(m_TotalMemory.base_address, m_TotalMemory.byte_size, this);

	// apply inaccessible memory list
	if (m_pInaccessibleMemory) {
		MEMORY_DESC *pDesc = m_pInaccessibleMemory;

		while (pDesc->byte_size) {
			pMem->SetInaccessible(pDesc->base_address, pDesc->byte_size);
			pDesc++;
		}
	}

	// run simulation thread
	m_ISR.RunThread();
	EnableInterrupt(false);
	return true;
}

void SystemDriver::Release(void)
{
	delete this;
}

uint32_t SystemDriver::RegRead(uint64_t ulAddress)
{
	uint32_t dwData = m_pNativeDriver->RegRead(ulAddress);
	return dwData;
}

void SystemDriver::RegWrite(uint64_t ulAddress, uint32_t dwData)
{
	m_pNativeDriver->RegWrite(ulAddress, dwData);
}

void SystemDriver::RegisterInterruptService(INTRRUPT_SERVICE routine, void *pPrivate)
{
	m_ISR.RegisterService(routine, pPrivate);
}

void SystemDriver::EnableInterrupt(bool bEnable)
{
	m_ISR.Enable(bEnable);
}

void SystemDriver::ClearInterruptPending(void)
{
	m_ISR.ClearPending();
}

uint32_t SystemDriver::DriverCommand(void *pCommand)
{
	return m_pNativeDriver->Command(pCommand);
}

// Memory interface
uint64_t SystemDriver::GetMemoryBase(void)
{
	return m_TotalMemory.base_address;
}

uint64_t SystemDriver::GetMemorySize(void)
{
	return m_TotalMemory.byte_size;
}

void SystemDriver::InvokeISR(void)
{
	m_ISR.Awake();
}

IMemoryNative *SystemDriver::CreateMemory(uint64_t ulByteSize, uint64_t ulByteAlignment)
{
	return new NativeMemory(ulByteSize, ulByteAlignment);
}
