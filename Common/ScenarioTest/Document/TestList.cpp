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
#include "TestList.h"

TestList::TestList(void)
{
	memset(&m_Analysis, 0, sizeof(m_Analysis));
	m_sTestFilter.GetBuffer(MAX_PATH);
}

TestList::~TestList(void)
{
	Clear();
}

void TestList::Initialize(LPCTSTR sScenarioPath, LPCTSTR sProgramPath)
{
	if (sScenarioPath) {
		m_sScenarioPath = g_pSystem->RetrieveFullPath(sScenarioPath);
		m_sLogPath.Format(_T("%s\\ScenarioTest.report"), (LPCTSTR)m_sScenarioPath);
	}

	if (sProgramPath) {
		m_sProgramPath = g_pSystem->RetrieveFullPath(sProgramPath);
	}

	memset(&m_Analysis, 0, sizeof(m_Analysis));
	m_pHtmlTable->Clear();
	Clear();
	Scan(m_sScenarioPath);
	{
		// add table
		m_pHtmlTable->NewRow();
		m_pHtmlTable->NewCell(CELL_TH);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_LEFT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->SetBody(_L(SCORE_RESULT));
		m_pHtmlTable->Control()->CallJScript(_T("SetTBody(\"<img src='chart.png' border='0'/> %s\");"), _L(SCORE_RESULT));
		m_pHtmlTable->NewCell(CELL_TH, _T("total"));
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("passed"));
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("failed"));
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("crashed"));
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->NewCell(CELL_TH, _T("notest"));
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
	}
	UpdateTable();
}

void TestList::Clear(void)
{
	for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end(); i++) {
		TestGroup *pGroup = *i;
		delete pGroup;
	}

	m_List.clear();
}

void TestList::Refresh(void)
{
	for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end(); i++) (*i)->UpdateProfile(FALSE);

	Initialize();
}

void TestList::Scan(LPCTSTR sPath)
{
	HANDLE			hFind;
	WIN32_FIND_DATA FindFileData;
	CString			sFindPath;
	DWORD			dwGroupID = 0;
	Clear();
	m_pHtmlTable->Clear();
	// Find sub folder
	sFindPath.Format(_T("%s\\*.*"), sPath);
	hFind = FindFirstFile(sFindPath, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.cFileName[0] == _T('.')) {
			} else if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				CString sGroupPath;
				sGroupPath.Format(_T("%s\\%s"), sPath, FindFileData.cFileName);
				{
					// check "no-search" tag
					CString sNoSearch;
					sNoSearch.Format(_T("%s\\.TestDrive.nosearch"), (LPCTSTR)sGroupPath);

					if (_taccess(sNoSearch, 0) != -1)
						continue;
				}
				{
					TestGroup *pTestGroup = new TestGroup;

					if (pTestGroup->Initialize(dwGroupID, sGroupPath, m_sTestFilter)) {
						m_List.push_back(pTestGroup);
						dwGroupID++;
					} else {
						delete pTestGroup;
					}
				}
			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}
}

void TestList::Analysis(void)
{
	memset(&m_Analysis, 0, sizeof(m_Analysis));

	for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end(); i++) {
		(*i)->Analysis(&m_Analysis);
	}
}

void TestList::UpdateTable(BOOL bSubGroups)
{
	Analysis();

	if (bSubGroups) {
		for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end(); i++) (*i)->UpdateTable();
	}

	{
		// refresh table
		DWORD	dwTotal = m_Analysis.total;
		CString sTag;
		sTag.Format(_T("SetBody('total',\"%s\");"), _L(SCORE_TOTAL));
		m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.total);
		sTag.Format(_T("SetBody('passed',\"%s\");"), _L(SCORE_PASSED));
		m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.passed, dwTotal ? (float)(m_Analysis.fScoreSum / dwTotal) : 0);
		sTag.Format(_T("SetBody('failed',\"%s\");"), _L(SCORE_FAILD));
		m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.failed, dwTotal ? (100.f * m_Analysis.failed / dwTotal) : 0);
		sTag.Format(_T("SetBody('crashed',\"%s\");"), _L(SCORE_CRASHED));
		m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.crashed, dwTotal ? (100.f * m_Analysis.crashed / dwTotal) : 0);
		sTag.Format(_T("SetBody('notest',\"%s\");"), _L(SCORE_NOT_TESTED));
		m_pHtmlTable->Control()->CallJScript(sTag, m_Analysis.untested, dwTotal ? (100.f * m_Analysis.untested / dwTotal) : 0);
	}
}

TestGroup *TestList::FindGroup(LPCTSTR sPath)
{
	for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end(); i++) {
		if (!(*i)->Name().Compare(sPath)) {
			return (*i);
		}
	}

	return NULL;
}

TestVector *TestList::FindVector(LPCTSTR sPath)
{
	TestVector *pVector = NULL;

	for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end() && !pVector; i++) {
		pVector = (*i)->FindVector(sPath);
	}

	return pVector;
}

TestGroup *TestList::GetNextGroup(TestGroup *pGroup)
{
	if (Size()) {
		if (!pGroup)
			return m_List.front();

		for (vector<TestGroup *>::iterator i = m_List.begin(); i < m_List.end(); i++) {
			if ((*i) == pGroup) {
				i++;

				if (i == m_List.end())
					return NULL;

				return (*i);
			}
		}
	}

	return NULL;
}
