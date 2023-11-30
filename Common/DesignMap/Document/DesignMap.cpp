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
// Rev.  : 11/20/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "DesignMap.h"
#include "testdrive_document.inl"
#include "ProfileConfig.inl"
#include<fstream>

using namespace std;

REGISTER_LOCALED_DOCUMENT(CDesignMap);

ITDDocument*	g_pDoc		= NULL;
ITDHtml*		g_pHtml		= NULL;

static LPCTSTR	__DOC_NAME	= _T("DESIGN_MAP");

int DecodeStringList(CString& str, LPCTSTR* pID)
{
	for(int i = 0; pID[i]; i++) if(!str.Compare(pID[i])) return i;

	return -1;
}

BOOL IsFileExist(LPCTSTR sFileName)
{
	return _taccess(sFileName, 0) == 0;
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

		{
			// rid of 'URL' target.
			int iPos = 0;

			while((iPos = sContents.Find(_T("target=\""), iPos)) >= 0) {
				int iEnd	= sContents.Find(_T("\""), iPos + 8);

				if(iEnd > 0) {
					sContents.erase(iPos, iEnd - iPos + 1);
				}
			}
		}

		sContents.Replace(_T("\""), _T("'"));
		sContents.Replace(_T("\r"), _T(""));
		sContents.Replace(_T("\n"), _T(""));
		sContents.Replace(_T("<svg"), _T("<svg id='svg_object'"));
		sContents.Replace(_T("<a xlink:href='"), _T("<a xlink:href='cmd://URL/"));
		sContents.Replace(_T("cmd://URL/cmd://"), _T("cmd://"));				// cut off duplication of 'cmd://'
		fclose(fp);
		return true;
	}

	return false;
}

CDesignMap::CDesignMap(ITDDocument* pDoc)
{
	m_bInitialized	= FALSE;
	g_pDoc			= pDoc;
	{
		// get previous buffer
		CString sConfig;	// for clean assign to string
		g_pDoc->GetConfigString(_T("WORK_PATH"), sConfig.GetBuffer(4096), 4096);
		m_sWorkPath			= (LPCTSTR)sConfig;
		g_pDoc->GetConfigString(_T("OUTPUT_PATH"), sConfig.GetBuffer(4096), 4096);
		m_sOutputPath		= (LPCTSTR)sConfig;
		g_pDoc->GetConfigString(_T("DESIGN_PATH"), sConfig.GetBuffer(4096), 4096);
		m_sDesignFilePath	= (LPCTSTR)sConfig;
	}
	g_pHtml			= pDoc->GetHtml(_T("design_map"));
	g_pHtml->SetManager(this);
	g_pHtml->Navigate(_T("DesignMap.html"));
	{
		// get memory
		CString	sName;
		// get system memory & it's name
		m_pMemory	= g_pSystem->GetMemory();

		if(m_pMemory) sName	= m_pMemory->GetName();

		sName	+= _T("_DesignMap");
		m_pMemory	= g_pSystem->GetMemory(sName, TRUE);

		if(sName.Compare(m_pMemory->GetName())) {	// if not existed, create!
			m_pMemory->Create(1024 * 16, sName);	// 16KB
		}
	}
	m_pConfig		= (DESIGNMAP_CONFIG*)m_pMemory->GetConfig();
	m_pConfig->hWnd	= pDoc->GetWindowHandle();
}

