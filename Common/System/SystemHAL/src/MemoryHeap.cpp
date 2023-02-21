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
// Rev.  : 2/21/2023 Tue (clonextop@gmail.com)
//================================================================================
#include <assert.h>
#include <stdio.h>
#include "MemoryHeap.h"

using namespace std;

static IMemoryManager*		__pMemoryManager		= NULL;
static UINT64				__HEAP_PHY_BASE			= 0;
static UINT64				__HEAP_BYTE_SIZE		= 0;
static bool					__bInaccessibleMemory	= false;

static void __ShowByteSize(UINT64 dwSize)
{
	bool bShow = false;

	if(dwSize > 1024 * 1024 * 1024) {
		UINT64	b_size	= dwSize / (1024 * 1024 * 1024);
		dwSize	-= b_size * (1024 * 1024 * 1024);
		printf("%lldGB", b_size);
		bShow	= true;
	}

	if(dwSize > 1024 * 1024) {
		UINT64	b_size	= dwSize / (1024 * 1024);
		dwSize	-= b_size * (1024 * 1024);

		if(bShow) printf(" ");

		printf("%lldMB", b_size);
		bShow	= true;
	}

	if(dwSize > 1024) {
		UINT64	b_size	= dwSize / (1024);
		dwSize	-= b_size * (1024);

		if(bShow) printf(" ");

		printf("%lldKB", b_size);
		bShow	= true;
	}

	if(dwSize) {
		if(bShow) printf(" ");

		printf("%lld Byte", dwSize);
	}
}

MemoryHeap::MemoryHeap(UINT64 dwByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress, bool bDMA) : MemoryHeap()
{
	assert(dwByteSize != 0);
	assert(dwByteAlignment != 0);
	{
		// find empty heap area
		FreeLink*		pFree	= m_Free.Head();

		while(pFree) {
			if(pFree->Item()->Alloc(this, dwByteSize, dwByteAlignment, dwPhyAddress, bDMA)) return;

			pFree	= pFree->Next();
		}
	}
	printf("*E: Sorry! No more linear physical heap memory : ");
	__ShowByteSize(dwByteSize);
	printf("\n");
}

MemoryHeap::MemoryHeap(MemoryHeap* pPrev) : m_Link(this), m_Free(this)
{
	m_iRefCount			= 1;	// referenced start
	m_dwPhysical		= 0;
	m_dwByteSize		= 0;
	m_pNative			= NULL;
	m_bFree				= false;
	m_bInaccessible		= false;

	if(pPrev) m_Link.Link(&pPrev->m_Link);
}

MemoryHeap::~MemoryHeap(void)
{
	SAFE_RELEASE(m_pNative);
}

MemoryHeap::MemoryHeap(UINT64 dwPhysical, UINT64 dwByteSize) : MemoryHeap()
{
	m_bFree				= true;
	m_Link.Link();
	m_Free.Link();
	m_dwPhysical		= dwPhysical;
	m_dwByteSize		= dwByteSize;
}

