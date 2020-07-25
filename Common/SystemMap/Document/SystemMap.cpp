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
// Title : System map
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "SystemMap.h"
#include "testdrive_document.inl"
#include "ProfileConfig.inl"

REGISTER_LOCALED_DOCUMENT(CSystemMap);

ITDDocument*	g_pDoc		= NULL;
ITDHtml*		g_pHtml		= NULL;

int DecodeStringList(CString& str, LPCTSTR* pID)
{
	for(int i = 0; pID[i]; i++) if(!str.Compare(pID[i])) return i;

	return -1;
}

CSystemMap::CSystemMap(ITDDocument* pDoc)
{
	m_bInitialized	= FALSE;
	g_pDoc			= pDoc;
	g_pHtml			= pDoc->GetHtml(_T("system_map"));
	g_pHtml->SetManager(this);
	{
		CString sPath;
		CString sURL;
		sURL.Format(_T("SystemMap.html?file=%s"), g_ProfileConfig.GetPath(_T("FILE")));
		g_pHtml->Navigate(sURL);
	}
}

CSystemMap::~CSystemMap(void)
{
}

BOOL CSystemMap::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData();
	/*switch(pProperty->GetID()){
	default:
		break;
	}*/
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

BOOL CSystemMap::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CSystemMap::OnSize(int width, int height)
{
	if(g_pHtml) {
		ITDLayout* pLayout = g_pHtml->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
		g_pHtml->GetObject()->UpdateLayout();
	}
}

static LPCTSTR __sCommandID[CMD_ID_SIZE] = {
	_T("shell"),
	_T("execute"),
	_T("profile"),
	_T("edit"),
	_T("visio"),
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

LPCTSTR CSystemMap::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) return lpszURL;

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
		case CMD_ID_SHELL:
		case CMD_ID_EXCUTE:
		case CMD_ID_PROFILE:
		case CMD_ID_EDIT: {
			CString filename, params, workdir;
			filename							= cmd.Tokenize(__sTokenDeli, iStart);

			if(!filename.IsEmpty())	params		= cmd.Tokenize(__sTokenDeli, iStart);

			if(!params.IsEmpty())	workdir		= cmd.Tokenize(__sTokenDeli, iStart);

			//g_pSystem->LogInfo(_T("test : %s, %s %s"), (LPCTSTR)filename, (LPCTSTR)params, (LPCTSTR)workdir);

			if(workdir.IsEmpty()) workdir	= g_pSystem->GetProjectPath();

			if(filename.Find(_T("%")) != -1)	filename	= g_pSystem->RetrieveFullPath(filename);

			if(params.Find(_T("%")) != -1)	params		= g_pSystem->RetrieveFullPath(params);

			if(workdir.Find(_T("%")) != -1)	workdir		= g_pSystem->RetrieveFullPath(workdir);

			switch(cmd_id) {
			case CMD_ID_SHELL:
				ShellExecute(NULL, _T("open"), filename, params.IsEmpty() ? (LPCTSTR)NULL : (LPCTSTR)params, workdir, SW_SHOWDEFAULT);
				break;

			case CMD_ID_EXCUTE:
				g_pSystem->ExecuteFile(filename, params.IsEmpty() ? (LPCTSTR)NULL : (LPCTSTR)params, TRUE, NULL, workdir, NULL);
				break;

			case CMD_ID_PROFILE:
				g_pSystem->RunProfile(filename, TRUE);
				break;

			case CMD_ID_EDIT: {
				CString notepad_path	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%bin\\notepad\\notepad++.exe"));
				CString sArg;
				int iLineNumber			= GetFileSearchLine(filename, params);

				switch(iLineNumber) {
				case -2:
					g_pSystem->LogError(_L(CANT_FIND_FILE));
					break;

				case -1:
					g_pSystem->LogWarning(_L(CANT_FIND_TAG_STRING), (LPCTSTR)params);

				default:
					if(iLineNumber > 0) sArg.AppendFormat(_T("-n%d "), iLineNumber);

					sArg.AppendFormat(_T("\"%s\""), (LPCTSTR)filename);
					ShellExecute(NULL, _T("open"), notepad_path, sArg, NULL, SW_SHOWDEFAULT);
					break;
				}
			}
			break;
			}
		}
		break;

		case CMD_ID_VISIO: {
			CString filename	= cmd.Tokenize(__sTokenDeli, iStart);
			filename	= g_pSystem->RetrieveFullPath(filename);
			filename.Replace(_T("\\"), _T("/"));
			g_pHtml->CallJScript(_T("SetVisioFile(\"%s\");"), (LPCTSTR)filename);
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

void CSystemMap::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) m_bInitialized	= TRUE;

	if(_tcsstr(lpszURL, _T("google"))) {
		static BOOL bWarned	= FALSE;

		if(!bWarned) {
			g_pSystem->LogWarning(_L(VISIO_INSTALLATION_IS_NEEDED));
			g_pDoc->SetForegroundDocument();
			bWarned	= TRUE;
		}
	}
}
