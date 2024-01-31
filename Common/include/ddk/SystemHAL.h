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
// Rev.  : 1/31/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_HAL_H__
#define __SYSTEM_HAL_H__
#include "STDInterface.h"

//------------------------------------------------------------------------------------------------------------------
// system control interface
//------------------------------------------------------------------------------------------------------------------
typedef void (*INTRRUPT_SERVICE)(void* pPrivate);	// interrupt service routine

struct ISystem {
	// Identify
	virtual const char* GetDescription(void) = 0;							// get system description

	// Life cycle
	virtual void Release(void) = 0;											// system release

	// memory
	virtual UINT64 GetMemoryBase(void) = 0;									// get physical memory start address
	virtual UINT64 GetMemorySize(void) = 0;									// get physical memory byte size

	// register
	virtual DWORD RegRead(UINT64 dwAddress) = 0;							// read register
	virtual void RegWrite(UINT64 dwAddress, DWORD dwData) = 0;				// write register

	// System control
	virtual void RegisterInterruptService(INTRRUPT_SERVICE routine, void* pPrivate) = 0;	// register ISR(interrrupt service routine)
	virtual void EnableInterrupt(bool bEnable = true) = 0;					// enable ISR
	virtual void ClearInterruptPending(void) = 0;							// clear interrupt pending bit
	virtual DWORD DriverCommand(void* pCommand) = 0;						// post driver specific command
};

#if defined(SYSTEM_IMPLEMENTATION) || defined(SYSTEM_EXPORTS)
struct IMemoryNative {
	virtual void Release(void) = 0;
	virtual BYTE* Virtual(void) = 0;
	virtual bool Flush(UINT64 dwOffset, UINT64 dwPhyAddress, UINT64 dwByteSize, bool bWrite) = 0;
};

struct IMemoryManager {
	virtual IMemoryNative* CreateMemory(UINT64 dwByteSize, UINT64 dwByteAlignment) = 0;
};

struct IMemoryImp {
	virtual bool Initialize(UINT64 dwPhysical, UINT64 dwByteSize, IMemoryManager* pMemoryManager) = 0;
	virtual void Release(void) = 0;
	virtual bool SetInaccessible(UINT64 dwPhysical, UINT64 dwByteSize) = 0;
};

struct ISystemImp : public ISystem {
	virtual bool Initialize(IMemoryImp* pMem) = 0;					// create memory implementation
};
#endif

#ifndef SYSTEM_IMPLEMENTATION
#define SYSTEM_API		extern "C"
#ifdef SYSTEM_EXPORTS
typedef ISystemImp* (*CreateSystemImplementationFunction)(void);
#endif

SYSTEM_API ISystem* CreateSystem(void);		// System creation

struct IMemory {
	virtual void AddRef(void) = 0;			// add reference
	virtual void Release(void) = 0;			// release memory object
	virtual void* Virtual(void) = 0;		// virtual memory pointer
	virtual UINT64 Physical(void) = 0;		// physical memory address
	virtual UINT64 ByteSize(void) = 0;		// byte allocation size
	virtual bool Flush(bool bWrite = true, UINT64 dwOffset = 0, UINT64 dwByteSize = 0) = 0;	// flush memory
	virtual void PhysicalOverride(UINT64 dwPhysical = (UINT64) - 1) = 0;
};

typedef void (*ENUMERATE_MEMORY_FUNCTION)(IMemory* pMemory, void* pPrivate);

SYSTEM_API IMemory* CreateMemory(UINT64 dwByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress = (UINT64) - 1);
SYSTEM_API void EnumerateMemory(ENUMERATE_MEMORY_FUNCTION func, void* pPrivate = NULL);
SYSTEM_API void ReportMemory(void);

#endif

#endif//__SYSTEM_HAL_H__
