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
// Title : Common DPI
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "VirtualDisplay.h"
#include "SelfDestory.h"

static VirtualDisplay*	__VirtualDisplay	= NULL;

VirtualDisplay::VirtualDisplay(void)
{
	memset(&m_MemoryRange, 0, sizeof(m_MemoryRange));
	m_pConfig			= NULL;
	__VirtualDisplay	= this;
}
VirtualDisplay::~VirtualDisplay(void)
{
	m_pConfig			= NULL;
	__VirtualDisplay	= NULL;
}

void VirtualDisplay::Initialize(bool bReverse)
{
	m_pConfig			= GetDisplayConfig();
	m_pConfig->bReverse	= bReverse;
}
void VirtualDisplay::SetBaseAddress(DWORD dwBaseAddress, bool bFront)
{
	if(!m_pConfig) return;

	DisplayBuffer* pBuffer	= bFront ? &m_pConfig->Front : &m_pConfig->Back;
	pBuffer->dwAddress	= dwBaseAddress - GetMemoryBaseAddress();
	pBuffer->bUpdate	= true;
	UpdateMemoryRange();
}
void VirtualDisplay::SetFormat(int iWidth, int iHeight, DWORD dwByteStride, DISPLAY_FORMAT Format)
{
	if(!m_pConfig) return;

	if(!dwByteStride) {
		static const BYTE __FormatSize[] = {
			1,		// A8
			1,		// L8
			2,		// L8A8
			2,		// R5G6B5
			2,		// R4G4B4A4
			2,		// R5G5B5A1
			3,		// R8G8B8
			4,		// A8B8G8R8
			4,		// R8G8B8A8
			4,		// A8R8G8B8
		};
		dwByteStride			= iWidth * __FormatSize[m_pConfig->ColorFormat];
	}

	m_pConfig->iWidth			= iWidth;
	m_pConfig->iHeight			= iHeight;
	m_pConfig->ColorFormat		= Format;
	m_pConfig->Front.bUpdate	= true;
	m_pConfig->Back.bUpdate		= true;
	m_pConfig->dwByteStride		= dwByteStride;
	UpdateMemoryRange();
}

void VirtualDisplay::Update(bool bFront)
{
	if(!m_pConfig) return;

	DisplayBuffer* pBuffer	= bFront ? &m_pConfig->Front : &m_pConfig->Back;
	pBuffer->bUpdate		= true;
}

void VirtualDisplay::OnDoFilter(DWORD dwAddress, DWORD dwByteSize)
{
	if(dwAddress < m_MemoryRange.dwBase  || dwAddress >= m_MemoryRange.dwEnd) return;

	if(!m_pConfig) return;

	m_pConfig->Back.bUpdate		= true;
}


void VirtualDisplay::UpdateMemoryRange(void)
{
	if(!m_pConfig) return;

	m_MemoryRange.dwBase	= m_pConfig->Back.dwAddress + GetMemoryBaseAddress();
	m_MemoryRange.dwEnd		= m_MemoryRange.dwBase + (m_pConfig->dwByteStride * m_pConfig->iHeight);
}

DPI_FUNCTION void VirtualDisplayInitialize(int bReverse)
{
	if(!__VirtualDisplay) __VirtualDisplay	= new VirtualDisplay;

	__VirtualDisplay->Initialize(bReverse);
}

DPI_FUNCTION void VirtualDisplayBaseAddress(unsigned int dwBaseAddress, int bFront)
{
	if(!__VirtualDisplay) return;

	__VirtualDisplay->SetBaseAddress(dwBaseAddress, bFront);
}

DPI_FUNCTION void VirtualDisplayFormat(int iWidth, int iHeight, unsigned int dwByteStride, unsigned int Format)
{
	if(!__VirtualDisplay) return;

	if(iWidth < 0 || iWidth > 4096) iWidth	= 100;

	if(iHeight < 0 || iHeight > 4096) iHeight	= 100;

	__VirtualDisplay->SetFormat(iWidth, iHeight, dwByteStride, (DISPLAY_FORMAT)Format);
}

DPI_FUNCTION void VirtualDisplayUpdate(int bFront)
{
	if(!__VirtualDisplay) return;

	__VirtualDisplay->Update(bFront);
}
