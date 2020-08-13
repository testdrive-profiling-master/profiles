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
// Rev.  : 8/13/2020 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SCENARIO_TEST_H__
#define __SCENARIO_TEST_H__
#include "common.h"
#include "TestList.h"
#include "HtmlTable.h"
#include <list>
using namespace std;

typedef enum{
	PROPERTY_ID_NAME_FILTER,
	PROPERTY_ID_SIZE
}PROPERTY_ID;

typedef enum{
	COMMAND_ID_DO_TEST,
	COMMAND_ID_REFRESH_TABLE,
	COMMAND_ID_SIZE
}COMMAND_ID;

class ScenarioTest :
	public TDImplDocumentBase,
	public ITDHtmlManager,
	public TestResource
{
public:
	ScenarioTest(ITDDocument* pDoc);
	virtual ~ScenarioTest(void);

	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(void, OnSize)(int width, int height);
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);

	void EnableButton(BOOL bEnable = TRUE);
	void SetLock(BOOL bLock = TRUE);

	STDMETHOD_(LPCTSTR, OnHtmlBeforeNavigate)(DWORD dwID, LPCTSTR lpszURL);
	STDMETHOD_(void, OnHtmlDocumentComplete)(DWORD dwID, LPCTSTR lpszURL);

protected:
	void ClearTable(void);
	void NewRow(void);
	void NewTH(void);
	void NewTD(void);
	void NewElement(LPCTSTR sElement);
	void AppendCell(void);
	void AppendRow(void);
	void SetBody(LPCTSTR sBody);

	void Execute(LPCTSTR sScript, BOOL bShell = FALSE);
	void StartTest(LPCTSTR sPath = NULL, BOOL bGroup = TRUE);
	BOOL OnTest(void);
	void ForceToQuit(LPCTSTR sFileName);

private:
	ITDDocument*		m_pDoc;
	HtmlTable			m_HtmlTable;
	TestList			m_TestList;
	CString				m_sScenarioPath;
	BOOL				m_bInitialized;
	list<TestVector*>	m_Scenario;
	time_t				m_TimeToday;			// check for day change
};
#endif//__SCENARIO_TEST_H__
