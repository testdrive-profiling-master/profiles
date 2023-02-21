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
#include "DDK_Context.h"

DDK_API DDK* CreateDDK(void)
{
	DDKContext* pContext	= new DDKContext;
	setbuf(stdout, NULL);	// eliminate delayed console output.

	if(!pContext->IsInitialized())
		SAFE_RELEASE(pContext);

	return pContext;
}

DDK_API DDKMemory* CreateDDKMemory(UINT64 dwByteSize, UINT64 dwByteAlignment, bool bDMA)
{
	return (DDKMemory*)CreateMemory(dwByteSize, dwByteAlignment, (UINT64) - 1, bDMA);
}

DDK_API DDKMemory* CreateDDKMemoryEx(UINT64 dwByteSize, UINT64 dwByteAlignment, UINT64 dwPhyAddress, bool bDMA)
{
	return (DDKMemory*)CreateMemory(dwByteSize, dwByteAlignment, dwPhyAddress, bDMA);
}

static DDK_INTRRUPT_SERVICE		__ISR_routine	= NULL;	// ISR routine
static void*					__ISR_pPrivate	= NULL;	// ISR private data
static void _ISR_(void)
{
	if(__ISR_routine) {
		__ISR_routine(__ISR_pPrivate);
	}
}

void EnumerateDDKMemory(ENUMERATE_DDK_MEMORY_FUNCTION func, void* pPrivate)
{
	EnumerateMemory((ENUMERATE_MEMORY_FUNCTION)func, pPrivate);
}

void ReportDDKMemory(void)
{
	ReportMemory();
}

//-----------------------------------------------------------------

DDKContext::DDKContext(void)
{
	m_pSystem		= CreateSystem();
	__ISR_pPrivate	= this;

	if(m_pSystem) m_pSystem->RegisterInterruptService(_ISR_);
}

DDKContext::~DDKContext(void)
{
	if(m_pSystem) m_pSystem->RegisterInterruptService(NULL);

	__ISR_pPrivate	= NULL;
	SAFE_RELEASE(m_pSystem);
}

const char* DDKContext::GetSystemDescription(void)
{
	if(m_pSystem) return m_pSystem->GetDescription();

	return NULL;
}

void DDKContext::Release(void)
{
	delete this;
}

UINT64 DDKContext::GetMemoryBase(void)
{
	return m_pSystem->GetMemoryBase();
}

UINT64 DDKContext::GetMemorySize(void)
{
	return m_pSystem->GetMemorySize();
}

DWORD DDKContext::RegRead(UINT64 dwAddress)
{
	return m_pSystem->RegRead(dwAddress);
}

void DDKContext::RegWrite(UINT64 dwAddress, DWORD dwData)
{
	m_pSystem->RegWrite(dwAddress, dwData);
}

void DDKContext::RegisterInterruptService(DDK_INTRRUPT_SERVICE routine, void* pPrivate)
{
	__ISR_routine	= NULL;		// detach from ISR
	__ISR_pPrivate	= pPrivate ? pPrivate : (DDK*)this;
	__ISR_routine	= routine;	// attach to ISR
}

void DDKContext::EnableInterrupt(bool bEnable)
{
	m_pSystem->EnableInterrupt(bEnable);
}

void DDKContext::ClearInterruptPending(void)
{
	m_pSystem->ClearInterruptPending();
}

DWORD DDKContext::DriverCommand(void* pCommand)
{
	return m_pSystem->DriverCommand(pCommand);
}
