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
#include <assert.h>
#include <stdio.h>
#include "MemoryHeap.h"
#include "TD_Semaphore.h"

using namespace std;

static Semaphore	   __SemaAlloc(1);
static IMemoryManager *__pMemoryManager		 = NULL;
static uint64_t		   __HEAP_PHY_BASE		 = 0;
static uint64_t		   __HEAP_BYTE_SIZE		 = 0;
static bool			   __bInaccessibleMemory = false;

static void			   __ShowByteSize(uint64_t ulSize)
{
	bool bShow = false;

	if (ulSize >= 1024 * 1024 * 1024) {
		uint64_t b_size = ulSize / (1024 * 1024 * 1024);
		ulSize -= b_size * (1024 * 1024 * 1024);
		printf("%lldGB", b_size);
		bShow = true;
	}

	if (ulSize >= 1024 * 1024) {
		uint64_t b_size = ulSize / (1024 * 1024);
		ulSize -= b_size * (1024 * 1024);

		if (bShow)
			printf(" ");

		printf("%lldMB", b_size);
		bShow = true;
	}

	if (ulSize >= 1024) {
		uint64_t b_size = ulSize / (1024);
		ulSize -= b_size * (1024);

		if (bShow)
			printf(" ");

		printf("%lldKB", b_size);
		bShow = true;
	}

	if (ulSize) {
		if (bShow)
			printf(" ");

		printf("%lld Byte", ulSize);
	}
}

MemoryHeap::MemoryHeap(uint64_t ulByteSize, uint64_t ulByteAlignment, uint64_t ulPhyAddress) : MemoryHeap()
{
	assert(ulByteSize != 0);
	assert(ulByteAlignment != 0);
	{
		// find empty heap area
		FreeLink *pFree = m_Free.Head();

		while (pFree) {
			if (pFree->Item()->Alloc(this, ulByteSize, ulByteAlignment, ulPhyAddress))
				return;

			pFree = pFree->Next();
		}
	}
	printf("*E: Sorry! No more linear physical heap memory : ");
	__ShowByteSize(ulByteSize);
	printf("\n");
}

MemoryHeap::MemoryHeap(MemoryHeap *pPrev) : m_Link(this), m_Free(this)
{
	m_iRefCount			 = 1; // referenced start
	m_ulPhysical		 = 0;
	m_ulPhysicalOverride = (uint64_t)-1;
	m_ulByteSize		 = 0;
	m_pNative			 = NULL;
	m_bFree				 = false;
	m_bInaccessible		 = false;

	if (pPrev)
		m_Link.Link(&pPrev->m_Link);
}

MemoryHeap::~MemoryHeap(void)
{
	SAFE_RELEASE(m_pNative);
}

MemoryHeap::MemoryHeap(uint64_t ulPhysical, uint64_t ulByteSize) : MemoryHeap()
{
	m_bFree = true;
	m_Link.Link();
	m_Free.Link();
	m_ulPhysical = ulPhysical;
	m_ulByteSize = ulByteSize;
}

