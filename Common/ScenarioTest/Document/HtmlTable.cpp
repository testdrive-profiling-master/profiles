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
// Title : Scenario test
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "HtmlTable.h"

HtmlTable::HtmlTable(void)
{
	m_pHtml		= NULL;
	m_pManager	= NULL;
}

HtmlTable::~HtmlTable(void)
{
}

void HtmlTable::Create(ITDDocument* pDoc, LPCTSTR sFileName, int sx, int sy, int width, int height)
{
	m_pHtml	= pDoc->CreateHtml(NULL, sx, sy, width, height);
	m_pHtml->SetManager(this);
	m_pHtml->Navigate(sFileName);
}

void HtmlTable::SetManager(ITDHtmlManager* pManager)
{
	m_pManager	= pManager;
}

LPCTSTR HtmlTable::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if(m_pManager) return m_pManager->OnHtmlBeforeNavigate(dwID, lpszURL);

	return lpszURL;
}

void HtmlTable::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(m_pManager) m_pManager->OnHtmlDocumentComplete(dwID, lpszURL);
}

void HtmlTable::Clear(void)
{
	m_pHtml->CallJScript(_T("ClearTable();"));
}
void HtmlTable::NewRow(void)
{
	m_pHtml->CallJScript(_T("NewRow();"));
}
void HtmlTable::NewTH(void)
{
	m_pHtml->CallJScript(_T("NewTH();"));
}
void HtmlTable::NewTD(void)
{
	m_pHtml->CallJScript(_T("NewTD();"));
}
void HtmlTable::NewCell(TABLE_CELL type, LPCTSTR sFormat, ...)
{
	m_pHtml->CallJScript((type == CELL_TD) ? _T("NewTD();") : _T("NewTH();"));

	if(sFormat) {
		TCHAR	sLine[1024];
		va_list	args;
		va_start(args, sFormat);
		//_vstprintf(sLine, sFormat, args);			// not exist in mingw
		_vsntprintf(sLine, 1024, sFormat, args);
		va_end(args);
		m_pHtml->CallJScript(_T("SetTID(\"%s\");"), sLine);
	}
}
void HtmlTable::SetTextAlignment(TABLE_ALIGN alignment)
{
	static LPCTSTR __ALIGNMENT[] = {
		_T("left"),
		_T("right"),
		_T("center")
	};
	m_pHtml->CallJScript(_T("SetTStyle(\"text-align\",\"%s\");"), __ALIGNMENT[alignment]);
}
void HtmlTable::SetBoarderWidth(TABLE_BOARDER side, int iWidth)
{
	static LPCTSTR __SIDE[] = {
		_T("left"),
		_T("right"),
		_T("top"),
		_T("bottom")
	};
	m_pHtml->CallJScript(_T("SetTStyle(\"border-%s\",\"%dpx\");"), __SIDE[side], iWidth);
}
void HtmlTable::SetSpan(TABLE_SPAN span, int iSize)
{
	static LPCTSTR __SPAN[] = {
		_T("Col"),
		_T("Row")
	};
	m_pHtml->CallJScript(_T("SetT%sSpan(\"%d\");"), __SPAN[span], iSize);
}
void HtmlTable::SetBody(LPCTSTR sBody)
{
	m_pHtml->CallJScript(_T("SetTBody(\"%s\");"), sBody);
}

void HtmlTable::JScript(LPCTSTR sFormat, ...)
{
	if(sFormat) {
		TCHAR	sLine[4096];
		va_list	args;
		va_start(args, sFormat);
		//_vstprintf(sLine, sFormat, args);			// not exist in mingw
		_vsntprintf(sLine, 1024, sFormat, args);
		va_end(args);
		m_pHtml->CallJScript(sLine);
	}
}