CDesignMap::~CDesignMap(void)
{
	UpdateSourceViews(true);
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

void CDesignMap::UpdateSourceViews(bool bNewProject)
{
	// refresh opened source view
	for(auto i = m_SourceViewList.begin(); i != m_SourceViewList.end();) {
		auto next = i;
		next++;
		LPCTSTR sTarget	= (LPCTSTR)i->first;
		HWND hWnd = FindWindow(NULL, sTarget);

		if(hWnd) {
			SOURCE_VIEW& view	= i->second;

			if(bNewProject) {
				// close old project source view page
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			} else {
				// reopen the result source
				g_pHtml->CallJScript(_T("OpenURL('%s', '%s');"), (LPCTSTR)view.sFilename, (LPCTSTR)sTarget);
			}
		} else {
			m_SourceViewList.erase(i);
		}

		i = next;
	}

	if(bNewProject) m_SourceViewList.clear();
}

BOOL CDesignMap::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch(command) {
	case MAIN_CMD_WATCHDOG_FREE:
		g_pDoc->KillTimer(MAIN_CMD_WATCHDOG_FREE);
		SetEnvironmentVariable(_T("TESTDRIVE_WATCHDOG"), NULL);
		break;

	case TD_EXTERNAL_COMMAND:
		switch(wParam) {
		case USER_CMD_UPDATE:
			CString	sWorkPath, sOutputPath, sDesignFileName;
			{
				int			iPos	= 0;
				LPCTSTR		sDelim	= _T(";");
				CString	sMsg((char*)m_pMemory->GetPointer());
				sWorkPath			= sMsg.Tokenize(sDelim, iPos);
				sOutputPath			= sMsg.Tokenize(sDelim, iPos);
				sDesignFileName		= sMsg.Tokenize(sDelim, iPos);
			}
			bool	bNewProject = (sWorkPath != m_sWorkPath);
			m_sDesignFilePath.Format(_T("%s\\%s"), (LPCTSTR)sOutputPath, (LPCTSTR)sDesignFileName);

			if(IsFileExist(m_sDesignFilePath)) {
				{
					// open design
					CString sContents;
					__ReadSVG(sContents, m_sDesignFilePath);
					g_pHtml->CallJScript(_T("SetContent(\"%s\", %s);"), (LPCTSTR)sContents, bNewProject ? _T("false") : _T("true"));
				}
				{
					// refresh opened source view
					UpdateSourceViews(bNewProject);
				}
				m_sWorkPath		= sWorkPath;
				m_sOutputPath	= sOutputPath;
			} else {
				g_pSystem->LogError(_T("Design '%s' is not found."), (LPCTSTR)m_sDesignFilePath);
				m_sWorkPath.clear();
				m_sOutputPath.clear();
				m_sDesignFilePath.clear();
				g_pHtml->CallJScript(_T("ResetDesignMap();"));
			}

			g_pDoc->SetConfigString(_T("WORK_PATH"), m_sWorkPath);
			g_pDoc->SetConfigString(_T("OUTPUT_PATH"), m_sOutputPath);
			g_pDoc->SetConfigString(_T("DESIGN_PATH"), m_sDesignFilePath);
			break;
		}

		break;
	}

	return TRUE;
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
	_T("URL"),
	_T("DOC"),
	_T("MANUAL"),
	_T("LUA"),
	_T("PROJECT"),
	_T("NEW_MODULE_FILE"),
};

