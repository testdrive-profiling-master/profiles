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
#ifndef __SYSTEM_DRIVER_INTERFACE_H__
#define __SYSTEM_DRIVER_INTERFACE_H__
#include "STDInterface.h"
#include "VirtualDisplayConfig.h"
#include "ddk/SystemHAL.h"
#include "NativeMemory.h"
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

void LOGI(const char *fmt, ...);
void LOGE(const char *fmt, ...);

//#define USE_TRACE_LOG
#ifdef USE_TRACE_LOG
#	define TRACE_LOG(s)                                                                                                                        \
		printf("\t* TRACE %s : %s - %s (%d)\n", s, __FILE__, __FUNCTION__, __LINE__);                                                           \
		fflush(stdout);
#else
#	define TRACE_LOG(s)
#endif

class NativeMemory;

typedef struct {
	uint64_t base_address;
	uint64_t byte_size;
} MEMORY_DESC;

class DriverCommon
{
public:
	void			   SetSystemDescription(const char *sDesc);
	inline const char *GetSystemDescription(void)
	{
		return m_sSystemDesc.c_str();
	}

protected:
	static MEMORY_DESC	m_TotalMemory;
	static MEMORY_DESC *m_pInaccessibleMemory;

private:
	static std::string m_sSystemDesc;
};

class SystemDriverInterface : public DriverCommon
{
public:
	SystemDriverInterface(void);
	virtual ~SystemDriverInterface(void);

	// life cycle
	virtual bool Initialize(const char *sDeviceName = NULL);
	virtual void Release(void);

	// register & memory interface
	virtual void	 SetCurrentCard(uint32_t dwIndex);
	virtual void	 RegWrite(uint64_t dwAddress, uint32_t dwData)													= 0;
	virtual uint32_t RegRead(uint64_t dwAddress)																	= 0;
	virtual void	 MemoryWrite(NativeMemory *pNative, uint64_t dwAddress, uint64_t dwOffset, uint32_t dwByteSize) = 0;
	virtual void	 MemoryRead(NativeMemory *pNative, uint64_t dwAddress, uint64_t dwOffset, uint32_t dwByteSize)	= 0;
	virtual void	 InterruptLock(void)																			= 0;
	virtual void	 InterruptFree(void)																			= 0;
	virtual uint32_t Command(void *pCommand);
	virtual void	 MemoryCreate(NativeMemory *pNative, uint64_t dwByteSize, uint64_t dwAlignment);
	virtual void	 MemoryFree(NativeMemory *pNative);

	// inlines
	inline uint32_t CardCount(void)
	{
		return m_dwCardCount;
	}

protected:
	HANDLE	 m_hDriver;
	uint32_t m_dwCardCount;
};

extern SystemDriverInterface *g_pDriver;

#define DECLARE_NATIVE_DRIVER(T)                                                                                                                \
	SystemDriverInterface *CreateNativeDriver(void)                                                                                             \
	{                                                                                                                                           \
		return new T;                                                                                                                           \
	}
SystemDriverInterface *CreateNativeDriver(void);

#endif //__SYSTEM_DRIVER_INTERFACE_H__
