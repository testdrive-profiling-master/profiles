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
// Title : Scenario test
// Rev.  : 8/14/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "ScenarioTest.h"

static CString __sScenarioPath;
static CString __sProgramPath;

REGISTER_LOCALED_DOCUMENT_EX(ScenarioTest)
{
	CString sName(pDoc->DocumentTitle());

	if (!sName.Compare(_T("Scenario Test"))) {
		// default scenario path
		pDoc->DocumentTitle(_L(DOCUMENT_TITLE));
		__sScenarioPath = _T("%PROJECT%Program\\ScenarioTest");
		__sProgramPath	= _T("%PROJECT%Program");
		return TRUE;
	} else {
		// user defined scenario path
		int		iPos	= 0;
		LPCTSTR sDelim	= _T(":");
		CString sTitle	= sName.Tokenize(sDelim, iPos);
		__sScenarioPath = (iPos > 0) ? sName.Tokenize(sDelim, iPos) : _T("%PROJECT%Program\\ScenarioTest");
		__sProgramPath	= (iPos > 0) ? sName.Tokenize(sDelim, iPos) : _T("%PROJECT%Program");
		sName.Format(_T("%s(%s)"), _L(DOCUMENT_TITLE), (LPCTSTR)sTitle);
		pDoc->DocumentTitle(sName);
		return TRUE;
	}

	return TRUE;
}

LPCTSTR g_sTestStatus[TEST_STATUS_SIZE];

ScenarioTest::ScenarioTest(ITDDocument *pDoc)
{
	m_bInitialized = FALSE;
	m_pDoc		   = pDoc;
	m_HtmlTable.Create(pDoc, _T("../media/tables.html"), 0, 0, 100, 100);
	m_HtmlTable.SetManager(this);
	m_pHtmlTable = &m_HtmlTable;
	{
		// initialize test status string
		g_sTestStatus[TEST_STATUS_NOT_TESTED]		  = _L(TEST_STATUS_NOT_TESTED);
		g_sTestStatus[TEST_STATUS_RUN_PASSED]		  = _L(TEST_STATUS_RUN_PASSED);
		g_sTestStatus[TEST_STATUS_RUN_FAILED]		  = _L(TEST_STATUS_RUN_FAILED);
		g_sTestStatus[TEST_STATUS_RUN_CRACHED]		  = _L(TEST_STATUS_RUN_CRACHED);
		g_sTestStatus[TEST_STATUS_COMPILE_FAILED]	  = _L(TEST_STATUS_COMPILE_FAILED);
		g_sTestStatus[TEST_STATUS_LINK_FAILED]		  = _L(TEST_STATUS_LINK_FAILED);
		g_sTestStatus[TEST_STATUS_FILE_NOT_FOUND]	  = _L(TEST_STATUS_FILE_NOT_FOUND);
		g_sTestStatus[TEST_STATUS_SYSTEM_IS_REQUIRED] = _L(TEST_STATUS_SYSTEM_IS_REQUIRED);
		g_sTestStatus[TEST_STATUS_SCORE]			  = _L(TEST_STATUS_SCORE);
	}
	{
		ITDPropertyData *pProperty;
		pProperty = pDoc->AddPropertyData(
			PROPERTY_TYPE_STRING, PROPERTY_ID_NAME_FILTER, _L(NAME_FILTER), (DWORD_PTR)((LPCTSTR)m_TestList.TestFilter()), _L(DESC_NAME_FILTER));
		pProperty->UpdateConfigFile();

		pProperty = pDoc->AddPropertyData(
			PROPERTY_TYPE_BOOL, PROPERTY_ID_SUPPRESS_WAVEFORM_OUT, _L(SUPPRESS_WAVEFORM_OUT), (DWORD_PTR)(&m_bSuppressWaveform),
			_L(DESC_SUPPRESS_WAVEFORM_OUT));
		pProperty->UpdateConfigFile();
	}

	{
		m_TimeToday = GetCurrentDayTime();
		m_pDoc->SetTimer(COMMAND_ID_REFRESH_TABLE, 1000 * 60 * 60); // every 1 hour, refresh table when day changed
	}
}

ScenarioTest::~ScenarioTest(void)
{
	m_pDoc->KillTimer(COMMAND_ID_REFRESH_TABLE);
}

