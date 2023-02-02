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
// Rev.  : 2/2/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_DRIVER_INTERFACE_H__
#define __SYSTEM_DRIVER_INTERFACE_H__
#include "STDInterface.h"
#include "VirtualDisplayConfig.h"
#include "ddk/SystemHAL.h"
#include "driver_testdrive.h"
#include "NativeMemory.h"
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

void LOGI(char* fmt, ...);
void LOGE(char* fmt, ...);

//#define USE_TRACE_LOG
#ifdef USE_TRACE_LOG
#define	TRACE_LOG(s)	printf("\t* TRACE %s : %s - %s (%d)\n", s, __FILE__, __FUNCTION__, __LINE__);fflush(stdout);
#else
#define	TRACE_LOG(s)
#endif

class NativeMemory;

class DriverCommon {
public:
	void SetSystemDescription(const char* sDesc);
	inline const char* GetSystemDescription(void) {
		return m_sSystemDesc.c_str();
	}

private:
	static std::string		m_sSystemDesc;
};

class SystemDriverInterface : public DriverCommon {
public:
	SystemDriverInterface(void);
	virtual ~SystemDriverInterface(void);

	// life cycle
	virtual bool Initialize(const char* sDeviceName = NULL);
	virtual void Release(void);

	// register & memory interface
	virtual void SetCurrentCard(DWORD dwIndex);
	virtual void RegWrite(UINT64 dwAddress, DWORD dwData) = 0;
	virtual DWORD RegRead(UINT64 dwAddress) = 0;
	virtual void MemoryWrite(NativeMemory* pMem, UINT64 dwAddress, UINT64 dwOffset, DWORD dwByteSize) = 0;
	virtual void MemoryRead(NativeMemory* pMem, UINT64 dwAddress, UINT64 dwOffset, DWORD dwByteSize) = 0;
	virtual void InterruptLock(void) = 0;
	virtual void InterruptFree(void) = 0;
	virtual TD_DMA_MEMORY* DMAAlloc(UINT64 dwByteSize);
	virtual void DMAFree(TD_DMA_MEMORY* pMem);

	// inlines
	inline DWORD CardCount(void)	{
		return m_dwCardCount;
	}

	inline UINT64 MemBaseAddress(void) {
		return m_MemBaseAddress;
	}

	inline UINT64 MemByteSize(void) {
		return m_MemByteSize;
	}

protected:
	HANDLE				m_hDriver;
	DWORD				m_dwCardCount;
	UINT64				m_MemBaseAddress;
	UINT64				m_MemByteSize;
};

extern SystemDriverInterface*	g_pDriver;

#define DECLARE_NATIVE_DRIVER(T)	SystemDriverInterface* CreateNativeDriver(void) {return new T;}
SystemDriverInterface* CreateNativeDriver(void);

#endif//__SYSTEM_DRIVER_INTERFACE_H__
