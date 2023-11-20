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
// Title : System map
// Rev.  : 11/8/2023 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __DESIGN_MAP_H__
#define __DESIGN_MAP_H__
#include "testdrive_document.h"
#include "Locale.h"
#include <map>

using namespace std;

typedef enum {
	CMD_ID_URL,
	CMD_ID_DOC,
	CMD_ID_MANUAL,
	CMD_ID_LUA,
	CMD_ID_PROJECT,
	CMD_ID_NEW_MODULE_FILE,
	CMD_ID_SIZE
} CMD_ID;

typedef enum {
	MAIN_CMD_WATCHDOG_FREE = 1000,
} MAIN_CMD;

typedef enum {
	USER_CMD_UPDATE,
} USER_CMD;

typedef enum {
	PROPERTY_ID_EDITOR,
	PROPERTY_ID_SIZE
} PROPERTY_ID;

typedef struct {
	CString		sFilename;
} SOURCE_VIEW;

class CDesignMap :
	public TDImplDocumentBase,
	public ITDHtmlManager {
public:
	CDesignMap(ITDDocument* pDoc);
	virtual ~CDesignMap(void);

	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);
	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(void, OnSize)(int width, int height);
	STDMETHOD_(LPCTSTR, OnHtmlBeforeNavigate)(DWORD dwID, LPCTSTR lpszURL);
	STDMETHOD_(void, OnHtmlDocumentComplete)(DWORD dwID, LPCTSTR lpszURL);
	STDMETHOD_(LPCTSTR, OnHtmlNewWindowRequest)(DWORD dwID, LPCTSTR lpszURL, BOOL bUserInitiated);

	int CheckModuleFile(LPCTSTR sFileName, LPCTSTR sModuleName);
	void UpdateSourceViews(bool bNewProject);
	bool OpenSourceFile(LPCTSTR sModuleName, BOOL bMustOpen = FALSE);
	void OpenEditFile(LPCTSTR sFileName);


protected:
	BOOL						m_bInitialized;
	ITDMemory*					m_pMemory;
	DESIGNMAP_CONFIG*			m_pConfig;
	CString						m_sWorkPath;
	CString						m_sOutputPath;
	CString						m_sDesignFilePath;
	map<CString, SOURCE_VIEW>	m_SourceViewList;
};
#endif//__DESIGN_MAP_H__
