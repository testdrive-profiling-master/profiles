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
#ifndef __SYSTEM_DDK_H__
#define __SYSTEM_DDK_H__
#include "STDInterface.h"

#define DDK_API extern "C"

typedef void (*DDK_INTRRUPT_SERVICE)(void *pPrivate); // user interrupt service routine template

struct DDK {
	// Identify
	virtual const char *GetSystemDescription(void) = 0;

	// life cycle
	virtual void Release(void) = 0; // Release display object

	// memory
	virtual uint64_t GetMemoryBase(void)						  = 0;
	virtual uint64_t GetMemorySize(void)						  = 0;
	virtual bool	 MakeMemoryDump(const char *sFileName = NULL) = 0;
	virtual bool	 LoadMemoryDump(const char *sFileName = NULL) = 0;

	// register
	virtual uint32_t RegRead(uint64_t ulAddress)				   = 0;
	virtual void	 RegWrite(uint64_t ulAddress, uint32_t dwData) = 0;

	// system
	virtual void RegisterInterruptService(DDK_INTRRUPT_SERVICE routine, void *pPrivate = NULL) = 0; // register ISR(interrrupt service routine)
	virtual void EnableInterrupt(bool bEnable = true)										   = 0; // enable ISR
	virtual void ClearInterruptPending(void)												   = 0; // clear interrupt pending bit
	virtual uint32_t DriverCommand(void *pCommand)											   = 0; // post driver specific command
};

struct DDKMemory {
	virtual void	 AddRef(void)															   = 0; // add reference
	virtual void	 Release(void)															   = 0; // release memory object
	virtual void	*Virtual(void)															   = 0; // virtual memory pointer
	virtual uint64_t Physical(void)															   = 0; // physical memory address
	virtual uint64_t ByteSize(void)															   = 0; // byte allocation size
	virtual bool	 Flush(bool bWrite = true, uint64_t ulOffset = 0, uint64_t ulByteSize = 0) = 0; // flush memory
	virtual void	 PhysicalOverride(uint64_t ulPhysical = (uint64_t)-1)					   = 0;
};

// DDK
typedef void (*ENUMERATE_DDK_MEMORY_FUNCTION)(DDKMemory *pMemory, void *pPrivate);

DDK_API DDK		  *CreateDDK(void);
DDK_API DDKMemory *CreateDDKMemory(uint64_t ulByteSize, uint64_t ulByteAlignment);
DDK_API DDKMemory *CreateDDKMemoryEx(uint64_t ulByteSize, uint64_t ulByteAlignment, uint64_t ulPhyAddress);
DDK_API void	   EnumerateDDKMemory(ENUMERATE_DDK_MEMORY_FUNCTION func, void *pPrivate = NULL);
DDK_API void	   ReportDDKMemory(void);

#endif //__SYSTEM_DDK_H__
