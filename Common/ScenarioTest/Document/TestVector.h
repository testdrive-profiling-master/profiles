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
// Title : Scenario test
// Rev.  : 4/24/2023 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __TEST_VECTOR_H__
#define __TEST_VECTOR_H__
#include "common.h"
#include "HtmlTable.h"
#include "ScenarioTestStatus.h"
#include <vector>

using namespace std;

typedef union {
	DWORD		m[7];
	struct {
		DWORD		total;
		DWORD		passed;
		DWORD		failed;
		DWORD		crashed;
		DWORD		untested;
		double		fScoreSum;
	};
} TEST_ANALYSIS;

extern LPCTSTR	g_sTestStatus[TEST_STATUS_SIZE];
extern LPCTSTR	g_sAppName;
extern LPCTSTR	g_sEmpty;

class TestResource {
public:
	TestResource(void);
	~TestResource(void);
	LPCTSTR ScenarioPath(void)	{
		return m_sScenarioPath;
	}

protected:
	static CString		m_sProgramPath;
	static CString		m_sScenarioPath;
	static CString		m_sLogPath;
	static HtmlTable*	m_pHtmlTable;
	static BOOL			m_bSuppressWaveform;
};

class TestGroup;
class TestVector : public TestResource {
public:
	TestVector(TestGroup* pGroup, int iID, LPCTSTR sRelativePath, LPCTSTR sFileName, LPCTSTR sPathName);
	~TestVector(void);

	void UpdateTable(void);
	void UpdateProfile(BOOL bUpdate = TRUE);
	void DoTest(void);
	CString FullName(void);

	inline LPCTSTR Name(void)			{
		return m_sFileName;
	}
	inline TEST_STATUS Status(void)	{
		return m_Status;
	}
	TestGroup* Group(void)				{
		return m_pGroup;
	}
	double Score(void);

	static TestVector* CurrentTestVecotr(void)	{
		return m_pCurrentTestVector;
	}

protected:
	void ExtensionString(CString& sStr);
	CString GetConfigString(LPCTSTR sKey, LPCTSTR sDefault = NULL);

	TestGroup*			m_pGroup;
	int					m_iID;
	TEST_STATUS			m_Status;
	time_t				m_LastTime;
	double				m_fScore;

	CString				m_sRelativePath, m_sFileName;

private:
	static TestVector*	m_pCurrentTestVector;
};
#endif//__TEST_VECTOR_H__