bool MemoryHeap::Alloc(MemoryHeap* pHeap, UINT64 dwAllocByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress, bool bDMA)
{
	// adjust align size
	dwAllocByteSize			+= (m_dwPhysical + m_dwByteSize - dwAllocByteSize) & (dwByteAlignment - 1);

	// not freed memory or size is not fit to.
	if(!m_bFree || m_dwByteSize < dwAllocByteSize) return false;

	// physical address is not fit to.
	if((dwPhyAddress != (UINT64) - 1) && (dwPhyAddress < m_dwPhysical || (dwPhyAddress + dwAllocByteSize) > (m_dwPhysical + m_dwByteSize))) return false;

	if(!__bInaccessibleMemory) {
		// create native first
		pHeap->m_pNative		= __pMemoryManager->CreateMemory(dwAllocByteSize, dwByteAlignment, bDMA);

		if(!pHeap->m_pNative) return false;
	} else {
		pHeap->m_bInaccessible	= true;
	}

	// set not free heap to new heap
	pHeap->m_bFree			= false;
	// set size
	pHeap->m_dwByteSize		= dwAllocByteSize;
	m_dwByteSize			-= dwAllocByteSize;
	// set link : this >> pHeap >> m_Link.pNext
	pHeap->m_Link.Link(&m_Link);

	// set physical
	if(dwPhyAddress != (UINT64) - 1) {	// address specification allocation
		pHeap->m_dwPhysical		= dwPhyAddress;
		// check tail freed size
		UINT64	dwExtraFreeSize	= m_dwByteSize - (dwPhyAddress - m_dwPhysical);

		if(dwExtraFreeSize) {
			m_dwByteSize	-= dwExtraFreeSize;
			auto pNext		= pHeap->Next();

			if(pNext && pNext->IsFree()) {
				pNext->m_dwPhysical		-= dwExtraFreeSize;
				pNext->m_dwByteSize		+= dwExtraFreeSize;
			} else {
				// add extra free memory block
				MemoryHeap*	pExtraHeap		= new MemoryHeap(pHeap);
				pExtraHeap->m_dwPhysical	= pHeap->m_dwPhysical + pHeap->m_dwByteSize;
				pExtraHeap->m_dwByteSize	= dwExtraFreeSize;
				pExtraHeap->m_bFree			= true;
				pExtraHeap->m_Link.Link(&pHeap->m_Link);
				pExtraHeap->m_Free.Link();
			}

			if(!m_dwByteSize) Release();
		}
	} else {	// managed allocation
		pHeap->m_dwPhysical		= m_dwPhysical + m_dwByteSize;
	}

	return true;
}

MemoryHeap* MemoryHeap::Prev(void)
{
	auto pLink	= m_Link.Prev();

	if(pLink) return pLink->Item();

	return NULL;
}

MemoryHeap* MemoryHeap::Next(void)
{
	auto pLink	= m_Link.Next();

	if(pLink) return pLink->Item();

	return NULL;
}

void MemoryHeap::AddRef(void)
{
	m_iRefCount++;
}

void MemoryHeap::Release(void)
{
	m_iRefCount--;
	assert(m_iRefCount >= 0);

	if(!m_iRefCount) {	// free this
		assert(m_bFree == false);
		m_bFree	= true;
		{
			MemoryHeap* pCurHeap	= this;
			MemoryHeap*	pPrev		= m_Link.Prev() ? m_Link.Prev()->Item() : NULL;
			MemoryHeap*	pNext		= m_Link.Next() ? m_Link.Next()->Item() : NULL;

			if(pPrev && pPrev->IsFree()) {
				pPrev->m_dwByteSize	+= m_dwByteSize;
				m_dwByteSize		= 0;
				pCurHeap			= pPrev;
			}

			if(pNext && pNext->IsFree()) {
				pCurHeap->m_dwByteSize	+= pNext->m_dwByteSize;
				delete pNext;
			}

			if(!m_dwByteSize) delete this;
			else m_Free.Link();	// set to free link stack
		}
	}
}

void* MemoryHeap::Virtual(void)
{
	if(!m_pNative) return NULL;

	return m_pNative->Virtual();
}

UINT64 MemoryHeap::Physical(void)
{
	return m_dwPhysical;
}

UINT64 MemoryHeap::ByteSize(void)
{
	return m_dwByteSize;
}

bool MemoryHeap::Flush(bool bWrite, UINT64 dwOffset, UINT64 dwByteSize)
{
	assert((dwOffset + dwByteSize) <= m_dwByteSize);

	if(!dwByteSize) dwByteSize	= m_dwByteSize - dwOffset;

	if(m_pNative) return m_pNative->Flush(dwOffset, m_dwPhysical + dwOffset, dwByteSize, bWrite);

	return false;
}

