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
// Rev.  : 2/20/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "SystemDriverInterface.h"

SystemDriverInterface*	g_pDriver	= NULL;

void LOGI(char* fmt, ...)
{
	printf("*I: [SystemDriver] ");
	{
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
	printf("\n");
	fflush(stdout);
}

void LOGE(char* fmt, ...)
{
	printf("*E: [SystemDriver] ");
	{
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
	printf("\n");
	fflush(stdout);
}

std::string		DriverCommon::m_sSystemDesc;
MEMORY_DESC		DriverCommon::m_TotalMemory				= {0};		// total memory block
MEMORY_DESC*	DriverCommon::m_pInaccessibleMemory		= NULL;		// unused memorys

void DriverCommon::SetSystemDescription(const char* sDesc)
{
	if(sDesc) {
		if(m_sSystemDesc.size()) {
			if(strstr(m_sSystemDesc.c_str(), sDesc))
				return;

			m_sSystemDesc.insert(0, ",");
		}

		m_sSystemDesc.insert(0, sDesc);
	}
}

SystemDriverInterface::SystemDriverInterface(void)
{
	m_hDriver			= NULL;
	m_dwCardCount		= 0;
	g_pDriver			= this;
}

SystemDriverInterface::~SystemDriverInterface(void)
{
	SystemDriverInterface::Release();
	g_pDriver			= NULL;
}

bool SystemDriverInterface::Initialize(const char* sDeviceName)
{
	Release();

	if(sDeviceName) {
		if((m_hDriver = CreateFile(sDeviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL)) != INVALID_HANDLE_VALUE) {
			return true;
		}

		// failed to create device driver
		m_hDriver	= NULL;
	}

	return false;
}

void SystemDriverInterface::Release(void)
{
	if(m_hDriver) {
		CloseHandle(m_hDriver);
		m_hDriver	= NULL;
	}
}

void SystemDriverInterface::SetCurrentCard(DWORD dwIndex)
{
	// None implementation
}

DWORD SystemDriverInterface::Command(void* pCommand)
{
	// None implementation
	return (DWORD) -1;
}

void SystemDriverInterface::MemoryCreate(NativeMemory* pNative, UINT64 dwByteSize, UINT64 dwAlignment, bool bDMA)
{
	// default implementation
	pNative->pMem	= new BYTE[dwByteSize];
}

void SystemDriverInterface::MemoryFree(NativeMemory* pNative)
{
	// default implementation
	SAFE_DELETE_ARRAY(pNative->pMem);
}
