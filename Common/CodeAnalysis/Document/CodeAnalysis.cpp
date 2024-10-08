//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Code Analysis
// Rev.  : 8/19/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "CodeAnalysis.h"
#include "testdrive_document.inl"
#include "ProfileConfig.inl"

REGISTER_LOCALED_DOCUMENT(CodeAnalysis)

CodeAnalysis *CodeAnalysis::m_pCurrent = NULL;
DWORD		  g_dwErrorCount		   = 0;
DWORD		  g_dwWarningCount		   = 0;
BOOL		  g_bLogSuppress		   = TRUE;
CString		  g_sCurrentDir;

typedef enum {
	CPPCHECK_TOKEN_ERROR,
	CPPCHECK_TOKEN_WARNING,
	CPPCHECK_TOKEN_INFORMATION,
} CPPCHECK_TOKEN;

const LPCTSTR __sCppcheckTokenList[] = {
	_T(": error:"),
	_T(": warning:"),
	_T(": information:"),
};

CodeAnalysis::CodeAnalysis(ITDDocument *pDoc)
{
	m_pCurrent = this;
	m_pDoc	   = pDoc;
	m_pReport  = pDoc->CreateReport(NULL, 0, 0, 10, 10);
	m_pReport->ShowScrollBar(TRUE);
	m_pReport->EnableEdit(FALSE);
	m_pReport->SetManager(this);
	{
		ITDPropertyData *pProperty;
		// Log suppression
		pProperty = pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, 0, _L(LOG_SURPPRESS), (DWORD_PTR)&g_bLogSuppress, _L(DESC_LOG_SURPPRESS));
		pProperty->UpdateConfigFile();
		// Error suppression
		m_sErrorSuppress.GetBuffer(4096);
		pProperty = pDoc->AddPropertyData(
			PROPERTY_TYPE_STRING, 1, _L(STATIC_ANALYSIS_SURPPRESS), (DWORD_PTR)(LPCTSTR)m_sErrorSuppress, _L(DESC_STATIC_ANALYSIS_SURPPRESS));
		pProperty->UpdateConfigFile();
	}
	m_pReport->SetText(_T("Code Analysis.\n"));
	m_sDocPath = g_pSystem->RetrieveFullPath(_T("%CURRENT%"));
	pDoc->SetTimer(0, 10);
}

CodeAnalysis::~CodeAnalysis(void)
{
	m_pCurrent = NULL;
}

BOOL CodeAnalysis::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch (command) {
	case 0:
		m_pDoc->KillTimer(0);
		DoStaticCodeAnalysis();
		break;

	default:
		break;
	}

	return TRUE;
}

