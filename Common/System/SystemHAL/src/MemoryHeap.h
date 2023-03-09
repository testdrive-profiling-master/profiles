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
// Title : Common profiles
// Rev.  : 3/9/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __MEMORY_HEAP_H__
#define __MEMORY_HEAP_H__
#include <list>
#include "ddk/SystemHAL.h"
#include "CircularLink.h"

class MemoryHeap;
class MemoryImplementation;

typedef	CircularLink<MemoryHeap*, 0>	HeapLink;
typedef	CircularLink<MemoryHeap*, 1>	FreeLink;

class MemoryHeap : public IMemory {
public:
	MemoryHeap(UINT64 dwByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress, bool bDMA);
	virtual void AddRef(void);
	virtual void Release(void);

	virtual void* Virtual(void);
	virtual UINT64 Physical(void);
	virtual UINT64 ByteSize(void);
	virtual bool Flush(bool bWrite = true, UINT64 dwOffset = 0, UINT64 dwByteSize = 0);
	virtual void PhysicalOverride(UINT64 dwPhysical = (UINT64) - 1);

	inline bool IsFree(void) {
		return m_bFree;
	}

	inline bool IsInaccessible(void) {
		return m_bInaccessible;
	}

	bool Alloc(MemoryHeap* pHeap, UINT64 dwAllocByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress, bool bDMA);

protected:
	MemoryHeap(MemoryHeap* pPrev = NULL);
	virtual ~MemoryHeap(void);
	MemoryHeap(UINT64 dwPhysical, UINT64 dwByteSize);

	MemoryHeap* Prev(void);
	MemoryHeap* Next(void);

	friend class MemoryImplementation;

private:
	// life-cycle management
	int							m_iRefCount;		// reference count

	// resource link management
	HeapLink					m_Link;				// all heap link
	FreeLink					m_Free;				// free heap link for free heap fast search

	// memory allocation information
	bool						m_bFree;				// is free heap?
	UINT64						m_dwPhysical;			// physical memory address
	UINT64						m_dwPhysicalOverride;	// physical memory address override
	UINT64						m_dwByteSize;			// heap size (byte, block aligned)
	IMemoryNative*				m_pNative;				// native system memory manager
	bool						m_bInaccessible;		// inaccessible memory
};

class MemoryImplementation : public IMemoryImp {
public:
	MemoryImplementation(void);
	virtual ~MemoryImplementation(void);

	virtual bool Initialize(UINT64 dwPhysical, UINT64 dwByteSize, IMemoryManager* pMemoryManager);
	virtual void Release(void);
	virtual bool SetInaccessible(UINT64 dwPhysical, UINT64 dwByteSize);

	int AllocatedMemoryCount(void);
	static void Report(void);

private:
	std::list<IMemory*>	m_InaccessibleList;
};

extern MemoryImplementation	g_MemoryImplementation;
#endif//__MEMORY_HEAP_H__
