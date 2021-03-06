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
// Title : Simulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_MEMORY_H__
#define __SYSTEM_MEMORY_H__
#include "TestDriver.h"
#include "SystemConfig.h"
#include "VirtualDisplayConfig.h"
#include "dpi_interfaces.h"

class SystemMemory
{
public:
	SystemMemory(void);
	virtual ~SystemMemory(void);
	void SetBaseAddress(DWORD dwAddress)				{m_dwBaseAddress	= dwAddress;}
	DWORD ByteSize(void);
	BYTE* GetPointer(DWORD dwAddress, DWORD dwSize = 0, BOOL bDisplay = FALSE);
	void Flush(DWORD dwAddress, DWORD dwSize)			{if(m_pSystemMemory) m_pSystemMemory->Flush(dwAddress, dwSize);}

	inline BOOL IsInitialized(void)					{return (m_pSystemMemory!=NULL) && (m_pDisplayMemory!=NULL);}
	inline DWORD BaseAddress(void)					{return m_dwBaseAddress;}
	inline ITestDriverMemory* Handle(void)			{return m_pSystemMemory;}
	inline SYSTEM_CONFIG* GetSystemConfig(void)		{return m_pSystemConfig;}
	inline DisplayConfig* GetDisplayConfig(void)		{return m_pDisplayConfig;}

private:
	ITestDriverMemory*		m_pSystemMemory;
	ITestDriverMemory*		m_pDisplayMemory;
	SYSTEM_CONFIG*			m_pSystemConfig;
	DisplayConfig*			m_pDisplayConfig;
	DWORD					m_dwBaseAddress;
};

extern SystemMemory			g_SystemMemory;
#endif//__SYSTEM_MEMORY_H__