BOOL CodeAnalysis::OnPropertyUpdate(ITDPropertyData *pProperty)
{
	pProperty->UpdateData();
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

void CodeAnalysis::OnSize(int width, int height)
{
	if (m_pReport) {
		ITDLayout *pLayout = m_pReport->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
	}
}

void CodeAnalysis::OnReportLink(DWORD dwID, LPCTSTR lpszString, long cpMin, long cpMax)
{
	if (_tcsstr(lpszString, _T("edit:")) == lpszString) {
		CString sNotepad, sFile;
		sNotepad = g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%bin\\notepad\\notepad++.exe"));
		lpszString += 5;

		if (_tcsstr(lpszString, _T("@"))) {
			sFile.Format(_T("\"%s"), lpszString);
			sFile.Replace(_T("@"), _T("\" -n"));
		} else {
			sFile.Format(_T("\"%s\""), lpszString);
		}

		ShellExecute(NULL, NULL, sNotepad, sFile, NULL, SW_SHOW);
	} else if (_tcsstr(lpszString, _T("open:")) == lpszString) {
		lpszString += 5;
		CString sPath(g_pSystem->RetrieveFullPath(lpszString));
		sPath.Replace(_T('\\'), _T('/'));
		sPath += _T('/');
		ShellExecute(NULL, NULL, sPath, NULL, NULL, SW_SHOW);
	}
}

static CString __sWorkPath;
void		   ChangeToWorkPath(CString &sPath)
{
	sPath.Replace(_T('\\'), _T('/'));
	{
		CString sOrgPath((LPCTSTR)sPath);
		{
			int iPos = 0;

			while ((iPos = sOrgPath.Find(_T(':'), iPos)) > 0) {
				TCHAR ch = sOrgPath[iPos + 1];

				if (isdigit(sOrgPath[iPos + 1])) {
					sOrgPath.Delete(iPos, -1);
					break;
				}

				iPos++;
			}
		}
		{
			TCHAR sCurrentDir[4096];
			GetCurrentDirectory(4096, sCurrentDir);
			SetCurrentDirectory((LPCTSTR)__sWorkPath);
			CString sFullPath(g_pSystem->RetrieveFullPath(sOrgPath));
			sFullPath.Replace(_T('\\'), _T('/'));
			sPath.Replace(sOrgPath, sFullPath);
			SetCurrentDirectory(sCurrentDir);
		}
	}
}

const char *Log_StaticAnalysis(LPCTSTR lpszLog, int iID)
{
	CodeAnalysis *pAnalysis	 = CodeAnalysis::GetCurrent();
	ITDReport	 *pReport	 = pAnalysis->Report();
	static long	  start		 = -1;
	static bool	  bSupressed = true;
	static bool	  bPrevInfo	 = false;

	if (!lpszLog) {
		start = -1;
		return NULL;
	}

	{
		CString str(lpszLog);
		str.TrimLeft(_T("\n\r"));

		if (!str.GetLength())
			return NULL;

		if (g_bLogSuppress && bPrevInfo && (str.GetAt(0) == _T('^'))) {
			bPrevInfo  = false;
			bSupressed = true;
			return NULL;
		}

		if (g_bLogSuppress && start >= 0 && !iID && bSupressed) {
			pReport->ReplaceText(start, -1, _T(""));
			start = -1;
		}

		switch (iID) {
		case -1:   // on error
		case -2: { // on warning
			LPCTSTR sCppcheckToken = __sCppcheckTokenList[-iID - 1];
			pAnalysis->Document()->SetForegroundDocument();

			if (_tcsstr((LPCTSTR)str, _T("cppcheck")) == (LPCTSTR)str) {
				goto NORMAL_LOGOUT;
			}

			if (iID == -1) { // on error
				pReport->SetColor(RGB(255, 0, 0));
				g_dwErrorCount++;
			} else { // on warning
				pReport->SetColor(RGB(250, 100, 0));
				g_dwWarningCount++;
			}

			{
				LPTSTR sLast = (LPTSTR)_tcsstr((LPCTSTR)str, sCppcheckToken);

				if (sLast) {
					if ((sLast - (LPCTSTR)str) > 2) {
						LPCTSTR sLinePos = _tcsstr((LPCTSTR)str + 2, _T(":"));

						if (sLinePos) {
							LPCTSTR sLinePos2 = _tcsstr(sLinePos + 1, _T(":"));

							if (sLinePos2)
								sLast = (LPTSTR)sLinePos2;
						}
					}

					CString strLast(sLast);
					*sLast = _T('\0');
					CString sLink((LPCTSTR)str);
					{
						int iPos = str.Find(':');

						if (iPos < 0 || iPos > 1) {
							sLink.Format(_T("%s/%s"), (LPCTSTR)g_sCurrentDir, (LPCTSTR)str);
							sLink = g_pSystem->RetrieveFullPath(sLink);
							sLink.Replace(_T("\\"), _T("/"));
						}

						// separate argument
						iPos = sLink.ReverseFind(_T(':'));

						if (iPos > 0) {
							int iPos2 = sLink.ReverseFind(_T(":"), iPos - 1);

							if (iPos2 > 2)
								iPos = iPos2;

							if (iPos > 2)
								sLink.SetAt(iPos, _T('@'));
						}
					}
					sLink.Replace(_T('\\'), _T('/'));
					sLink.Insert(0, _T("edit:"));
					pReport->AppendText(_T("\t"));
					pReport->SetLink(sLink);
					pReport->AppendText(_T("%s"), (LPCTSTR)str);
					pReport->AppendText(_T("%s"), (LPCTSTR)strLast);
				} else {
					pReport->AppendText(_T("\t%s"), lpszLog);
				}
			}

			bSupressed = false;
		} break;

		default: { // not error
		NORMAL_LOGOUT:
			start = pReport->GetTextLength();
			{
				COLORREF color;

				switch (iID) {
				case 1:
					color = RGB(0, 0, 255);
					break;

				case 2:
					color = RGB(0, 128, 0);
					break;

				case 3:
					color = RGB(128, 128, 0);
					break;

				default:
					color = RGB(0, 0, 0);
				}

				pReport->SetColor(RGB(0, 0, 0));
			}

			if (_tcsstr((LPCTSTR)str, _T("Checking")) == (LPCTSTR)str) {
				LPCTSTR sLast = NULL;
				CString strLast;
				pReport->AppendText(_T("\tChecking "));
				str.Delete(0, 9);
				sLast = _tcsstr((LPCTSTR)str + 2, _T(":"));

				if (!sLast)
					sLast = _tcsstr((LPCTSTR)str + 2, _T("..."));

				if (sLast)
					strLast = sLast;

				*(PSTR)sLast = _T('\0');
				{
					CString sLink;

					if (str.Find(':') >= 0)
						sLink = str;
					else {
						sLink.Format(_T("%s/%s"), (LPCTSTR)g_sCurrentDir, (LPCTSTR)str);
						sLink = g_pSystem->RetrieveFullPath(sLink);
						sLink.Replace(_T("\\"), _T("/"));
					}

					sLink.Insert(0, _T("edit:"));
					pReport->SetLink(sLink);
				}
				pReport->AppendText(_T("%s"), (LPCTSTR)str);
				pReport->AppendText(_T("%s"), (LPCTSTR)strLast);
				bSupressed = true;
			} else {
				pReport->AppendText(_T("\t%s"), (LPCTSTR)str);
				bSupressed = (str.GetAt(0) != _T(' '));
			}
		} break;
		}
		bPrevInfo = (iID == 2); // previous log is info message.
	}

	pReport->ScrollToLastLine();
	return NULL;
}

static BOOL IsNoSearch(LPCTSTR sPath)
{
	CString sFilePath;
	sFilePath.Format(_T("%s/.TestDrive.nosearch"), sPath);
	return (_taccess(g_pSystem->RetrieveFullPath(sFilePath), 0) != -1);
}

static BOOL IsMakefileExist(LPCTSTR sPath)
{
	CString sFilePath;
	sFilePath.Format(_T("%s/Makefile"), sPath);
	return (_taccess(g_pSystem->RetrieveFullPath(sFilePath), 0) != -1);
}

BOOL CodeAnalysis::StaticCodeAnalysisPrivate(LPCTSTR lpszTitle, LPCTSTR lpszPath)
{
	BOOL bRet = TRUE;

	if (IsNoSearch(lpszPath))
		return TRUE;

	if (IsMakefileExist(lpszPath)) {
		CString sPath(g_pSystem->RetrieveFullPath(lpszPath));
		sPath.Replace(_T('\\'), _T('/'));
		m_pReport->SetColor(RGB(0, 0, 0));
		{
			m_pReport->AppendText(_L(ANALYSIS_TARGET));
			{
				// set path link to title
				CString sLink;
				m_pReport->AppendText(_T("%s ("), lpszTitle);
				sLink.Format(_T("open:%s"), (LPCTSTR)sPath);
				sLink.Replace(_T('\\'), _T('/'));
				m_pReport->SetLink(sLink);
				m_pReport->AppendText(lpszPath);
				m_pReport->AppendText(_T(")\n"));
			}
		}
		{
			// Do static analysis
			g_sCurrentDir = sPath;

			if (g_pSystem->ExecuteFile(
					_T("make"), _T("static"), TRUE, Log_StaticAnalysis, (LPCTSTR)sPath, __sCppcheckTokenList[CPPCHECK_TOKEN_ERROR], -1,
					__sCppcheckTokenList[CPPCHECK_TOKEN_WARNING], -2, __sCppcheckTokenList[CPPCHECK_TOKEN_INFORMATION], 2, NULL) >= 0) {
				m_pReport->SetColor(RGB(0, 0, 255));
				m_pReport->AppendText(_L(NO_ERROR_FOUND));
			} else
				bRet = FALSE;

			m_pReport->AppendText(_T("\n"));
			Log_StaticAnalysis(NULL, 0);
			m_pReport->ScrollToLastLine();
		}
	}

	{
		// searching sub directories
		WIN32_FIND_DATA FindFileData;
		HANDLE			hFind;
		CString			sPath;
		sPath.Format(_T("%s\\*"), lpszPath);

		if ((hFind = FindFirstFile(sPath, &FindFileData)) != INVALID_HANDLE_VALUE) {
			BOOL bSearch = TRUE;

			while (bSearch) {
				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
					sPath.Format(_T("%s/%s"), lpszPath, FindFileData.cFileName);

					if (!StaticCodeAnalysisPrivate(lpszTitle, sPath))
						bRet = FALSE;
				}

				bSearch = FindNextFile(hFind, &FindFileData);
			}

			FindClose(hFind);
		}
	}

	return bRet;
}

