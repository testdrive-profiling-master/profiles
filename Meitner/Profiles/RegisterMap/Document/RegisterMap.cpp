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
// Title : Meitner processor register map document
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "testdrive_document.inl"
#include "RegisterMap.h"
#include "RegmapCommand.h"
#include "RegmapSystem.h"
#include "RegmapScratchCounter.h"
#include "RegmapGPRs.h"
#include "RegmapMB.h"
#include "RegmapTrace.h"

ITDDocument*	g_pDoc				= NULL;
ITDHtml*		g_pHtml				= NULL;

REGISTER_LOCALED_DOCUMENT(CRegsterMap);

CRegsterMap::CRegsterMap(ITDDocument* pDoc)
{
	m_bInitialize	= FALSE;
	g_pDoc			= pDoc;
	g_pHtml			= pDoc->GetHtml(_T("regmap"));
	{
		// system's display memory == register map
		ITDMemory*	pMainMem	= g_pSystem->GetMemory();
		CString		sName;
		sName.Format(_T("%s_Display"), pMainMem->GetName());
		Regmap::m_pReg	= (MTSP_REGMAP*)pDoc->GetSystem()->GetMemory(sName)->GetPointer();
	}
	{
		// add register monitors
		new RegmapCommand;
		new RegmapSystem;
		new RegmapScratchCounter;
		new RegmapGPRs;
		new RegmapMB;
		new RegmapTrace;
	}
	g_pHtml->SetManager(this);
	g_pHtml->Navigate(_T("regmap.html"));
}

CRegsterMap::~CRegsterMap(void)
{
	g_pDoc->KillTimer(10);
	Regmap::ReleaseAll();
}

BOOL CRegsterMap::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData();
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

BOOL CRegsterMap::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	if(command == 10) {
		g_pDoc->KillTimer(10);
		g_pDoc->SetTimer(10, Regmap::Update() ? 50 : 1000);
	}

	return 0;
}

void CRegsterMap::OnSize(int width, int height)
{
	if(g_pHtml) {
		ITDLayout* pLayout = g_pHtml->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
		g_pHtml->GetObject()->UpdateLayout();
	}
}

LPCTSTR CRegsterMap::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if(m_bInitialize) {
		if(_tcsstr(lpszURL, _T("cmd://")) == lpszURL) {
			Regmap::Command(&lpszURL[6]);
		}

		return NULL;
	}

	return lpszURL;
}

void CRegsterMap::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialize) {
		m_bInitialize	= TRUE;
		Regmap::Initialize();
		g_pDoc->SetTimer(10, 50);
	}
}

void CRegsterMap::OnShow(BOOL bShow)
{
	if(!m_bInitialize) return;

	if(bShow)
		g_pDoc->SetTimer(10, 50);
	else
		g_pDoc->KillTimer(10);
}
