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
// Title : System map
// Rev.  : 11/6/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "DesignMap.h"
#include "testdrive_document.inl"
#include "ProfileConfig.inl"
#include<fstream>

using namespace std;

REGISTER_LOCALED_DOCUMENT(CDesignMap);

ITDDocument*	g_pDoc		= NULL;
ITDHtml*		g_pHtml		= NULL;

int DecodeStringList(CString& str, LPCTSTR* pID)
{
	for(int i = 0; pID[i]; i++) if(!str.Compare(pID[i])) return i;

	return -1;
}

static bool __ReadSVG(CString& sContents, LPCTSTR sFilename)
{
	FILE* fp = _tfopen(sFilename, _T("r"));

	if(fp) {
		TCHAR	sLine[4096];
		bool	bSVG	= false;

		while(_fgetts(sLine, 4096, fp)) {
			if(!bSVG && _tcsstr(sLine, _T("<svg")) == sLine) bSVG = true;

			if(bSVG) {
				sContents	+= sLine;
			}
		}

		sContents.Replace(_T("\""), _T("'"));
		sContents.Replace(_T("\r"), _T(""));
		sContents.Replace(_T("\n"), _T(""));
		sContents.Replace(_T("<svg"), _T("<svg id='svg_object'"));
		//sContents.Replace(_T("xlink:href='"), _T("xlink:href='"));
		fclose(fp);
		return true;
	}

	return false;
}

static void __BuildHTML(LPCTSTR sFilename)
{
	CString sStr;
	_tfopen(sFilename, _T("r"));
}

CDesignMap::CDesignMap(ITDDocument* pDoc)
{
	m_bInitialized	= FALSE;
	g_pDoc			= pDoc;
	//g_pDoc->Show(FALSE);
	g_pHtml			= pDoc->GetHtml(_T("design_map"));
	g_pHtml->SetManager(this);
	g_pHtml->Navigate(_T("DesignMap.html"));
}

CDesignMap::~CDesignMap(void)
{
}

BOOL CDesignMap::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData();
	/*switch(pProperty->GetID()){
	default:
		break;
	}*/
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

BOOL CDesignMap::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CDesignMap::OnSize(int width, int height)
{
	if(g_pHtml) {
		ITDLayout* pLayout = g_pHtml->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
		g_pHtml->GetObject()->UpdateLayout();
	}
}

static LPCTSTR __sCommandID[CMD_ID_SIZE] = {
	_T("url"),
	_T("FIT_ON_SCREEN"),
};

static LPCTSTR __sCommandDeli	= _T("/?");
static LPCTSTR __sTokenDeli	= _T("?");

// Search string line from text file
int GetFileSearchLine(LPCTSTR sFileName, LPCTSTR sSearch)
{
	int line_count = -2;
	CStringA sToken(sSearch);
	FILE* fp = fopen(CStringA(sFileName), "rt");

	if(fp) {
		char line[4096];
		line_count	= 1;

		while(fgets(line, 4096, fp)) {
			if(strstr(line, sToken)) goto FOUND_TOKEN;

			line_count++;
		}

		line_count	= -1;
	FOUND_TOKEN:
		fclose(fp);
	}

	return line_count;
}

LPCTSTR CDesignMap::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) return lpszURL;

	g_pSystem->LogInfo(_T("lpszURL = %s"), lpszURL);
	{
		CString cmd(lpszURL), tag;
		CMD_ID	cmd_id = (CMD_ID) - 1;
		int iStart	= 0;
		// "http://"
		tag	= cmd.Tokenize(__sCommandDeli, iStart);
		// get command
		tag	= cmd.Tokenize(__sCommandDeli, iStart);

		for(int i = 0; i < CMD_ID_SIZE; i++)
			if(!tag.Compare(__sCommandID[i])) {
				cmd_id	= (CMD_ID)i;
				break;
			}

		switch(cmd_id) {
		case CMD_ID_URL: {
		} break;

		case CMD_ID_FIT_ON_SCREEN: {
			g_pSystem->LogInfo(_T("FIT_ON_SCREEN"));
		}
		break;

		default:
			if(_tcsstr(lpszURL, _T("http")) == lpszURL)
				return lpszURL;

			g_pSystem->LogInfo(_T("Invalid command : %s"), lpszURL);
		}
	}
	return NULL;
}

void CDesignMap::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) {
		g_pDoc->Show();
		g_pDoc->SetForegroundDocument();
		m_bInitialized	= TRUE;
		CString sContents;
		__ReadSVG(sContents, _T("D:/Project/Profiles/MobilintCI/System/HDL/DUTs/MTSP/rtl/MTSP_wrapper_hierarchy.svg"));
		g_pHtml->CallJScript(_T("SetContent(\"%s\");"), (LPCTSTR)sContents);
	}
}