BOOL CodeAnalysis::StaticCodeAnalysis(LPCTSTR lpszTitle, LPCTSTR lpszPath, LPCTSTR lpszArg)
{
	// BOOL bRet		= TRUE;
	CString sPath(g_pSystem->RetrieveFullPath(lpszPath));
	__sWorkPath = sPath;
	__sWorkPath.Replace(_T('\\'), _T('/'));
	{
		// error suppress
		LPCTSTR sDelim = _T(" ,;");
		int		iPos   = 0;
		CString sSuppressToken(m_sErrorSuppress);
		CString sArgs(lpszArg);
		sArgs.Insert(0, _T("--platform=win64 ")); // for cppcheck v2.13

		while (iPos >= 0) {
			CString sToken = sSuppressToken.Tokenize(sDelim, iPos);

			if (iPos >= 0) {
				if (sArgs.GetLength())
					sArgs.Append(_T(" "));

				sArgs.AppendFormat(_T("--suppress=%s"), (LPCTSTR)sToken);
			}
		}

		SetEnvironmentVariable(_T("CPPCHECK_ARG"), (LPCTSTR)sArgs);
	}
	return StaticCodeAnalysisPrivate(lpszTitle, sPath);
}

void CodeAnalysis::DoStaticCodeAnalysis(void)
{
	BOOL bRet = TRUE;
	m_pDoc->Lock();
	m_pReport->Clear();
	m_pReport->SetFont(_T("Cascadia Mono"));
	m_pReport->SetColor(RGB(0, 0, 0));
	m_pReport->SetText(_L(STATIC_CODE_ANALYSIS));
	g_pSystem->LogInfo(_L(STATIC_CODE_ANALYSIS_PLEASE_WAIT));
	m_pDoc->SetForegroundDocument();
	{
		CString sConfigPath = g_pSystem->RetrieveFullPath(_T("%PROJECT%Profiles/CodeAnalysis.ini"));
		LPCTSTR sApp		= _T("CodeAnalysis");
		CString sKey;
		CString sName, sPath, sArg;

		for (int i = 0;; i++) {
			sKey.Format(_T("NAME_%d"), i);
			sName = g_ProfileConfig.GetString(sKey); // description name
			sKey.Format(_T("PATH_%d"), i);
			sPath = g_ProfileConfig.GetString(sKey); // target path
			sKey.Format(_T("ARG_%d"), i);
			sArg = g_ProfileConfig.GetString(sKey); // additional argument

			if (sName.IsEmpty())
				break;

			if (!StaticCodeAnalysis(sName, sPath, sArg))
				bRet = FALSE;
		}
	}
	m_pReport->SetColor(RGB(0, 0, 0));
	m_pReport->AppendText(_L(CODE_ANALYSIS_COMPLETE));

	if (g_dwErrorCount || g_dwWarningCount) {
		m_pReport->SetColor(RGB(255, 0, 0));
		m_pReport->AppendText(_L(CODE_ANALYSIS_FAILED), g_dwErrorCount, g_dwWarningCount);
	} else {
		m_pReport->SetColor(RGB(0, 0, 255));
		m_pReport->AppendText(_L(CODE_ANALYSIS_PASSED));
	}

	m_pReport->SetColor(RGB(0, 0, 0));
	m_pReport->AppendText(_L(CODE_ANALYSIS_INFO), g_dwErrorCount);
	m_pReport->ScrollToLastLine();
	g_pSystem->LogInfo(_L(STATIC_CODE_ANALYSIS_COMPLETE));
	m_pDoc->UnLock();
}

void CodeAnalysis::Restart(void)
{
	ShellExecute(NULL, NULL, m_sDocPath + _T("\\restart.bat"), NULL, g_pSystem->GetProjectPath(), SW_SHOW);
	g_pSystem->TryQuit();
}
