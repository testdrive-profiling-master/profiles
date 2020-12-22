//================================================================================
// Copyright (c) 2013 ~ 2020. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 12/22/2020 Tue (clonextop@gmail.com)
//================================================================================
#include "TestGroup.h"
#include "TestList.h"

TestGroup::TestGroup(void)
{
	memset(&m_Analysis, 0, sizeof(m_Analysis));
	m_iGroupID	= 0;
}

TestGroup::~TestGroup(void)
{
	Clear();
}

void TestGroup::Clear(void)
{
	for(vector<TestVector*>::iterator i = m_List.begin(); i != m_List.end(); i++) {
		TestVector*	pGroup	= *i;
		delete pGroup;
	}

	m_List.clear();
}

static LPCTSTR	__sScenarioID[TG_DESC_SIZE] = {
	_T("NAME"),
	_T("PROGRAM"),
	_T("PARAMETERS"),
	_T("FILES"),
	_T("ERROR_STRING"),
	_T("PREFIX"),
	_T("POSTFIX"),
	_T("SCORE_FORMAT"),
};

#define GetConfig(id, str)	GetPrivateProfileString(g_sAppName, __sScenarioID[id], g_sEmpty, str, 4096, sScenarioFileName)

BOOL TestGroup::Initialize(int iGroupID, LPCTSTR sPath, LPCTSTR sNameFilter)
{
	m_iGroupID				= iGroupID;
	m_sPath					= sPath;
	m_sName					= sPath;
	m_ScoreFormat.sFormat	= _L(TEST_STATUS_SCORE);
	m_ScoreFormat.min		= 0;
	m_ScoreFormat.max		= 100;
	{
		int iPos	= m_sName.ReverseFind(_T('\\'));

		if(iPos > 0) m_sName.Delete(0, iPos + 1);
	}
	{
		// read description
		CString	sScenarioFileName;
		TCHAR sDesc[4096];
		sScenarioFileName.Format(_T("%s\\Scenario.ini"), sPath);

		for(int i = 0; i < TG_DESC_SIZE; i++) {
			GetConfig((TG_DESC)i, sDesc);
			m_sDesc[i]	= sDesc;
		}

		if(sNameFilter && *sNameFilter && !m_sDesc[TG_DESC_NAME].IsEmpty()) {
			if(m_sDesc[TG_DESC_NAME].Find(sNameFilter) < 0)
				return FALSE;
		}

		if(m_sDesc[TG_DESC_FILES].IsEmpty()) return FALSE;

		if(!m_sDesc[TG_DESC_SCORE_FORAMT].IsEmpty()) {
			LPCTSTR	sDelim	= _T(";");
			int		pos	= 0;
			{
				CString	sFormat	= m_sDesc[TG_DESC_SCORE_FORAMT].Tokenize(sDelim, pos);
				m_ScoreFormat.sFormat	= _T("<font color='#%06X'>");
				m_ScoreFormat.sFormat	+= sFormat;
				m_ScoreFormat.sFormat	+= _T("</font>");
			}

			if(pos > 0) {
				CString	sValue	= m_sDesc[TG_DESC_SCORE_FORAMT].Tokenize(sDelim, pos);
				_stscanf(sValue, _T("%lf"), &m_ScoreFormat.min);
			}

			if(pos > 0) {
				CString	sValue	= m_sDesc[TG_DESC_SCORE_FORAMT].Tokenize(sDelim, pos);
				_stscanf(sValue, _T("%lf"), &m_ScoreFormat.max);
			}

			if(m_ScoreFormat.min == m_ScoreFormat.max) {
				g_pSystem->LogError(_T("%s : Score's min/max value must not be same!"), (LPCTSTR)sScenarioFileName);
				return FALSE;
			}
		}
	}
	{
		// Add table
		CString	sTag;
		m_pHtmlTable->NewRow();
		m_pHtmlTable->NewCell(CELL_TH);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_LEFT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->Control()->CallJScript(_T("SetTBody(\"<a href='folder:%s'><img src='document.png' title='%s' border='0'/></a> <a href='group:%s'>%d. %s</a>\");"),
											 (LPCTSTR)Name(),
											 _L(OPEN_FOLDER),
											 (LPCTSTR)Name(),
											 iGroupID + 1,
											 (LPCTSTR)m_sDesc[TG_DESC_NAME]);
		m_pHtmlTable->NewCell(CELL_TH, _T("total_%d"), iGroupID);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("passed_%d"), iGroupID);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("failed_%d"), iGroupID);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("crashed_%d"), iGroupID);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("notest_%d"), iGroupID);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
	}
	// scan test vectors
	Scan(sPath);	// scan current folder
	{
		// scan sub folder
		HANDLE				hFind;
		WIN32_FIND_DATA		FindFileData;
		{
			// scan sub vectors
			CString	sFindPath;
			sFindPath.Format(_T("%s\\*.*"), sPath);
			hFind = FindFirstFile(sFindPath, &FindFileData);
		}

		if(hFind != INVALID_HANDLE_VALUE) {
			do {
				if(FindFileData.cFileName[0] == _T('.')) {
				} else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					CString	sNextPath;
					sNextPath.Format(_T("%s\\%s"), sPath, FindFileData.cFileName);
					Scan(sNextPath);
				}
			} while(FindNextFile(hFind, &FindFileData));

			FindClose(hFind);
		}
	}
	UpdateTable();
	return TRUE;
}

