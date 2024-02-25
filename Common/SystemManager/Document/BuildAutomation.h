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
// Title : System manager
// Rev.  : 2/26/2024 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __BUILD_AUTOMATION_H__
#define __BUILD_AUTOMATION_H__
#include "BuildAutomationList.h"
#include "TestDrive.h"

class BuildAutomation
{
public:
	BuildAutomation(ITDDocument *pDoc, DWORD dwCheckCommandStart, DWORD dwBuildCommandStart);
	~BuildAutomation(void);

	inline BOOL &UseDebug(void)
	{
		return m_bDebug;
	}
	inline BOOL &AutoBuild(void)
	{
		return m_bAutoBuild;
	}
	inline CString &CodingStyle(void)
	{
		return m_sCodingStyle;
	}
	inline CString &iStyle(void)
	{
		return m_siStyle;
	}
	inline BOOL &StrictBuild(void)
	{
		return m_bStrictBuild;
	}
	inline CString &AuthorName(void)
	{
		return m_sAuthorName;
	}
	inline void ForceToBuild(DWORD dwIndex)
	{
		DoBuild(dwIndex + m_dwBuildCommandStart);
	}
	inline void SetDirtySystem(void)
	{
		m_bDirtySystem = TRUE;
	}

	BOOL OnCommand(DWORD command, WPARAM wParam, LPARAM lParam);

private:
	void DoCheck(DWORD command, LPCTSTR sFileName);
	void DoBuild(DWORD command);
	BOOL CheckBuild(LPCTSTR sFilePath, BuildAutomationItem *pItem);
	BOOL CppCheck(LPCTSTR lpszPath);

private:
	ITDDocument		   *m_pDoc;
	DWORD				m_dwCheckCommandStart;
	DWORD				m_dwBuildCommandStart;
	BOOL				m_bAutoBuild;
	BOOL				m_bDebug;
	BOOL				m_bStrictBuild;
	BOOL				m_bDirtySystem;
	CString				m_sCodingStyle;
	CString				m_siStyle;
	CString				m_sAuthorName;
	CString				m_sTempFilePath;

	BuildAutomationList m_BuildList;
};
#endif //__BUILD_AUTOMATION_H__
