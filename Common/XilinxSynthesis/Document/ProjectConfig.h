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
#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__
#include "common.h"

#define	GLOBAL_NAME				"XilinxSynthesis"
#define	SCORE_FILE_NAME			_T("." GLOBAL_NAME ".score")
#define	SOURCE_LIST_FILE_NAME	_T("." GLOBAL_NAME ".sources")
#define	WORK_DIR_NAME			_T("." GLOBAL_NAME ".work")
#define	NO_SEARCH_NAME			_T("." GLOBAL_NAME ".nosearch")
#define	NO_SEARCH_NAME2			_T("." "TestDrive" ".nosearch")

extern LPCTSTR		g_sGlobalName;
extern LPCTSTR		g_sEmpty;
extern LPCTSTR		g_sNotepadPath;

typedef enum{
	CONFIG_ID_FAMILY,
	CONFIG_ID_DEVICE,
	CONFIG_ID_PACKAGE,
	CONFIG_ID_SPEED_GRADE,
	CONFIG_ID_TARGET_SPEED,
	CONFIG_ID_CLOCKS,
	CONFIG_ID_RESETS,
	CONFIG_ID_MAX_PATHS,
	CONFIG_ID_SIZE
}CONFIG_ID;

typedef struct{
	int			iMaxPaths;
	TCHAR		sProjectPath[2048];
	TCHAR		sXilinxPath[2048];
	struct{
		TCHAR		sFamily[1024];
		TCHAR		sDevice[1024];
		TCHAR		sPackage[1024];
		TCHAR		sSpeedGrade[1024];
		TCHAR		sTargetSpeed[1024];
		TCHAR		sClocks[1024];
		TCHAR		sResets[1024];
	}spec;
	CString		sDocPath;
}CONFIG;

class SourceVector;
class HtmlTable;

class ProjectConfig
{
public:
	ProjectConfig(void);
	virtual ~ProjectConfig(void);

	BOOL UpdateConfig(CONFIG_ID id, BOOL bUpdate = TRUE);
	BOOL UpdateConfigAll(BOOL bUpdate = TRUE);
	CString RetrievePath(LPCTSTR sExtPath = NULL);
	BOOL RetrieveSourcePath(LPCTSTR sSrcFile, CString& sSrcPath);
	void SetRebuild(void)	{m_bMustRebuild	= TRUE;}
	CString MakeDevicePartName(void);

	static CONFIG		m_Config;

protected:
	BOOL GetDefaultProjectPath(void);
	BOOL CheckConfigCompleteness(void);
	BOOL MakeProjectConfigFile(SourceVector* pSource);
	BOOL IsMustRebuild(void)	{if(m_bMustRebuild){m_bMustRebuild	= FALSE;return TRUE;}return FALSE;}

	static HtmlTable*	m_pTable;

private:

	static DWORD		m_dwProjectResourceRefCount;
	static BOOL			m_bMustRebuild;
};

time_t GetCurrentDayTime(void);
#endif//__PROJECT_CONFIG_H__
