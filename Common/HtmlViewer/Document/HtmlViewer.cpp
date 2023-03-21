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
// Title : HTML viewer
// Rev.  : 3/21/2023 Tue (clonextop@gmail.com)
//================================================================================
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "HtmlViewer.h"
#include "testdrive_document.inl"

ITDSystem*	g_pSystem	= NULL;

ITDImplDocument* __cdecl RegisterDocument(ITDDocument* pDoc)
{
	g_pSystem	= pDoc->GetSystem();

	if(g_pSystem->GetVersion() != TESTDRIVE_VERSION) return NULL;

	{
		TCHAR	name[1024], *pTok;
		_tcscpy(name, pDoc->DocumentTitle());
		pTok = _tcsstr(name, _T(":"));

		if(pTok) {
			if(*pTok) *pTok = NULL;

			ITDDocument* pPrevDoc = pDoc->GetSystem()->GetDocument(name);

			if(pPrevDoc) {
				pPrevDoc->SetForegroundDocument();
				pDoc->GetSystem()->LogInfo(_T("Already another document '%s' is opened."), name);
				return NULL;
			}
		}
	}
	return new HtmlViewer(pDoc);
}

HtmlViewer::HtmlViewer(ITDDocument* pDoc)
{
	m_pDoc		= pDoc;
	m_pHtml		= pDoc->CreateHtml(_T("html"), 0, 0, 100, 100, TRUE);
	m_ValidationCheck.bEnable	= FALSE;
	m_pHtml->SetManager(this);
	{
		// Open HTML Document.
		m_sURL = pDoc->DocumentTitle();

		// 'name':'url'[:c+'class' , :+i'id' or :+o'object']
		if(m_sURL.Find(_T(":")) != -1) {
			TCHAR	name[1024], *pUrl, *pVal;
			_tcscpy(name, m_sURL);
			pUrl	= _tcsstr(name, _T(":"));
			*pUrl	= NULL;
			pUrl++;
			pVal	= _tcsstr(pUrl, _T(":+"));

			if(pVal) {
				*pVal	= NULL;
				pVal	+= 2;
				m_ValidationCheck.bEnable	= TRUE;
				m_ValidationCheck.FuncType	= (*pVal == _T('c')) ? GET_FUNCTION_TYPE_CLASS :
											  (*pVal == _T('i')) ? GET_FUNCTION_TYPE_ID : GET_FUNCTION_TYPE_NAME;
				pVal++;
				m_ValidationCheck.sName		= pVal;
			}

			pDoc->DocumentTitle(name);
			m_sURL = pUrl;
		} else {
			m_sURL += _T(".html");
		}

		m_pHtml->Navigate(m_sURL);
	}
}

HtmlViewer::~HtmlViewer(void)
{
}


void HtmlViewer::OnSize(int width, int height)
{
	if(m_pHtml) {
		ITDLayout* pLayout = m_pHtml->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
		m_pHtml->GetObject()->UpdateLayout();
	}
}

LPCTSTR HtmlViewer::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	return lpszURL;
}
void HtmlViewer::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(m_ValidationCheck.bEnable) {
		static LPCTSTR	__sGetFunction[] = {
			_T("getElementsByClassName"),
			_T("getElementById"),
			_T("getElementsByName"),
		};
		m_pHtml->CallJScript(_T("if(document.%s('%s')==null) window.location = 'res://ieframe.dll/dnserror.htm';"),
							 __sGetFunction[(int)m_ValidationCheck.FuncType],
							 (LPCTSTR)m_ValidationCheck.sName);
		m_ValidationCheck.bEnable	= FALSE;
	}

	{
		CString	sUrl(lpszURL);

		if(sUrl.Find(_T("/dnserror.")) > 0) {
			g_pSystem->LogWarning(_T("%s('%s') is not a valid page. Maybe your internet connection is not established or blocked by fire-wall."), m_pDoc->DocumentTitle(), (LPCTSTR)m_sURL);
			m_pDoc->SetAutoClose();
			m_pDoc->UnLock();
		}
	}
}

BOOL HtmlViewer::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