bool MemoryHeap::Alloc(MemoryHeap *pHeap, uint64_t ulAllocByteSize, uint64_t ulByteAlignment, uint64_t ulPhyAddress)
{
	// adjust align size
	ulAllocByteSize += (m_ulPhysical + m_ulByteSize - ulAllocByteSize) & (ulByteAlignment - 1);

	// not freed memory or size is not fit to.
	if (!m_bFree || m_ulByteSize < ulAllocByteSize)
		return false;

	// physical address is not fit to.
	if ((ulPhyAddress != (uint64_t)-1) && (ulPhyAddress < m_ulPhysical || (ulPhyAddress + ulAllocByteSize) > (m_ulPhysical + m_ulByteSize)))
		return false;

	if (!__bInaccessibleMemory) {
		// create native first
		pHeap->m_pNative = __pMemoryManager->CreateMemory(ulAllocByteSize, ulByteAlignment);

		if (!pHeap->m_pNative)
			return false;
	} else {
		pHeap->m_bInaccessible = true;
	}

	// set not free heap to new heap
	pHeap->m_bFree = false;
	// set size
	pHeap->m_ulByteSize = ulAllocByteSize;
	m_ulByteSize -= ulAllocByteSize;
	// set link : this >> pHeap >> m_Link.pNext
	pHeap->m_Link.Link(&m_Link);

	// set physical
	if (ulPhyAddress != (uint64_t)-1) { // address specification allocation
		pHeap->m_ulPhysical = ulPhyAddress;
		// check tail freed size
		uint64_t ulExtraFreeSize = m_ulByteSize - (ulPhyAddress - m_ulPhysical);

		if (ulExtraFreeSize) {
			m_ulByteSize -= ulExtraFreeSize;
			auto pNext = pHeap->Next();

			if (pNext && pNext->IsFree()) {
				pNext->m_ulPhysical -= ulExtraFreeSize;
				pNext->m_ulByteSize += ulExtraFreeSize;
			} else {
				// add extra free memory block
				MemoryHeap *pExtraHeap	 = new MemoryHeap(pHeap);
				pExtraHeap->m_ulPhysical = pHeap->m_ulPhysical + pHeap->m_ulByteSize;
				pExtraHeap->m_ulByteSize = ulExtraFreeSize;
				pExtraHeap->m_bFree		 = true;
				pExtraHeap->m_Link.Link(&pHeap->m_Link);
				pExtraHeap->m_Free.Link();
			}

			if (!m_ulByteSize)
				ReleaseNoneThreadSafe();
		}
	} else { // managed allocation
		pHeap->m_ulPhysical = m_ulPhysical + m_ulByteSize;
	}

	return true;
}

MemoryHeap *MemoryHeap::Prev(void)
{
	auto pLink = m_Link.Prev();

	if (pLink)
		return pLink->Item();

	return NULL;
}

MemoryHeap *MemoryHeap::Next(void)
{
	auto pLink = m_Link.Next();

	if (pLink)
		return pLink->Item();

	return NULL;
}

void MemoryHeap::AddRef(void)
{
	m_iRefCount++;
}

void MemoryHeap::ReleaseNoneThreadSafe(void)
{
	m_iRefCount--;
	assert(m_iRefCount >= 0);

	if (!m_iRefCount) { // free this
		assert(m_bFree == false);
		m_bFree = true;
		{
			MemoryHeap *pCurHeap = this;
			MemoryHeap *pPrev	 = m_Link.Prev() ? m_Link.Prev()->Item() : NULL;
			MemoryHeap *pNext	 = m_Link.Next() ? m_Link.Next()->Item() : NULL;

			if (pPrev && pPrev->IsFree()) {
				pPrev->m_ulByteSize += m_ulByteSize;
				m_ulByteSize = 0;
				pCurHeap	 = pPrev;
			}

			if (pNext && pNext->IsFree()) {
				pCurHeap->m_ulByteSize += pNext->m_ulByteSize;
				delete pNext;
			}

			if (!m_ulByteSize)
				delete this;
			else
				m_Free.Link(); // set to free link stack
		}
	}
}

void MemoryHeap::Release(void)
{
	__SemaAlloc.Down();
	ReleaseNoneThreadSafe();
	__SemaAlloc.Up();
}

void *MemoryHeap::Virtual(void)
{
	if (!m_pNative)
		return NULL;

	return m_pNative->Virtual();
}

uint64_t MemoryHeap::Physical(void)
{
	return m_ulPhysical;
}

uint64_t MemoryHeap::ByteSize(void)
{
	return m_ulByteSize;
}

bool MemoryHeap::Flush(bool bWrite, uint64_t ulOffset, uint64_t ulByteSize)
{
	assert((ulOffset + ulByteSize) <= m_ulByteSize);

	if (!ulByteSize)
		ulByteSize = m_ulByteSize - ulOffset;

	uint64_t ulPhysical = (m_ulPhysicalOverride == (uint64_t)-1) ? m_ulPhysical : m_ulPhysicalOverride;

	if (m_pNative)
		return m_pNative->Flush(ulOffset, ulPhysical + ulOffset, ulByteSize, bWrite);

	return false;
}

void MemoryHeap::PhysicalOverride(uint64_t ulPhysical)
{
	m_ulPhysicalOverride = ulPhysical;
}

