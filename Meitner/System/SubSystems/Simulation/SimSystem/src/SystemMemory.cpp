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
// Title : Simulation sub-system
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "STDInterface.h"
#include "SystemMemory.h"

SystemMemory			g_SystemMemory;

bool GetConfiguration(LPCTSTR sKeyName, LPTSTR sValue)
{
	if(!GetEnvironmentVariable(sKeyName, sValue, MAX_PATH)) {
		char	sConfigFIlePath[4096];
		GetFullPathName(".TestDrive", 4096, sConfigFIlePath, NULL);
		GetPrivateProfileString("Configuration", sKeyName, "", sValue, MAX_PATH, sConfigFIlePath);
	}

	return (*sValue) != NULL;
}

SystemMemory::SystemMemory(void)
{
	char	sMemoryNameSystem[MAX_PATH];
	char	sMemoryNameDisplay[MAX_PATH];
	GetConfiguration("SYSTEM_MEMORY_NAME", sMemoryNameSystem);
	GetConfiguration("DISPLAY_MEMORY_NAME", sMemoryNameDisplay);
	m_pSystemMemory		= TestDriver_GetMemory(sMemoryNameSystem);
	m_pSystemConfig		= m_pSystemMemory ? (SYSTEM_CONFIG*)m_pSystemMemory->GetConfig() : NULL;
	m_pDisplayMemory	= TestDriver_GetMemory(sMemoryNameDisplay);
	m_pDisplayConfig	= m_pDisplayMemory ? (DisplayConfig*)m_pDisplayMemory->GetConfig() : NULL;
	m_dwBaseAddress		= 0x80000000;	// default system memory's base address
}

SystemMemory::~SystemMemory(void)
{
	TestDriver_Cleanup();
}

DWORD SystemMemory::ByteSize(void)
{
	return m_pSystemMemory->GetSize();
}

BYTE* SystemMemory::GetPointer(DWORD dwAddress, DWORD dwSize, bool bDisplay)
{
	if(bDisplay) return m_pDisplayMemory->GetPointer(dwAddress, dwSize);

	return m_pSystemMemory->GetPointer(dwAddress - m_dwBaseAddress, dwSize);
}
