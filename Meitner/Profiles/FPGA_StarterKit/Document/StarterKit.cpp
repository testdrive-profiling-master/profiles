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
// Rev.  : 12/31/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "testdrive_document.inl"
#include "StarterKit.h"
#include "RegmapLED.h"
#include "RegmapButton.h"
#include "RegmapNumericDisplay.h"
#include "RegmapTFT.h"
#include "RegmapMotor.h"

ITDDocument*	g_pDoc				= NULL;
ITDHtml*		g_pHtml				= NULL;

REGISTER_LOCALED_DOCUMENT(StarterKit);

StarterKit::StarterKit(ITDDocument* pDoc)
{
	m_bInitialized	= FALSE;
	g_pDoc			= pDoc;
	g_pHtml			= pDoc->GetHtml(_T("StarterKit"));
	{
		// system's display memory == register map
		ITDMemory*	pMainMem	= g_pSystem->GetMemory();
		CString		sName;
		sName.Format(_T("%s_Display"), pMainMem->GetName());
		Regmap::m_pReg	= (STARTERKIT_REGMAP*)pDoc->GetSystem()->GetMemory(sName)->GetPointer();
	}
	{
		// add register monitors
		new RegmapLED;
		new RegmapButton;
		new RegmapNumericDisplay;
		new RegmapTFT;
		new RegmapMotor;
	}
	g_sMediaPath	= g_pSystem->RetrieveFullPath(_T("./media/"));
	g_pHtml->SetManager(this);
	g_pHtml->Navigate(_T("StarterKit.html"));
}

StarterKit::~StarterKit(void)
{
	g_pDoc->KillTimer(10);
	Regmap::ReleaseAll();
}

BOOL StarterKit::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData();
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

BOOL StarterKit::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	if(command == 10) {
		g_pDoc->KillTimer(10);
		g_pDoc->SetTimer(10, Regmap::Update() ? 10 : 30);
	}

	return 0;
}

void StarterKit::OnSize(int width, int height)
{
	if(g_pHtml) {
		ITDLayout* pLayout = g_pHtml->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
		g_pHtml->GetObject()->UpdateLayout();
	}
}

LPCTSTR StarterKit::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if(m_bInitialized) {
		if(_tcsstr(lpszURL, _T("cmd://")) == lpszURL) {
			Regmap::Command(&lpszURL[6]);
		} else
			return NULL;
	}

	return lpszURL;
}

void StarterKit::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) {
		m_bInitialized	= TRUE;
		Regmap::Initialize();
		g_pDoc->SetTimer(10, 50);
	}
}

void StarterKit::OnShow(BOOL bShow)
{
	if(!m_bInitialized) return;

	if(bShow)
		g_pDoc->SetTimer(10, 50);
	else
		g_pDoc->KillTimer(10);
}
