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
#ifndef __SYSTEM_HAL_H__
#define __SYSTEM_HAL_H__
#include "STDInterface.h"

//------------------------------------------------------------------------------------------------------------------
// system control interface
//------------------------------------------------------------------------------------------------------------------
typedef void (*INTRRUPT_SERVICE)(void);	// interrupt service routine

interface ISystem{
	// Identify
	STDMETHOD_(const char*, GetDescription)(void) PURE;							// get system description

	// Life cycle
	STDMETHOD_(void, Release)(void) PURE;										// system release

	// memory
	STDMETHOD_(DWORD, GetMemoryBase)(void) PURE;								// get memory start address
	STDMETHOD_(void*, GetMemoryPointer)(DWORD dwPhyAddress, DWORD dwByteSize = 0) PURE;		// get virtual pointer from physical address

	// register
	STDMETHOD_(DWORD, RegRead)(DWORD dwAddress) PURE;							// read register
	STDMETHOD_(void, RegWrite)(DWORD dwAddress, DWORD dwData) PURE;				// write register

	// System control
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine) PURE;	// register ISR(interrrupt service routine)
	STDMETHOD_(void, EnableInterrupt)(BOOL bEnable = TRUE) PURE;				// enable ISR
	STDMETHOD_(void, ClearInterruptPending)(void) PURE;							// clear interrupt pending bit
};

#if defined(SYSTEM_IMPLEMENTATION) || defined(SYSTEM_EXPORTS)
	interface IMemoryNative{
		STDMETHOD_(void, Release)(void) PURE;
		STDMETHOD_(BYTE*, Virtual)(void) PURE;
		STDMETHOD_(BOOL, Flush)(DWORD dwOffset, DWORD dwPhyAddress, DWORD dwByteSize, BOOL bWrite) PURE;
	};

	interface IMemoryManager{
		STDMETHOD_(IMemoryNative*, CreateMemory)(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE) PURE;
	};

	interface IMemoryImp{
		STDMETHOD_(BOOL, Initialize)(BYTE* pVirtual, DWORD dwPhysical, DWORD dwByteSize, IMemoryManager* pMemoryManager) PURE;
		STDMETHOD_(void, Release)(void) PURE;
	};

	interface ISystemImp : public ISystem
	{
		STDMETHOD_(BOOL, Initialize)(IMemoryImp* pMem) PURE;					// create memory implementation
	};
#endif

#ifndef SYSTEM_IMPLEMENTATION
	#ifdef SYSTEM_EXPORTS
		#define SYSTEM_API __declspec(dllexport)
		typedef ISystemImp* (*CreateSystemImplementationFunction)(void);
	#else
		#define SYSTEM_API __declspec(dllimport)		
		#pragma comment(lib, "SystemHAL.lib")
	#endif

	SYSTEM_API ISystem* CreateSystem(void);		// 시스템 생성

	interface IMemory{
		STDMETHOD_(void, AddRef)(void) PURE;		// add reference
		STDMETHOD_(void, Release)(void) PURE;		// release memory object
		STDMETHOD_(void*, Virtual)(void) PURE;		// virtual memory pointer
		STDMETHOD_(DWORD, Physical)(void) PURE;		// physical memory address
		STDMETHOD_(DWORD, ByteSize)(void) PURE;		// byte allocation size
		STDMETHOD_(BOOL, Flush)(BOOL bWrite = TRUE, DWORD dwOffset = 0, DWORD dwByteSize = 0) PURE;	// flush memory
	};

	typedef void (*ENUMERATE_MEMORY_FUNCTION)(IMemory* pMemory, void* pPrivate);

	SYSTEM_API IMemory* CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE);
	SYSTEM_API void EnumerateMemory(ENUMERATE_MEMORY_FUNCTION func, void* pPrivate = NULL);

#endif

#endif//__SYSTEM_HAL_H__
