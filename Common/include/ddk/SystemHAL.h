//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 3/31/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_HAL_H__
#define __SYSTEM_HAL_H__
#include "STDInterface.h"

//------------------------------------------------------------------------------------------------------------------
// system control interface
//------------------------------------------------------------------------------------------------------------------
typedef void (*INTRRUPT_SERVICE)(void);	// interrupt service routine

struct ISystem {
	// Identify
	virtual const char* GetDescription(void) = 0;							// get system description

	// Life cycle
	virtual void Release(void) = 0;											// system release

	// memory
	virtual DWORD GetMemoryBase(void) = 0;									// get memory start address
	virtual void* GetMemoryPointer(DWORD dwPhyAddress, DWORD dwByteSize = 0) = 0;		// get virtual pointer from physical address

	// register
	virtual DWORD RegRead(DWORD dwAddress) = 0;								// read register
	virtual void RegWrite(DWORD dwAddress, DWORD dwData) = 0;				// write register

	// System control
	virtual void RegisterInterruptService(INTRRUPT_SERVICE routine) = 0;	// register ISR(interrrupt service routine)
	virtual void EnableInterrupt(BOOL bEnable = TRUE) = 0;					// enable ISR
	virtual void ClearInterruptPending(void) = 0;							// clear interrupt pending bit
};

#if defined(SYSTEM_IMPLEMENTATION) || defined(SYSTEM_EXPORTS)
	struct IMemoryNative {
		virtual void Release(void) = 0;
		virtual BYTE* Virtual(void) = 0;
		virtual BOOL Flush(DWORD dwOffset, DWORD dwPhyAddress, DWORD dwByteSize, BOOL bWrite) = 0;
	};

	struct IMemoryManager {
		virtual IMemoryNative* CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE) = 0;
	};

	struct IMemoryImp {
		virtual BOOL Initialize(BYTE* pVirtual, DWORD dwPhysical, DWORD dwByteSize, IMemoryManager* pMemoryManager) = 0;
		virtual void Release(void) = 0;
	};

	struct ISystemImp : public ISystem
	{
		virtual BOOL Initialize(IMemoryImp* pMem) = 0;					// create memory implementation
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

	struct IMemory {
		virtual void AddRef(void) = 0;			// add reference
		virtual void Release(void) = 0;			// release memory object
		virtual void* Virtual(void) = 0;		// virtual memory pointer
		virtual DWORD Physical(void) = 0;		// physical memory address
		virtual DWORD ByteSize(void) = 0;		// byte allocation size
		virtual BOOL Flush(BOOL bWrite = TRUE, DWORD dwOffset = 0, DWORD dwByteSize = 0) = 0;	// flush memory
	};

	typedef void (*ENUMERATE_MEMORY_FUNCTION)(IMemory* pMemory, void* pPrivate);

	SYSTEM_API IMemory* CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE);
	SYSTEM_API void EnumerateMemory(ENUMERATE_MEMORY_FUNCTION func, void* pPrivate = NULL);

#endif

#endif//__SYSTEM_HAL_H__
