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
	void SetBaseAddress(uint64_t ulAddress)
	{
		m_ulBaseAddress = ulAddress;
	}
	uint64_t ByteSize(void);
	uint8_t *GetPointer(uint64_t ulAddress, uint64_t ulSize = 0, bool bDisplay = false);
	void	 Flush(uint64_t ulAddress, uint64_t ulSize)
	{
		if (m_pSystemMemory)
			m_pSystemMemory->Flush(ulAddress, ulSize);
	}

	inline bool IsInitialized(void)
	{
		return (m_pSystemMemory != NULL) && (m_pDisplayMemory != NULL);
	}
	inline uint64_t BaseAddress(void)
	{
		return m_ulBaseAddress;
	}
	inline ITestDriverMemory *Handle(void)
	{
		return m_pSystemMemory;
	}
	inline SYSTEM_CONFIG *GetSystemConfig(void)
	{
		return m_pSystemConfig;
	}
	inline DisplayConfig *GetDisplayConfig(void)
	{
		return m_pDisplayConfig;
	}

private:
	ITestDriverMemory *m_pSystemMemory;
	ITestDriverMemory *m_pDisplayMemory;
	SYSTEM_CONFIG	  *m_pSystemConfig;
	DisplayConfig	  *m_pDisplayConfig;
	uint64_t		   m_ulBaseAddress;
};

extern SystemMemory g_SystemMemory;
#endif //__SYSTEM_MEMORY_H__
