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
// Title : Xilinx synthesis
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __XILINX_SYNTHESIS_H__
#define __XILINX_SYNTHESIS_H__
#include "common.h"
#include "XilinxDevices.h"
#include "ProjectConfig.h"
#include "SourceList.h"

typedef enum{
	PROPERTY_ID_INSTALL_PATH,
	PROPERTY_ID_PROJECT_PATH,
	PROPERTY_ID_FAMILY,
	PROPERTY_ID_DEVICE,
	PROPERTY_ID_PACKAGE,
	PROPERTY_ID_SPEED_GRADE,
	PROPERTY_ID_TARGET_SPEED,
	PROPERTY_ID_CLOCKS,
	PROPERTY_ID_RESETS,
	PROPERTY_ID_MAX_PATHS,
	PROPERTY_ID_SIZE
}PROPERTY_ID;

typedef enum{
	BTN_ID_REFRESH_TABLE,
}BTN_ID;

typedef enum{
	COMMAND_ID_SYNTHESIS
}COMMAND_ID;

class XilinxSynthesis :
	public TDImplDocumentBase,
	public ITDHtmlManager,
	public ITDButtonManager,
	public ProjectConfig
{
public:
	XilinxSynthesis(ITDDocument* pDoc);
	virtual ~XilinxSynthesis(void);

	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(void, OnSize)(int width, int height);
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);

	// html
	STDMETHOD_(LPCTSTR, OnHtmlBeforeNavigate)(DWORD dwID, LPCTSTR lpszURL);
	STDMETHOD_(void, OnHtmlDocumentComplete)(DWORD dwID, LPCTSTR lpszURL);

	// button
	STDMETHOD_(void, OnButtonClick)(DWORD dwID);

	inline ITDDocument* Document(void)		{return m_pDoc;}

	void UpdateProperty(PROPERTY_ID id, BOOL bClear = FALSE);
	void RefreshTable(void);
	void ResetProject(void);
	void XilinxToolCheck(void);
	void CheckProjectEmpty(void);
	void XilinxReport(LPCTSTR sPath);

protected:
	void StartSynthesis(LPCTSTR sPath, BOOL bGroup);
	BOOL OnSynthesis(void);

private:
	ITDDocument*		m_pDoc;
	CString				m_sSourceList;
	BOOL				m_bInitialized;

	HtmlTable			m_Table;
	ITDButton*			m_pBtnRefresh;

	ITDPropertyData*	m_pProperty[PROPERTY_ID_SIZE];

	XilinxDevices		m_XilinxDevices;

	SourceGroup*		m_pSourceList;

	list<CString>		m_pSynthesisList;
};
#endif//__XILINX_SYNTHESIS_H__
