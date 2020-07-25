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
// Title : Common profiles
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_DDK_H__
#define __SYSTEM_DDK_H__
#include "STDInterface.h"

#ifdef DDK_EXPORTS
	#define DDK_API __declspec(dllexport)
#else
	#define DDK_API __declspec(dllimport)		
	#pragma comment(lib, "SystemDDK.lib")

#endif

typedef enum{
	COLOR_FORMAT_R5G6B5,
	COLOR_FORMAT_A8B8G8R8,
}COLOR_FORMAT;

interface DDK;

typedef void (*DDK_INTRRUPT_SERVICE)(void* pPrivate);	// user interrupt service routine template

interface DDK{
	// Identify
	STDMETHOD_(const char*, GetSystemDescription)(void) PURE;

	// life cycle
	STDMETHOD_(void, Release)(void) PURE;		// Release display object

	// memory
	STDMETHOD_(DWORD, GetMemoryBase)(void) PURE;
	STDMETHOD_(void*, GetMemoryPointer)(DWORD dwPhyAddress, DWORD dwByteSize = 0) PURE;
	STDMETHOD_(BOOL, MakeMemoryDump)(const char* sFileName = NULL) PURE;
	STDMETHOD_(BOOL, LoadMemoryDump)(const char* sFileName = NULL) PURE;

	// register
	STDMETHOD_(DWORD, RegRead)(DWORD dwAddress) PURE;
	STDMETHOD_(void, RegWrite)(DWORD dwAddress, DWORD dwData) PURE;

	// system
	STDMETHOD_(void, RegisterInterruptService)(DDK_INTRRUPT_SERVICE routine, void* pPrivate = NULL) PURE;
	STDMETHOD_(void, EnableInterrupt)(BOOL bEnable = TRUE) PURE;
	STDMETHOD_(void, ClearInterruptPending)(BOOL bReleaseWait = FALSE) PURE;
	STDMETHOD_(void, WaitInterruptDone)(void) PURE;
};

interface DDKMemory{
	STDMETHOD_(void, AddRef)(void) PURE;		// add reference
	STDMETHOD_(void, Release)(void) PURE;		// release memory object
	STDMETHOD_(void*, Virtual)(void) PURE;		// virtual memory pointer
	STDMETHOD_(DWORD, Physical)(void) PURE;		// physical memory address
	STDMETHOD_(DWORD, ByteSize)(void) PURE;		// byte allocation size
	STDMETHOD_(BOOL, Flush)(BOOL bWrite = TRUE, DWORD dwOffset = 0, DWORD dwByteSize = 0) PURE;	// flush memory
};

// DDK
typedef void (*ENUMERATE_DDK_MEMORY_FUNCTION)(DDKMemory* pMemory, void* pPrivate);

DDK_API DDK* CreateDDK(void);
DDK_API DDKMemory* CreateDDKMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE);
DDK_API void EnumerateDDKMemory(ENUMERATE_DDK_MEMORY_FUNCTION func, void* pPrivate = NULL);

#endif//__SYSTEM_DDK_H__
