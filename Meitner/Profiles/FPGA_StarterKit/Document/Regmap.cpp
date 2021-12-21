//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Starter Kit document
// Rev.  : 12/21/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "Regmap.h"

Regmap*				Regmap::m_pHead		= NULL;
STARTERKIT_REGMAP*	Regmap::m_pReg		= NULL;

Regmap::Regmap(LPCTSTR sName)
{
	m_pNext			= m_pHead;
	m_pHead			= this;
	m_sName			= sName;
}

Regmap::~Regmap(void)
{
	if(m_pNext) {
		delete m_pNext;
		m_pNext	= NULL;
	}
}

void Regmap::ReleaseAll(void)
{
	if(m_pHead) {
		delete m_pHead;
		m_pHead	= NULL;
	}
}

BOOL Regmap::Update(void)
{
	BOOL	bUpdate	= FALSE;
	Regmap* pRegmap	= m_pHead;

	if(m_pReg->magic_code == SYSTEM_MAGIC_CODE)	// check correct register map.
		while(pRegmap) {
			if(pRegmap->OnUpdate()) bUpdate = TRUE;

			pRegmap	= pRegmap->m_pNext;
		}

	return bUpdate;
}

void Regmap::PostUpdate(void)
{
	PostMessage(g_pDoc->GetWindowHandle(), WM_TIMER, 10, 0);
}

void Regmap::Command(LPCTSTR lpszURL)
{
	Regmap* pRegmap	= m_pHead;

	while(pRegmap) {
		if(pRegmap->Name() && (_tcsstr(lpszURL, pRegmap->Name()) == lpszURL)) {
			pRegmap->OnCommand(lpszURL + _tcslen(pRegmap->Name()) + 1);
			break;
		}

		pRegmap	= pRegmap->m_pNext;
	}
}

void Regmap::Initialize(void)
{
	Broadcast(NULL);
}

void Regmap::Broadcast(LPVOID pData)
{
	Regmap* pRegmap	= m_pHead;

	while(pRegmap) {
		pRegmap->OnBroadcast(pData);
		pRegmap	= pRegmap->m_pNext;
	}
}

void Regmap::OnBroadcast(LPVOID pData)
{
}

BOOL Regmap::OnCommand(LPCTSTR lpszURL)
{
	return FALSE;
}

void Regmap::SetText(LPCTSTR lpszTarget, LPCTSTR lpszFormat, ...)
{
	CString cmd;
	{
		CString str;
		va_list args;
		va_start(args, lpszFormat);
		str.FormatV(lpszFormat, args);
		va_end(args);
		cmd.Format(_T("SetBody(\"%s\", \"%s\");"), lpszTarget, str);
	}
	g_pHtml->CallJScript(cmd);
}
