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
// Title : Scenario test
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __TEST_GROUP_H__
#define __TEST_GROUP_H__
#include "TestVector.h"

typedef enum{
	TG_DESC_NAME,
	TG_DESC_PROGRAM,
	TG_DESC_PARAMETERS,
	TG_DESC_FILES,
	TG_DESC_ERROR_STRING,
	TG_DESC_PREFIX,
	TG_DESC_POSTFIX,
	TG_DESC_SCORE_FORAMT,
	TG_DESC_SIZE
}TG_DESC;

typedef struct{
	CString		sFormat;
	double		min, max;
}TG_SCORE_FORMAT;

class TestGroup : public TestResource
{
public:
	TestGroup(void);
	~TestGroup(void);

	BOOL Initialize(int iGroupID, LPCTSTR sPath);
	void Clear(void);
	void Scan(LPCTSTR sPath);
	void Analysis(TEST_ANALYSIS* pParent = NULL);
	void UpdateTable(void);
	void UpdateProfile(BOOL bUpdate = TRUE);

	inline LPCTSTR GetConfig(TG_DESC id){return m_sDesc[id];}
	inline CString& Path(void)	{return m_sPath;}
	inline CString& Name(void)	{return m_sName;}

	void OpenFolder(void);

	inline int GroupID(void)	{return m_iGroupID;}
	inline size_t Size(void)	{return m_List.size();}

	TestVector* FindVector(LPCTSTR sPath);
	TestVector* GetNextVector(TestVector* pVector = NULL);
	inline TG_SCORE_FORMAT* ScoreFormat(void)	{return &m_ScoreFormat;}

protected:
	int					m_iGroupID;
	CString				m_sName;
	CString				m_sPath;
	CString				m_sDesc[TG_DESC_SIZE];
	TG_SCORE_FORMAT		m_ScoreFormat;
	vector<TestVector*>	m_List;
	TEST_ANALYSIS		m_Analysis;
};
#endif//__TEST_GROUP_H__
