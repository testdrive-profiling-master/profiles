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
#include <assert.h>
#include <stdio.h>
#include "MemoryHeap.h"

static IMemoryManager*		g_pMemoryManager	= NULL;

void MemoryHeap::Initialize(BOOL bFree)
{
	m_dwRefCount		= 0;
	m_bFree				= bFree;
	m_pNative			= NULL;
	m_dwPhysical		= 0;
	m_dwByteSize		= 0;
	m_Link.SetObject(this);
	m_Free.SetObject(this);

	if(bFree) {
		m_Link.Link();
		m_Free.Link();
	}
}

void ShowByteSize(DWORD dwSize);
MemoryHeap::MemoryHeap(DWORD dwReqBytes, DWORD dwByteAlignment, BOOL bDMA)
{
	Initialize(FALSE);
	AddRef();
	assert(dwReqBytes != 0);
	assert(dwByteAlignment != 0);
	{
		// find empty heap area
		FreeLink*		pFree	= m_Free.Head();

		while(pFree) {
			if(pFree->Item()->Alloc(this, dwReqBytes, dwByteAlignment, bDMA)) return;

			pFree	= pFree->Next();
		}
	}
	printf("*E: Sorry! No more linear physical heap memory : ");
	ShowByteSize(dwReqBytes);
	printf("\n");
}

MemoryHeap::~MemoryHeap(void)
{
	SAFE_RELEASE(m_pNative);
	//assert(m_bFree==TRUE);
}

MemoryHeap::MemoryHeap(BYTE* pVirtual, DWORD dwPhysical, DWORD dwByteSize)
{
	Initialize(TRUE);
	AddRef();
	m_dwPhysical		= dwPhysical;
	m_dwByteSize		= dwByteSize;
}

void MemoryHeap::Free(void)
{
	assert(m_bFree == FALSE);

	if(m_bFree) return;

	m_bFree	= TRUE;
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

BOOL MemoryHeap::Alloc(MemoryHeap* pHeap, DWORD dwAllocByteSize, DWORD dwByteAlignment, BOOL bDMA)
{
	// adjust align size
	dwAllocByteSize			+= (m_dwPhysical + m_dwByteSize - dwAllocByteSize) & (dwByteAlignment - 1);

	if(!m_bFree || m_dwByteSize < dwAllocByteSize) return FALSE;

	// set link (this >> pHeap(alloc) >> m_Link.pNext)
	pHeap->m_Link.Link(&m_Link);
	// set size
	pHeap->m_dwByteSize		= dwAllocByteSize;
	m_dwByteSize			-= dwAllocByteSize;
	// set virtual
	pHeap->m_pNative		= g_pMemoryManager->CreateMemory(dwAllocByteSize, dwByteAlignment, bDMA);
	// set physical
	pHeap->m_dwPhysical		= m_dwPhysical + m_dwByteSize;
	// set not free heap to new heap
	pHeap->m_bFree			= FALSE;
	return pHeap->m_pNative != NULL;
}

void ShowByteSize(DWORD dwSize)
{
	BOOL bShow = FALSE;

	if(dwSize > 1024 * 1024 * 1024) {
		DWORD	b_size	= dwSize / (1024 * 1024 * 1024);
		dwSize	-= b_size * (1024 * 1024 * 1024);
		printf("%dGB", b_size);
		bShow	= TRUE;
	}

	if(dwSize > 1024 * 1024) {
		DWORD	b_size	= dwSize / (1024 * 1024);
		dwSize	-= b_size * (1024 * 1024);

		if(bShow) printf(" ");

		printf("%dMB", b_size);
		bShow	= TRUE;
	}

	if(dwSize > 1024) {
		DWORD	b_size	= dwSize / (1024);
		dwSize	-= b_size * (1024);

		if(bShow) printf(" ");

		printf("%dKB", b_size);
		bShow	= TRUE;
	}

	if(dwSize) {
		if(bShow) printf(" ");

		printf("%d Byte", dwSize);
	}
}

void MemoryHeap::AddRef(void)
{
	m_dwRefCount++;
}

void MemoryHeap::Release(void)
{
	m_dwRefCount--;

	if(!m_dwRefCount) Free();
}

void* MemoryHeap::Virtual(void)
{
	if(!m_pNative) return NULL;

	return m_pNative->Virtual();
}

DWORD MemoryHeap::Physical(void)
{
	return m_dwPhysical;
}

DWORD MemoryHeap::ByteSize(void)
{
	return m_dwByteSize;
}

BOOL MemoryHeap::Flush(BOOL bWrite, DWORD dwOffset, DWORD dwByteSize)
{
	assert((dwOffset + dwByteSize) <= m_dwByteSize);

	if(!dwByteSize) dwByteSize	= m_dwByteSize - dwOffset;

	if(m_pNative) return m_pNative->Flush(dwOffset, m_dwPhysical + dwOffset, dwByteSize, bWrite);

	return FALSE;
}

IMemory* CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA)
{
	return new MemoryHeap(dwByteSize, dwByteAlignment, bDMA);
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

//-----------------------------------------------------------
// Memory implementation
//-----------------------------------------------------------
MemoryImplementation	g_MemoryImplementation;
MemoryImplementation::MemoryImplementation(void) {}
MemoryImplementation::~MemoryImplementation(void) {}

BOOL MemoryImplementation::Initialize(BYTE* pVirtual, DWORD dwPhysical, DWORD dwByteSize, IMemoryManager* pMemoryManager)
{
	Release();
	g_pMemoryManager	= pMemoryManager;
	new MemoryHeap(pVirtual, dwPhysical, dwByteSize);
	return TRUE;
}

void MemoryImplementation::Release(void)
{
	Report();

	// Eliminate all heaps.
	while(HeapLink::Head()) delete HeapLink::Head()->Item();
}

void MemoryImplementation::Report(void)
{
	if(!HeapLink::Head()) return;

	if(HeapLink::Head()->Next()) {
		// 2개 이상의 heap 이 존재하면 해제되지 않은 메모리가 존재하는 것이므로 자세히 출력한다.
		int i_not_free_count	= 0;
		HeapLink*	pLink		= HeapLink::Head();
		printf("\n------------------------------------------------------------------\nSystem heap memory stack status\n"\
			   "    ID   IsFree    Address(Phy.)     ByteSize\n");

		for(int i = 0; pLink; i++) {
			MemoryHeap* pHeap = pLink->Item();
			printf(
				"    %-4d %c         0x%08X    %12u (", i, pHeap->IsFree() ? 'O' : 'X', pHeap->Physical(), pHeap->ByteSize());
			ShowByteSize(pHeap->ByteSize());
			printf(")\n");

			if(!pHeap->IsFree()) i_not_free_count++;

			pLink = pLink->Next();
		}

		if(i_not_free_count)
			printf("*E: Non-free heap count : %d\n", i_not_free_count);

		printf("------------------------------------------------------------------\n");
	}
}
