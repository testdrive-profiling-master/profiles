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
// Title : Virtual display
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __VIRTUAL_DISPLAY_H__
#define __VIRTUAL_DISPLAY_H__
#include "testdrive_document.h"
#include "Locale.h"
#include "VirtualDisplayConfig.h"

typedef enum{
	PROPERTY_ID_AXIS,
	PROPERTY_ID_SHOW_FRONT,
	PROPERTY_ID_USE_ALPHA,
	PROPERTY_ID_MOVIE_SAVE,
	PROPERTY_ID_MOVIE_FPS,
	PROPERTY_ID_MOVIE_FILE_PATH,
	PROPERTY_ID_STOP_EACH_FRAME,
	PROPERTY_ID_PREFERRED_SCREEN_MODE,
	PROPERTY_ID_PREFERRED_SCREEN_SIZE,
}PROPERTY_ID;

class VirtualDisplay :
	public TDImplDocumentBase,
	public ITDBufferManager,
	public ITDButtonManager
{
public:
	VirtualDisplay(ITDDocument* pDoc);
	virtual ~VirtualDisplay(void);

	// 도큐먼트
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);
	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(void, OnSize)(int width, int height);
	STDMETHOD_(void, OnShow)(BOOL bShow);

	// 스크린
	STDMETHOD_(void, OnBufferMouseEvent)(DWORD dwID, DWORD x, DWORD y, DWORD dwFlag);
	STDMETHOD_(void, OnBufferClick)(DWORD dwID);
	STDMETHOD_(BOOL, OnBufferBeforeFileOpen)(DWORD dwID, LPCTSTR sFilename, DWORD dwWidth, DWORD dwHeight);
	STDMETHOD_(void, OnButtonClick)(DWORD dwID);

	BOOL CheckDisplayUpdate(void);
	void UpdateWiseSay(void);
	void ExternalCommand(DWORD dwCommand, DWORD dwParam);

protected:
	ITDDocument*			m_pDoc;
	
	ITDMemory*				m_pMemory;

	ITDBuffer*				m_pScreen;
	ITDButton*				m_pBtnPlay;
	ITDReport*				m_pReport;
	DisplayConfig*			m_pConfig;

	BOOL					m_bShow;
	BOOL					m_bUseAlphaChannel;
	
	ITDMovie*				m_pMovie;
	int						m_iMovieFPS;
	CString					m_sMovieFilePath;

	BOOL					m_bWaitPlayBtn;

	DWORD					m_iCoordinateType;
	CString					m_sCoordinateType;

	CString					m_sWiseSayFile;
	CString					m_sWiseSayConfigFile;

	DWORD					m_dwWiseTime;
	DWORD					m_dwUpdateTimeout;

	CString					m_sPreferredScreenMode;
	CString					m_sPreferredScreenSize;

	struct{
		BYTE*				pUsed;
		DWORD				dwTotalCount;
		DWORD				dwRestCount;
		DWORD				dwIndex;
		DWORD				dwTime;
	}m_Wise;
};
#endif//__VIRTUAL_DISPLAY_H__