IMemory* CreateMemory(UINT64 dwByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress, bool bDMA)
{
	IMemory* pMem = new MemoryHeap(dwByteSize, dwByteAlignment, dwPhyAddress, bDMA);

	if(__bInaccessibleMemory) {
		__bInaccessibleMemory	= false;
	} else {
		if(!pMem->Virtual()) {
			pMem->Release();
			pMem	= NULL;	// cppcheck-suppress memleak
		}
	}

	return pMem;
}

void EnumerateMemory(ENUMERATE_MEMORY_FUNCTION func, void* pPrivate)
{
	if(HeapLink::Head()) {
		HeapLink*	pLink		= HeapLink::Head();

		while(pLink) {
			MemoryHeap* pHeap	= pLink->Item();

			if(!pHeap->IsFree()) func(pHeap, pPrivate);

			pLink = pLink->Next();
		}
	}
}

void ReportMemory(void)
{
	MemoryImplementation::Report();
}

//-----------------------------------------------------------
// Memory implementation
//-----------------------------------------------------------
MemoryImplementation	g_MemoryImplementation;
MemoryImplementation::MemoryImplementation(void) {}
MemoryImplementation::~MemoryImplementation(void) {}

bool MemoryImplementation::Initialize(UINT64 dwPhysical, UINT64 dwByteSize, IMemoryManager* pMemoryManager)
{
	Release();
	__pMemoryManager	= pMemoryManager;
	__HEAP_PHY_BASE		= dwPhysical;
	__HEAP_BYTE_SIZE	= dwByteSize;
	new MemoryHeap(dwPhysical, dwByteSize);
	return true;
}

void MemoryImplementation::Release(void)
{
	if(AllocatedMemoryCount()) Report();

	// release all inaccessible memories
	if(m_InaccessibleList.size()) {
		for(auto& i : m_InaccessibleList) i->Release();

		m_InaccessibleList.clear();
	}

	// Eliminate all heaps.
	while(HeapLink::Head()) delete HeapLink::Head()->Item();
}

bool MemoryImplementation::SetInaccessible(UINT64 dwPhysical, UINT64 dwByteSize)
{
	__bInaccessibleMemory	= true;
	IMemory*	pMem		= CreateMemory(dwByteSize, 1, dwPhysical);

	if(pMem) {
		m_InaccessibleList.push_back(pMem);
		return true;
	} else {
		printf("*E: Can't define inaccessible memory area : physical_address(0x%llX), byte_size(0x%llX)\n", dwPhysical, dwByteSize);
	}

	return false;
}

int MemoryImplementation::AllocatedMemoryCount(void)
{
	int			iCount	= 0;
	HeapLink*	pLink	= HeapLink::Head();

	while(pLink) {
		MemoryHeap* pHeap = pLink->Item();

		if(!pHeap->IsInaccessible() && !pHeap->IsFree()) iCount++;

		pLink	= pLink->Next();
	}

	return iCount;
}

void MemoryImplementation::Report(void)
{
	if(HeapLink::Head()) {
		// Report detail un-freed heap memories
		int i_none_free_count	= 0;
		HeapLink*	pLink		= HeapLink::Head();
		printf("\n------------------------------------------------------------------\nSystem heap memory stack status\n"\
			   "      ID   Free   Address (Physical)         ByteSize\n");

		for(int i = 0; pLink; i++) {
			MemoryHeap* pHeap = pLink->Item();

			if(!pHeap->IsInaccessible()) {
				printf(
					"    %4d    %c     0x%08X_%08X    %12llu (", i, pHeap->IsFree() ? 'O' : 'X', (UINT32)(pHeap->Physical() >> 32), (UINT32)(pHeap->Physical() & 0xFFFFFFFF), pHeap->ByteSize());
				__ShowByteSize(pHeap->ByteSize());
				printf(")\n");

				if(!pHeap->IsFree()) i_none_free_count++;
			}

			pLink = pLink->Next();
		}

		if(i_none_free_count)
			printf("*I: Non-free heap count : %d\n", i_none_free_count);

		printf("------------------------------------------------------------------\n");
	}
}