static LPCTSTR __sCommandDeli	= _T("/?");
static LPCTSTR __sTokenDeli		= _T("?");
static LPCTSTR __sExcelDeli		= _T("@");

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

	{
		CString cmd(lpszURL), tag;
		int iStart	= 0;
		tag	= cmd.Tokenize(__sCommandDeli, iStart);

		if(tag == _T("cmd:")) {
			// get command
			tag	= cmd.Tokenize(__sCommandDeli, iStart);
			CMD_ID	cmd_id = (CMD_ID) - 1;

			for(int i = 0; i < CMD_ID_SIZE; i++)
				if(!tag.Compare(__sCommandID[i])) {
					cmd_id	= (CMD_ID)i;
					break;
				}

			switch(cmd_id) {
			case CMD_ID_URL: {
				CString sLink		= cmd.Tokenize(_T(""), iStart);
				CString sTarget		= sLink;
				CString sSource;
				{
					// get target name
					int iPos = sTarget.ReverseFind(_T('/'));

					if(iPos > 0) sTarget.erase(0, iPos + 1);

					iPos = sTarget.ReverseFind(_T('.'));

					if(iPos > 0) sTarget.erase(iPos, -1);
				}
				// check file existence
				{
					sSource.Format(_T("%s\\%s"), (LPCTSTR)m_sOutputPath, (LPCTSTR)sLink);
					sSource.Replace(_T("/"), _T("\\"));	// windows style

					if(!IsFileExist(sSource)) {
						g_pSystem->LogError(_T("File is not found : %s"), (LPCTSTR)sSource);
						break;
					}
				}
				// add to source view list
				sSource.Replace(_T("\\"), _T("/"));	// web style
				{
					SOURCE_VIEW& src_view	= m_SourceViewList[sTarget];
					src_view.sFilename		= sSource;
				}
				// open with HTML verilog output page
				g_pHtml->CallJScript(_T("OpenURL('%s', '%s');"), (LPCTSTR)sSource, (LPCTSTR)sTarget);
			}
			break;

			case CMD_ID_DOC: {
				CString sDoc		= cmd.Tokenize(_T(""), iStart);

				if(sDoc.rfind(_T(".xlsx")) == (sDoc.length() - 5)) {	// open excel
					CString sArg;

					if(sDoc.Find(_T('@')) < 0) {
						sArg.Format(_T("%s\\%s"), (LPCTSTR)m_sWorkPath, (LPCTSTR)sDoc);
					} else {	// open excel with worksheet
						iStart = 0;
						CString sSheet = sDoc.Tokenize(__sExcelDeli, iStart);
						CString sFileName = sDoc.Tokenize(_T(""), iStart);
						sArg.Format(_T("\"%s\\%s\" %s"), (LPCTSTR)m_sWorkPath, (LPCTSTR)sFileName, (LPCTSTR)sSheet);
					}

					g_pSystem->ExecuteFile(_T("%TESTDRIVE_PROFILE%common\\bin\\xlsx_open.bat"), sArg, TRUE, NULL, _T("%TESTDRIVE_PROFILE%common\\bin"),
										   _T("*E: "), -1,
										   NULL);
				} else {
					CString sExe;
					CString sWorkPath;
					sExe.Format(_T("%s\\%s"), (LPCTSTR)m_sWorkPath, (LPCTSTR)sDoc);
					sExe.Replace(_T("/"), _T("\\"));
					sWorkPath	= sExe;
					int iPos = sWorkPath.rfind(_T("\\"));
					if(iPos>0)sWorkPath.erase(iPos+1, -1);

					g_pSystem->ExecuteFile(sExe, NULL, TRUE, NULL, sWorkPath, NULL);
				}
			}
			break;

			case CMD_ID_MANUAL: {
				CString sName		= cmd.Tokenize(_T(""), iStart);
				CString sLink;
				sLink.Format(_T("https://testdrive-profiling-master.github.io/download/%s_userguide.pdf"), (LPCTSTR)sName);
				ShellExecute(NULL, _T("open"), sLink, NULL, NULL, SW_SHOWDEFAULT);
			}
			break;

			case CMD_ID_LUA: {
				CString sFileName		= cmd.Tokenize(__sCommandDeli, iStart);
				OpenEditFile(sFileName);
			}
			break;

			case CMD_ID_PROJECT: {
				ShellExecute(NULL, _T("open"), _T("explorer"), m_sWorkPath, NULL, SW_SHOWDEFAULT);
			}
			break;

			case CMD_ID_NEW_MODULE_FILE: {
				CString sModuleName		= cmd.Tokenize(__sCommandDeli, iStart);

				if(OpenSourceFile(sModuleName, TRUE)) {
					CString sMsg;
					sMsg.Format(_L(MODULE_SOURCE_FILE_CREATED), (LPCTSTR)sModuleName);
					g_pHtml->CallJScript(_T("toastr.info(\"%s\");"), (LPCTSTR)sMsg);
				}
			}
			break;

			default:
				if(_tcsstr(lpszURL, _T("http")) == lpszURL)
					return lpszURL;

				g_pSystem->LogInfo(_T("Invalid command : %s"), lpszURL);
			}

			return NULL;
		}
	}
	return lpszURL;
}

void CDesignMap::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) {
		g_pDoc->SetForegroundDocument();
		m_bInitialized	= TRUE;

		if(!m_sDesignFilePath.IsEmpty() && IsFileExist(m_sDesignFilePath)) {
			CString sContents;
			__ReadSVG(sContents, m_sDesignFilePath);
			g_pHtml->CallJScript(_T("SetContent(\"%s\");"), (LPCTSTR)sContents);
		}
	}
}

int CDesignMap::CheckModuleFile(LPCTSTR sFileName, LPCTSTR sModuleName)
{
	int iLine	= 0;
	CString sTargetFileName;
	sTargetFileName.Format(_T("%s\\%s"), (LPCTSTR)m_sWorkPath, (LPCTSTR)sFileName);
	FILE* fp	= _tfopen(sTargetFileName, _T("rt"));

	if(fp) {
		TCHAR	sLine[1024 * 16];
		LPCTSTR	sDelim	= _T(" \t\r\n");

		while(!feof(fp) && _fgetts(sLine, 1024 * 16, fp)) {
			CString s(sLine);
			int		iPos = 0;
			CString	sTag	= s.Tokenize(sDelim, iPos);
			iLine++;

			if(sTag == _T("module")) {
				CString	sName	= s.Tokenize(sDelim, iPos);

				if(sName == sModuleName) {
					return iLine;
				}
			}
		}

		fclose(fp);
	}

	return -1;
}