BOOL ScenarioTest::OnPropertyUpdate(ITDPropertyData *pProperty)
{
	pProperty->UpdateData();
	pProperty->UpdateConfigFile(FALSE);

	switch (pProperty->GetID()) {
	case PROPERTY_ID_NAME_FILTER:
		if (m_pDoc->IsLocked()) {
			g_pSystem->LogWarning(_L(ALREADY_TEST_IS_RUNNING));
		} else {
			m_TestList.Clear();
			m_TestList.Refresh();
			g_pSystem->LogInfo(_L(LIST_REFRESH_IS_DONE));
		}

		break;

	default:
		break;
	}

	return TRUE;
}

void ScenarioTest::OnSize(int width, int height)
{
	if (m_HtmlTable.Control()) {
		ITDLayout *pLayout;
		pLayout = m_HtmlTable.Control()->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
	}
}

BOOL ScenarioTest::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch (command) {
	case COMMAND_ID_DO_TEST:
		m_pDoc->KillTimer(command);

		if (OnTest())
			m_pDoc->SetTimer(command, 0);

		break;

	case COMMAND_ID_REFRESH_TABLE:
		if (!m_pDoc->IsLocked() && (m_TimeToday != GetCurrentDayTime())) {
			m_TestList.Refresh();
			m_TimeToday = GetCurrentDayTime();
		}

		break;

	default:
		break;
	}

	return FALSE;
}

LPCTSTR ScenarioTest::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if (m_bInitialized) {
		if (_tcsstr(lpszURL, _T("group:")) == lpszURL) {
			int iGroupID;
			_stscanf(&lpszURL[6], _T("%d"), &iGroupID);
			StartTest(&lpszURL[6]);
		} else if (_tcsstr(lpszURL, _T("test:")) == lpszURL) {
			StartTest(&lpszURL[5], FALSE);
		} else if (_tcsstr(lpszURL, _T("folder:")) == lpszURL) {
			TestGroup *pGroup = m_TestList.FindGroup(&lpszURL[7]);

			if (pGroup)
				pGroup->OpenFolder();
		} else if (_tcsstr(lpszURL, _T("open:")) == lpszURL) {
			Execute(&lpszURL[5], TRUE);
		} else if (_tcsstr(lpszURL, _T("execute:")) == lpszURL) {
			Execute(&lpszURL[8]);
		} else if (_tcsstr(lpszURL, _T("quit:")) == lpszURL) {
			g_pSystem->LogWarning(_L(FORCE_TO_QUIT_PROGRAM));
			TestVector *pTestVector = TestVector::CurrentTestVecotr();

			if (pTestVector)
				ForceToQuit(pTestVector->Group()->GetConfig(TG_DESC_PROGRAM));
		} else if (_tcsstr(lpszURL, _T("refresh:")) == lpszURL) {
			if (m_pDoc->IsLocked()) {
				g_pSystem->LogWarning(_L(ALREADY_TEST_IS_RUNNING));
			} else {
				_tremove(m_sLogPath);
				m_TestList.Refresh();
				g_pSystem->LogInfo(_L(LIST_REFRESH_IS_DONE));
			}
		} else if (_tcsstr(lpszURL, _T("testall:")) == lpszURL) {
			if (m_pDoc->IsLocked()) {
				g_pSystem->LogWarning(_L(ALREADY_TEST_IS_RUNNING));
			} else
				StartTest();
		}

		return NULL;
	}

	return lpszURL;
}

void ScenarioTest::SetLock(BOOL bLock)
{
	if (bLock) {
		m_HtmlTable.JScript(
			_T("SetTitle(\"<table  class='null0'><tbody><td><button class='ShadowButton'>%s</button></td><td ")
			_T("style='text-align:right'></td></tbody></table>\");"),
			_L(TEST_IN_PROGRESS));
	} else {
		m_HtmlTable.JScript(
			_T("SetTitle(\"<table  class='null0'><tbody><td><a href='testall:'><button class='ShadowButton'>%s</button></a></td><td ")
			_T("style='text-align:right'><a href='refresh:'><img class='MiniButton' src='refresh.png' title='%s'></a></td></tbody></table>\");"),
			_L(TEST_ALL), _L(RESCAN_ALL));
	}

	if (m_bInitialized) {
		if (bLock) {
			m_pDoc->Lock();
		} else {
			m_pDoc->UnLock();
		}
	}
}

