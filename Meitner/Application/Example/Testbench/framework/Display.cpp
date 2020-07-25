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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "Display.h"

static const BYTE __DisplayFormatSize[] = {
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

Display::Display(void)
{
	m_pConfig		= NULL;
	m_pCmdMem		= NULL;
	m_pCmdBuffer	= NULL;
}

Display::~Display(void)
{
	Release();
}

BOOL Display::Initialize(DDK* pDDK)
{
	if(!m_TestDrive.Config.pSystem) return FALSE;

	m_pCmdMem		= CreateDDKMemory(4096, 4);
	m_dwBaseAddress	= pDDK->GetMemoryBase();
	m_dwCmdOffset	= m_pCmdMem->Physical() - m_dwBaseAddress;
	m_pCmdBuffer	= m_TestDrive.Memory.pSystem->GetPointer(m_dwCmdOffset);
	m_pConfig		= m_TestDrive.Config.pDisplay;
	return CheckDisplay();
}

void Display::Release(void)
{
	SAFE_RELEASE(m_pCmdMem);
}

BOOL Display::CheckDisplay(void)
{
	if(!m_pConfig->hWndHandle) {
		strcpy((char*)m_pCmdBuffer, "%TESTDRIVE_PROFILE%\\Common\\VirtualDisplay\\main.sp");
		SendMessage(m_TestDrive.Config.pSystem->hSystemManager, WM_USER, TESTDRIVE_CMD_RUN_PROFILE, m_dwCmdOffset);
	}

	return (m_pConfig->hWndHandle != NULL);
}

void Display::SaveToFile(const char* sFileName)
{
	if(!CheckDisplay()) return;

	strcpy((char*)m_pCmdBuffer, sFileName);
	SendMessage(m_pConfig->hWndHandle, WM_USER, DISPLAY_CMD_SAVE_TO_FILE, m_dwCmdOffset);
}

BOOL Display::LoadFromFile(const char* sFileName, DDKMemory* pMem, DWORD dwByteStride, DISPLAY_FORMAT format)
{
	if(!CheckDisplay() || !sFileName || !pMem) return FALSE;

	typedef struct {
		DISPLAY_FORMAT		format;
		DWORD				dwBaseAddress;
		DWORD				dwByteStride;
		char				sFileName[1024];
	} _load_cmd;
	_load_cmd*	pCmd		= (_load_cmd*)m_pCmdBuffer;
	pCmd->format			= format;
	pCmd->dwByteStride		= dwByteStride;
	pCmd->dwBaseAddress		= pMem->Physical() - m_dwBaseAddress;
	sprintf(pCmd->sFileName, "%%PROJECT%%Program\\%s", sFileName);
	SendMessage(m_pConfig->hWndHandle, WM_USER, DISPLAY_CMD_LOAD_FROM_FILE, m_dwCmdOffset);
	{
		if(!dwByteStride) {
			dwByteStride	= m_TestDrive.Config.pDisplay->iWidth * __DisplayFormatSize[m_TestDrive.Config.pDisplay->ColorFormat];
		}

		DWORD dwByteSize = m_TestDrive.Config.pDisplay->iHeight * dwByteStride;
		memcpy(pMem->Virtual(), m_TestDrive.Memory.pSystem->GetPointer(pCmd->dwBaseAddress), dwByteSize);
	}
	return (BOOL)pCmd->format;
}

void Display::Invalidate(BOOL bFront)
{
	if(bFront)
		m_pConfig->Front.bUpdate	= TRUE;
	else
		m_pConfig->Back.bUpdate		= TRUE;
}

void Display::SetBuffer(DDKMemory* pMem, BOOL bFront)
{
	if(!pMem) return;

	DWORD	dwAddr	= pMem->Physical() - m_dwBaseAddress;

	if(bFront)
		m_pConfig->Front.dwAddress	= dwAddr;
	else
		m_pConfig->Back.dwAddress	= dwAddr;
}

void Display::SetReverse(BOOL bReverse)
{
	m_pConfig->bReverse	= bReverse;
}

void Display::SetFormat(DISPLAY_FORMAT format)
{
	m_pConfig->ColorFormat	= format;
}

void Display::SetForeground(void)
{
	if(!CheckDisplay()) return;

	while(m_pConfig->bMovieSave && m_pConfig->Front.bUpdate) Sleep(10);

	SendMessage(m_pConfig->hWndHandle, WM_USER, DISPLAY_CMD_SET_FOREGROUND, 0);
}

void Display::SetResolution(int width, int height, DISPLAY_FORMAT format)
{
	m_pConfig->iWidth		= width;
	m_pConfig->iHeight		= height;
	m_pConfig->dwByteStride	= width * __DisplayFormatSize[(int)format];
	m_pConfig->ColorFormat	= format;
}