void TestGroup::Scan(LPCTSTR sPath)
{
	// Find Vectors
	HANDLE				hFind;
	WIN32_FIND_DATA		FindFileData;
	{
		CString	sWildCards(m_sDesc[TG_DESC_FILES]);
		int iPos = 0;
		CString sTok;

		for(;;) {
			sTok	= sWildCards.Tokenize(_T(";"), iPos);

			if(sTok.IsEmpty()) break;

			{
				// Find
				CString	sFindPath;
				sFindPath.Format(_T("%s\\%s"), sPath, (LPCTSTR)sTok);
				hFind = FindFirstFile(sFindPath, &FindFileData);
			}

			if(hFind != INVALID_HANDLE_VALUE) {
				CString	sRelPath(sPath);
				sRelPath.Delete(0, m_sScenarioPath.length());

				do {
					if(FindFileData.cFileName[0] == _T('.')) {
					} else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						CString	sNextPath;
						sNextPath.Format(_T("%s\\%s"), sPath, FindFileData.cFileName);
						Scan(sNextPath);
					} else {
						CString sCurFileName(FindFileData.cFileName);

						if(sCurFileName.Find(_T('~')) == -1 && sCurFileName.Find(_T('$')) == -1) {	// except with temporary files (~$*.docx...)
							TestVector* pTest	= new TestVector(this, m_List.size(), sRelPath, FindFileData.cFileName);
							m_Analysis.total++;
							m_List.push_back(pTest);
						}
					}
				} while(FindNextFile(hFind, &FindFileData));

				FindClose(hFind);
			}
		}
	}
}

void TestGroup::Analysis(TEST_ANALYSIS* pParent)
{
	memset(&m_Analysis, 0, sizeof(m_Analysis));

	for(vector<TestVector*>::iterator i = m_List.begin(); i != m_List.end(); i++) {
		m_Analysis.total++;
		m_Analysis.fScoreSum	+= (*i)->Score();

		switch((*i)->Status()) {
		case TEST_STATUS_SCORE:
		case TEST_STATUS_RUN_PASSED:
			m_Analysis.passed++;
			break;

		case TEST_STATUS_RUN_CRACHED:
			m_Analysis.crashed++;
			break;

		case TEST_STATUS_RUN_FAILED:
		case TEST_STATUS_COMPILE_FAILED:
		case TEST_STATUS_LINK_FAILED:
		case TEST_STATUS_FILE_NOT_FOUND:
			m_Analysis.failed++;
			break;

		case TEST_STATUS_NOT_TESTED:
		default:
			m_Analysis.untested++;
			break;
		}
	}

	if(pParent) {
		for(int i = 0; i < 5; i++) pParent->m[i]	+= m_Analysis.m[i];

		pParent->fScoreSum	+= m_Analysis.fScoreSum;
	}
}

void TestGroup::UpdateTable(void)
{
	Analysis();
	// refresh table
	DWORD	dwTotal	= m_Analysis.total;
	CString	sTag;
	sTag.Format(_T("SetBody('total_%d',\"%s\");"), m_iGroupID, _L(SCORE_TOTAL));
	m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.total);
	sTag.Format(_T("SetBody('passed_%d',\"%s\");"), m_iGroupID, _L(SCORE_PASSED));
	m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.passed, dwTotal ? (float)(m_Analysis.fScoreSum / dwTotal) : 0);
	sTag.Format(_T("SetBody('failed_%d',\"%s\");"), m_iGroupID, _L(SCORE_FAILD));
	m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.failed, dwTotal ? (100.f * m_Analysis.failed / dwTotal) : 0);
	sTag.Format(_T("SetBody('crashed_%d',\"%s\");"), m_iGroupID, _L(SCORE_CRASHED));
	m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.crashed, dwTotal ? (100.f * m_Analysis.crashed / dwTotal) : 0);
	sTag.Format(_T("SetBody('notest_%d',\"%s\");"), m_iGroupID, _L(SCORE_NOT_TESTED));
	m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.untested, dwTotal ? (100.f * m_Analysis.untested / dwTotal) : 0);
}

void TestGroup::UpdateProfile(BOOL bUpdate)
{
	for(vector<TestVector*>::iterator i = m_List.begin(); i != m_List.end(); i++)
		(*i)->UpdateProfile(bUpdate);
}

void TestGroup::OpenFolder(void)
{
	CString sFolderPath;
	sFolderPath	= m_sPath;
	sFolderPath	+= _T('\\');
	ShellExecute(NULL, NULL, sFolderPath, NULL, NULL, SW_SHOW);
}

TestVector* TestGroup::FindVector(LPCTSTR sPath)
{
	TestVector* pVector	= NULL;

	for(vector<TestVector*>::iterator i = m_List.begin(); i != m_List.end(); i++) {
		if(!(*i)->FullName().Compare(sPath)) {
			pVector	= (*i);
			break;
		}
	}

	return pVector;
}

TestVector* TestGroup::GetNextVector(TestVector* pVector)
{
	if(Size()) {
		if(!pVector) return m_List.front();

		for(vector<TestVector*>::iterator i = m_List.begin(); i != m_List.end(); i++) {
			if((*i) == pVector) {
				i++;

				if(i == m_List.end()) return NULL;

				return (*i);
			}
		}
	}

	return NULL;
}