void ScenarioTest::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if (!m_bInitialized) {
		m_TestList.Initialize(__sScenarioPath, __sProgramPath);
		SetLock(FALSE);
		m_bInitialized = TRUE;
	}
}

void ScenarioTest::Execute(LPCTSTR sScript, BOOL bShell)
{
	CString Script(sScript);
	Script.Replace(_T("/"), _T("\\"));
	Script.Replace(_T("%20"), _T(" "));
	{
		int		iPos   = 0;
		LPCTSTR sDelim = _T("?");
		CString sExecuteFile, sArg, sRunPath;
		sExecuteFile = Script.Tokenize(sDelim, iPos);

		if (iPos > 0)
			sArg = Script.Tokenize(sDelim, iPos);

		if (iPos > 0)
			sRunPath = Script.Tokenize(sDelim, iPos);

		if (sRunPath.IsEmpty())
			sRunPath = _T("%PROJECT%Program");

		sRunPath	 = g_pSystem->RetrieveFullPath(sRunPath);
		sExecuteFile = g_pSystem->RetrieveFullPath(sExecuteFile);

		if (bShell)
			ShellExecute(NULL, NULL, sExecuteFile, sArg, NULL, SW_SHOW);
		else
			g_pSystem->ExecuteFile(sExecuteFile, sArg, TRUE, NULL, sRunPath, _T("*E:"), -1, _T("*I:"), 1, NULL);
	}
}

void ScenarioTest::StartTest(LPCTSTR sPath, BOOL bGroup)
{
	if (!sPath) {
		// add all
		TestGroup *pGroup = m_TestList.GetNextGroup();

		while (pGroup) {
			TestVector *pVector = pGroup->GetNextVector();

			while (pVector) {
				m_Scenario.push_back(pVector);
				pVector = pGroup->GetNextVector(pVector);
			}

			pGroup = m_TestList.GetNextGroup(pGroup);
		}
	} else if (bGroup) {
		// add group
		TestGroup  *pGroup	= m_TestList.FindGroup(sPath);
		TestVector *pVector = NULL;

		if (pGroup)
			pVector = pGroup->GetNextVector();

		while (pVector) {
			m_Scenario.push_back(pVector);
			pVector = pGroup->GetNextVector(pVector);
		}
	} else {
		// add vector
		TestVector *pVector = m_TestList.FindVector(sPath);

		if (pVector)
			m_Scenario.push_back(pVector);
	}

	if (m_Scenario.size() > 1) {
		SetEnvironmentVariable(_T("SIM_WAVE_MODE"), _T("None"));
	}

	{
		CString sSize;
		sSize.Format(_T("%d"), m_Scenario.size());
		SetEnvironmentVariable(_T("GROUP_TEST_SIZE"), (LPCTSTR)sSize);
	}

	if (!m_pDoc->IsLocked()) {
		m_pDoc->SetTimer(COMMAND_ID_DO_TEST, 0);
	} else {
		g_pSystem->LogInfo(_L(ADD_TEST_LIST), sPath);
	}
}

BOOL ScenarioTest::OnTest(void)
{
	TestVector *pVector = m_Scenario.front();

	if (pVector) {
		SetLock();
		m_Scenario.pop_front();
		pVector->DoTest();
		m_TestList.UpdateTable();
		SetLock(FALSE);
	}

	if (!m_Scenario.size()) {
		ITDDocument *pDoc = g_pSystem->GetDocument(_T("System"));

		if (pDoc)
			pDoc->GetImplementation()->OnCommand(TD_EXTERNAL_COMMAND - 1);

		g_pSystem->LogInfo(_L(TEST_IS_OVER));
		return FALSE;
	}

	return TRUE;
}

#include <TlHelp32.h>
void ScenarioTest::ForceToQuit(LPCTSTR sFileName)
{
	HANDLE		   hSnapShot;
	PROCESSENTRY32 pEntry;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	if (hSnapShot == INVALID_HANDLE_VALUE) {
		return;
	}

	pEntry.dwSize = sizeof(pEntry);

	if (Process32First(hSnapShot, &pEntry)) {
		do {
			if (!_tcscmp(pEntry.szExeFile, sFileName)) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);

				if (hProcess) {
					if (TerminateProcess(hProcess, 0)) {
						unsigned long nCode;
						GetExitCodeProcess(hProcess, &nCode);
					}

					CloseHandle(hProcess);
				}

				break;
			}
		} while (Process32Next(hSnapShot, &pEntry));
	}
}