IMemory *CreateMemory(uint64_t ulByteSize, uint64_t ulByteAlignment, uint64_t ulPhyAddress)
{
	IMemory *pMem = NULL;
	__SemaAlloc.Down();
	{
		pMem = new MemoryHeap(ulByteSize, ulByteAlignment, ulPhyAddress);

		if (__bInaccessibleMemory) {
			__bInaccessibleMemory = false;
		} else {
			if (!pMem->Virtual()) {
				((MemoryHeap *)pMem)->ReleaseNoneThreadSafe();
				pMem = NULL; // cppcheck-suppress memleak
			}
		}
	}
	__SemaAlloc.Up();

	if (pMem && (pMem->ByteSize() != ulByteSize)) {
		printf(
			"*W: Memory byte size is not matched : requested(%lld bytes) != allocated(%lld bytes), alignment(%lld)\n", ulByteSize,
			pMem->ByteSize(), ulByteAlignment);
	}

	return pMem;
}

void EnumerateMemory(ENUMERATE_MEMORY_FUNCTION func, void *pPrivate)
{
	if (HeapLink::Head()) {
		HeapLink *pLink = HeapLink::Head();

		while (pLink) {
			MemoryHeap *pHeap = pLink->Item();

			if (!pHeap->IsFree())
				func(pHeap, pPrivate);

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
MemoryImplementation g_MemoryImplementation;
MemoryImplementation::MemoryImplementation(void) {}
MemoryImplementation::~MemoryImplementation(void) {}

bool MemoryImplementation::Initialize(uint64_t ulPhysical, uint64_t ulByteSize, IMemoryManager *pMemoryManager)
{
	Release();
	__pMemoryManager = pMemoryManager;
	__HEAP_PHY_BASE	 = ulPhysical;
	__HEAP_BYTE_SIZE = ulByteSize;
	new MemoryHeap(ulPhysical, ulByteSize);
	return true;
}

void MemoryImplementation::Release(void)
{
	if (AllocatedMemoryCount()) {
		Report();
		printf("*W: Unreleased memory is existed.\n");
	}

	// release all inaccessible memories
	if (m_InaccessibleList.size()) {
		for (auto &i : m_InaccessibleList) i->Release();

		m_InaccessibleList.clear();
	}

	// Eliminate all heaps.
	while (HeapLink::Head()) delete HeapLink::Head()->Item();
}

bool MemoryImplementation::SetInaccessible(uint64_t ulPhysical, uint64_t ulByteSize)
{
	__bInaccessibleMemory = true;
	IMemory *pMem		  = CreateMemory(ulByteSize, 1, ulPhysical);

	if (pMem) {
		m_InaccessibleList.push_back(pMem);
		return true;
	} else {
		printf("*E: Can't define inaccessible memory area : physical_address(0x%llX), byte_size(0x%llX)\n", ulPhysical, ulByteSize);
	}

	return false;
}

int MemoryImplementation::AllocatedMemoryCount(void)
{
	int		  iCount = 0;
	HeapLink *pLink	 = HeapLink::Head();

	while (pLink) {
		MemoryHeap *pHeap = pLink->Item();

		if (!pHeap->IsInaccessible() && !pHeap->IsFree())
			iCount++;

		pLink = pLink->Next();
	}

	return iCount;
}

void MemoryImplementation::Report(void)
{
	if (HeapLink::Head()) {
		// Report detail un-freed heap memories
		int		  i_none_free_count = 0;
		HeapLink *pLink				= HeapLink::Head();
		printf("\n------------------------------------------------------------------\nSystem heap memory stack status\n"
			   "      ID   Free   Address (Physical)         ByteSize\n");

		for (int i = 0; pLink; i++) {
			MemoryHeap *pHeap = pLink->Item();

			if (!pHeap->IsInaccessible()) {
				printf(
					"    %4d    %c     0x%08X_%08X    %12llu (", i, pHeap->IsFree() ? 'O' : 'X', (UINT32)(pHeap->Physical() >> 32),
					(UINT32)(pHeap->Physical() & 0xFFFFFFFF), pHeap->ByteSize());
				__ShowByteSize(pHeap->ByteSize());
				printf(")\n");

				if (!pHeap->IsFree())
					i_none_free_count++;
			}

			pLink = pLink->Next();
		}

		if (i_none_free_count)
			printf("*I: Non-free heap count : %d\n", i_none_free_count);

		printf("------------------------------------------------------------------\n");
	}
}