bool CDesignMap::OpenSourceFile(LPCTSTR sModuleName, BOOL bMustOpen)
{
	CString				sFoundFileName;
	int					iFoundLineNumber	= 0;
	{
		// searching verigen files
		WIN32_FIND_DATA		FindFileData;
		HANDLE				hFind;
		CString				sPath;
		sPath.Format(_T("%s\\*.sv"), (LPCTSTR)m_sWorkPath);

		if((hFind = FindFirstFile(sPath, &FindFileData)) != INVALID_HANDLE_VALUE) {
			BOOL	bSearch	= TRUE;

			while(bSearch) {
				if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
					int iLineNumber	= CheckModuleFile(FindFileData.cFileName, sModuleName);

					if(iLineNumber >= 0) {
						sFoundFileName		= FindFileData.cFileName;
						iFoundLineNumber	= iLineNumber;
						break;
					}
				}

				bSearch	= FindNextFile(hFind, &FindFileData);
			}

			FindClose(hFind);
		}
	}
	// editor function
	auto OpenEditor = [](LPCTSTR sFileName, LPCTSTR sWorkPath, int iLineNumber) {
		CString notepad_path	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%bin\\notepad\\notepad++.exe"));
		CString sArg;
		sArg.Format(_T("-n%d \"%s\\%s\""), iLineNumber, sWorkPath, sFileName);
		ShellExecute(NULL, _T("open"), notepad_path, sArg, NULL, SW_SHOWDEFAULT);
	};

	// open the verigen source
	if(!sFoundFileName.IsEmpty()) {
		OpenEditor(sFoundFileName, m_sWorkPath, iFoundLineNumber);
		return true;
	} else if(bMustOpen) {
		sFoundFileName	= sModuleName;
		{
			// name fix for verigen.
			int iPos = sFoundFileName.Find(_T('_'));

			if(iPos > 0) sFoundFileName.erase(0, iPos + 1);

			sFoundFileName.Insert(0, _T("__"));
			sFoundFileName	+= ".sv";
		}
		{
			// create new file
			CString sNewFilePath;
			sNewFilePath.Format(_T("%s\\%s"), (LPCTSTR)m_sWorkPath, (LPCTSTR)sFoundFileName);
			{
				// create new or append to existed file.
				bool bExist	= IsFileExist(sNewFilePath);
				SetEnvironmentVariable(_T("TESTDRIVE_WATCHDOG"), _T("0"));
				FILE* fp	= _tfopen(sNewFilePath, bExist ? _T("at") : _T("wt"));

				if(fp) {
					// insert separator
					if(bExist) _fputts(_T("\n//#==============================================================================================================\n"), fp);

					CString sModuleDeclare;
					sModuleDeclare.Format(_T("module %s\n"), sModuleName);
					_fputts(sModuleDeclare, fp);
					fclose(fp);
					g_pDoc->SetTimer(MAIN_CMD_WATCHDOG_FREE, 500);
				} else {
					g_pDoc->SetTimer(MAIN_CMD_WATCHDOG_FREE, 0);
					return false;
				}
			}
		}
		return OpenSourceFile(sModuleName);
	} else {
		CString sMsg;
		sMsg.Format(_L(CANT_FIND_MODULE_SOURCE), (LPCTSTR)sModuleName, (LPCTSTR)sModuleName);
		g_pHtml->CallJScript(_T("toastr.error(\"%s\");"), (LPCTSTR)sMsg);
	}

	return false;
}

void CDesignMap::OpenEditFile(LPCTSTR sFileName)
{
	CString notepad_path	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%bin\\notepad\\notepad++.exe"));
	CString sArg;
	sArg.Format(_T("\"%s\\%s\""), (LPCTSTR)m_sWorkPath, sFileName);
	ShellExecute(NULL, _T("open"), notepad_path, sArg, NULL, SW_SHOWDEFAULT);
}

LPCTSTR CDesignMap::OnHtmlNewWindowRequest(DWORD dwID, LPCTSTR lpszURL, BOOL bUserInitiated)
{
	CString cmd(lpszURL), tag;
	int iStart	= 0;
	tag	= cmd.Tokenize(__sCommandDeli, iStart);

	if(tag == _T("cmd:")) {
		tag	= cmd.Tokenize(__sCommandDeli, iStart);

		if(tag == _T("URL")) {
			CString sModuleName;
			// get module name
			sModuleName	= cmd.Tokenize(_T(""), iStart);
			{
				int iPos	= sModuleName.ReverseFind(_T("/"));

				if(iPos >= 0)  sModuleName.erase(0, iPos + 1);

				iPos		= sModuleName.Find(_T("."));

				if(iPos >= 0) sModuleName.erase(iPos, -1);
			}
			// open source file
			OpenSourceFile(sModuleName);
		}

		return NULL;
	}

	return lpszURL;
}
