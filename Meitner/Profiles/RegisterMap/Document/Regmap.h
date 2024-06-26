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
// Title : Meitner processor register map document
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __REGMAP_H__
#define __REGMAP_H__
#include "testdrive_document.h"
#include "SystemConfigMTSP.h"
#include "Locale.h"
#include "HString.h"

#define COLOR_ENABLED	 0x0
#define COLOR_DISENABLED RGB(170, 170, 170)

typedef enum { TABLE_BOARDER_LEFT, TABLE_BOARDER_RIGHT, TABLE_BOARDER_TOP, TABLE_BOARDER_BOTTOM } TABLE_BOARDER;

#define LOGI g_pSystem->LogInfo
#define LOGW g_pSystem->LogWarning
#define LOGE g_pSystem->LogError

typedef struct {
	union {
		DWORD udata;
		int	  idata;
		float fdata;
	};
	BOOL	 bDiff;
	LONGLONG llPrevTime;
} REG_DATA;

extern ITDDocument *g_pDoc;
extern ITDHtml	   *g_pHtml;

extern LPCTSTR		g_sDefaultFontName;

CString				GetBitsString(DWORD data, DWORD bits_count);
LPCTSTR				GetBufferFormatString(DWORD format);
void				RetrieveSimulationTime(CString &sTime, UINT64 lTime);
void				RetrieveBytes(CString &sBytes, UINT64 lByteSize);

extern LPCTSTR		g_sInvalid;
extern LPCTSTR		g_sBoolean[2];
extern LPCTSTR		g_s4DElement[4];

float				Fixed2Float(DWORD data, BOOL bUseSign, int bits_high, int bits_low);
void				PresentTime(CString &sTime, UINT64 llTime);

typedef struct {
	BOOL	bCoreTrace;
	BOOL	bMemoryAutoTrace;
	CString sCoreTab;
	int		iMemoryTab;
} ENV;

class Regmap
{
protected:
	virtual ~Regmap(void);

public:
	Regmap(LPCTSTR sName = NULL);
	static void	   ReleaseAll(void);
	static BOOL	   Update(void);
	static void	   PostUpdate(void);
	static void	   Initialize(void);
	static void	   Broadcast(LPVOID pData);
	static void	   Command(LPCTSTR lpszURL);
	static void	   UpdateSimulationTime(void);
	inline LPCTSTR Name(void) const
	{
		return m_sName;
	}

	static MTSP_REGMAP		  *m_pReg;
	static MTSP_REGMAP_CORE	  *m_pCore;
	static MTSP_REGMAP_THREAD *m_pThread;

	static ENV				   env;

protected:
	static void	 SetText(LPCTSTR lpszTarget, LPCTSTR lpszFormat, ...);

	virtual BOOL OnUpdate(void) = 0;
	virtual void OnBroadcast(LPVOID pData);
	virtual BOOL OnCommand(LPCTSTR lpszURL);

	void		 SetTableID(LPCTSTR sTableName);
	void		 ClearTable(LPCTSTR sTableName = NULL);
	void		 AppendCell(LPCTSTR sCell, LPCTSTR sBody = NULL);
	void		 NewRow(LPCTSTR sTableName = NULL);
	void		 NewTD(void);
	void		 NewTH(void);
	void		 SetTBody(LPCTSTR lpszFormat, ...);
	void		 SetTStyle(LPCTSTR sTarget, LPCTSTR sAttrib);
	void		 SetTBorderRight(LPCTSTR sStyle);
	void		 SetTBorderLeft(LPCTSTR sStyle);
	void		 SetTBorderTop(LPCTSTR sStyle);
	void		 SetTBorderBottom(LPCTSTR sStyle);
	void		 SetTClassAdd(LPCTSTR sName);
	void		 SetTClassRemove(LPCTSTR sName);
	void		 SetTID(LPCTSTR lpszFormat, ...);
	void		 SetColSpan(int iSize);
	void		 SetRowSpan(int iSize);
	void		 SetTextAlignment(TEXT_ALIGN align);
	void		 SetBoarderWidth(TABLE_BOARDER side, int iWidth);
	void		 SetCheckBox(LPCTSTR sName, BOOL bCheck = TRUE);
	void		 ShowCollapse(LPCTSTR sName, BOOL bShow = TRUE);
	inline BOOL	 IsInitialized(void)
	{
		return m_pReg->magic_code == MTSP_MAGIC_CODE;
	}

private:
	static Regmap *m_pHead;
	static UINT64  m_lSimulationTime;
	Regmap		  *m_pNext;
	LPCTSTR		   m_sName;
	CString		   m_sTableName;
	BOOL		   m_bTableNewRow;
};
#endif //__REGMAP_H__
