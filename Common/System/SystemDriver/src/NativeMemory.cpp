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
// Title : TestDrive System Driver wrapper
// Rev.  : 2/14/2023 Tue (clonextop@gmail.com)
//================================================================================
#include "STDInterface.h"
#include "NativeMemory.h"

bool	NativeMemory::bAlwaysUseDMA	= false;

NativeMemory::NativeMemory(UINT64 dwByteSize, UINT64 dwAlignment, bool bDMA)
{
	if(bDMA || bAlwaysUseDMA) {
		m_pDMA	= g_pDriver->DMAAlloc(dwByteSize);
		m_pMem	= m_pDMA ? (BYTE*)m_pDMA->vir_addr.pointer : NULL;
	} else {
		m_pDMA	= NULL;
		m_pMem	= new BYTE[dwByteSize];
	}
}
NativeMemory::~NativeMemory(void)
{
	if(m_pDMA) {
		g_pDriver->DMAFree(m_pDMA);
		m_pDMA	= NULL;
		m_pMem	= NULL;
	} else {
		SAFE_DELETE_ARRAY(m_pMem);
	}
}

void NativeMemory::Release(void)
{
	delete this;
}

BYTE* NativeMemory::Virtual(void)
{
	return m_pMem;
}

bool NativeMemory::Flush(UINT64 dwOffset, UINT64 dwPhyAddress, UINT64 dwByteSize, bool bWrite)
{
	if(m_pMem) {
		if(bWrite)
			g_pDriver->MemoryWrite(this, dwPhyAddress, dwOffset, dwByteSize);
		else
			g_pDriver->MemoryRead(this, dwPhyAddress, dwOffset, dwByteSize);
	}

	return true;
}
