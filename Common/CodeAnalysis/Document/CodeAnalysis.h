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
// Title : Code Analysis
// Rev.  : 3/4/2020 Wed (clonextop@gmail.com)
//================================================================================
#pragma once
#include "testdrive_document.h"
#include "Locale.h"

class CodeAnalysis :
	public TDImplDocumentBase,
	public ITDReportManager
{
public:
	CodeAnalysis(ITDDocument* pDoc);
	virtual ~CodeAnalysis(void);

	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(void, OnSize)(int width, int height);
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);

	void DoStaticCodeAnalysis(void);
	BOOL StaticCodeAnalysis(LPCTSTR lpszTitle, LPCTSTR lpszPath);
	void Restart(void);

	STDMETHOD_(void, OnReportLink)(DWORD dwID, LPCTSTR lpszString, long cpMin, long cpMax);

	static inline CodeAnalysis* GetCurrent(void)	{return m_pCurrent;}
	inline ITDReport* Report(void)					{return m_pReport;}
	inline ITDDocument* Document(void)				{return m_pDoc;}

protected:
	static CodeAnalysis*	m_pCurrent;
	ITDDocument*			m_pDoc;
	ITDReport*				m_pReport;
	CString					m_sDocPath;
	CString					m_sErrorSuppress;
};
