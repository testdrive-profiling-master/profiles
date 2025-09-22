//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 9/22/2025 Mon (clonextop@gmail.com)
//================================================================================
#include "DDK_Context.h"

DDK_API DDK *CreateDDK(void)
{
	DDKContext *pContext = new DDKContext;
	setbuf(stdout, NULL); // eliminate delayed console output.

	if (!pContext->IsInitialized())
		SAFE_RELEASE(pContext);

	return pContext;
}

DDK_API DDKMemory *CreateDDKMemory(uint64_t ulByteSize, uint64_t ulByteAlignment)
{
	return (DDKMemory *)CreateMemory(ulByteSize, ulByteAlignment, (uint64_t)-1);
}

DDK_API DDKMemory *CreateDDKMemoryEx(uint64_t ulByteSize, uint64_t ulByteAlignment, uint64_t ulPhyAddress)
{
	return (DDKMemory *)CreateMemory(ulByteSize, ulByteAlignment, ulPhyAddress);
}

static void _ISR_(void *pPrivate)
{
	UNUSED(pPrivate);
	printf("*I: [DDK] Interrupt occurred.\n");
}

void EnumerateDDKMemory(ENUMERATE_DDK_MEMORY_FUNCTION func, void *pPrivate)
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
	m_pSystem = CreateSystem();

	if (m_pSystem)
		m_pSystem->RegisterInterruptService(_ISR_, this);
}

DDKContext::~DDKContext(void)
{
	if (m_pSystem)
		m_pSystem->RegisterInterruptService(NULL, NULL);

	SAFE_RELEASE(m_pSystem);
}

const char *DDKContext::GetSystemDescription(void)
{
	if (m_pSystem)
		return m_pSystem->GetDescription();

	return NULL;
}

void DDKContext::Release(void)
{
	delete this;
}

uint64_t DDKContext::GetMemoryBase(void)
{
	return m_pSystem->GetMemoryBase();
}

uint64_t DDKContext::GetMemorySize(void)
{
	return m_pSystem->GetMemorySize();
}

uint32_t DDKContext::RegRead(uint64_t ulAddress)
{
	return m_pSystem->RegRead(ulAddress);
}

void DDKContext::RegWrite(uint64_t ulAddress, uint32_t dwData)
{
	m_pSystem->RegWrite(ulAddress, dwData);
}

void DDKContext::RegisterInterruptService(DDK_INTRRUPT_SERVICE routine, void *pPrivate)
{
	if (m_pSystem)
		m_pSystem->RegisterInterruptService(routine ? routine : _ISR_, pPrivate ? pPrivate : this);
}

void DDKContext::EnableInterrupt(bool bEnable)
{
	m_pSystem->EnableInterrupt(bEnable);
}

void DDKContext::ClearInterruptPending(void)
{
	m_pSystem->ClearInterruptPending();
}

uint32_t DDKContext::DriverCommand(void *pCommand)
{
	return m_pSystem->DriverCommand(pCommand);
}
