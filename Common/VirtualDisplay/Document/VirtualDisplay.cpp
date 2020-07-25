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
#include "VirtualDisplay.h"
#include "testdrive_document.inl"
#include <assert.h>

REGISTER_LOCALED_DOCUMENT(VirtualDisplay)

const LPCTSTR		g_DisplayName = _T("Display");
#define		MAX_DISPLAY_LENGTH		4096			// Maximum display screen width/height

typedef enum {
	COORDINATE_TYPE_SCREEN,
	COORDINATE_TYPE_OPENGL,
	COORDINATE_TYPE_SIZE
} COORDINATE_TYPE;

typedef enum {
	COMMAND_ID_CHECK_DISPLAY	= 100,				// check display update
} COMMAND_ID;

LPCTSTR	g_sCoordinateType[COORDINATE_TYPE_SIZE] = {
	_T("Screen"),	// COORDINATE_TYPE_SCREEN
	_T("OpenGL"),	// COORDINATE_TYPE_OPENGL
};

LPCTSTR g_sColorFormat[] = {
	_T("A8"),		// DISPLAY_FORMAT_ALPHA
	_T("L8"),		// DISPLAY_FORMAT_LUMINANCE
	_T("L8A8"),		// DISPLAY_FORMAT_LUMINANCE_ALPHA
	_T("R5G6B5"),	// DISPLAY_FORMAT_RGB_565
	_T("R4G4B4A4"),	// DISPLAY_FORMAT_RGBA_4444
	_T("R5G5B5A1"),	// DISPLAY_FORMAT_RGBA_5551
	_T("R8G8B8"),	// DISPLAY_FORMAT_RGB_888
	_T("A8B8G8R8"),	// DISPLAY_FORMAT_ABGR_8888
	_T("R8G8B8A8"),	// DISPLAY_FORMAT_RGBA_8888
	_T("A8R8G8B8"),	// DISPLAY_FORMAT_ARGB_8888
};

LPCTSTR g_sWiseSay = _T("WISE");

static LPCTSTR	__sListPreferredScreenMode[] = {
	_T("TestDrive"),
	_T("Window"),
	_T("FullScreen"),
	NULL
};

static LPCTSTR	__sListPreferredScreenSize[] = {
	_T("Orignal"),
	_T("160x120 (QCIF)"),
	_T("320x240 (QVGA)"),
	_T("640x480 (VGA)"),
	_T("1024x768 (XGA)"),
	_T("1920x1080 (FullHD)"),
	_T("3840x2160 (4K UHD)"),
	_T("7680x4320 (8K UHD)"),
	NULL
};

VirtualDisplay::VirtualDisplay(ITDDocument* pDoc)
{
	m_pDoc				= pDoc;
	m_bShow				= TRUE;
	m_bUseAlphaChannel	= FALSE;
	m_pMemory			= NULL;
	m_bWaitPlayBtn		= FALSE;
	m_dwUpdateTimeout	= 0;
	ZeroMemory(&m_Wise, sizeof(m_Wise));
	{
		// wise saying initialization
		m_sWiseSayFile			= g_pSystem->RetrieveFullPath(_T("wise.ini"));
		m_sWiseSayConfigFile	= g_pSystem->RetrieveFullPath(_T("wise_config.ini"));
		m_Wise.dwRestCount		=
			m_Wise.dwTotalCount		= GetPrivateProfileInt(g_sWiseSay, _T("COUNT"), 0, m_sWiseSayFile);

		if(m_Wise.dwTotalCount) {
			m_Wise.pUsed		= new BYTE[m_Wise.dwTotalCount];
			CString	sID;

			for(int i = 0; i < m_Wise.dwTotalCount; i++) {
				sID.Format(_T("SAY%d"), i);
				m_Wise.pUsed[i] = GetPrivateProfileInt(g_sWiseSay, sID, 0, m_sWiseSayFile);

				if(m_Wise.pUsed[i]) {
					m_Wise.dwRestCount--;
				}
			}
		}
	}
	{
		// Get display memory
		CString		sName(g_DisplayName);	// display memory name
		ITDMemory*	pMemory		= g_pSystem->GetMemory(NULL);		// get system memory

		if(pMemory) {												// new name that added post-fix '_Display' with name of system memory
			sName = pMemory->GetName();
			sName.AppendFormat(_T("_%s"), g_DisplayName);
			m_pMemory	= pMemory;
		}

		// Initialize system configuration
		pMemory = g_pSystem->GetMemory(sName);						// get display memory

		if(!pMemory) pMemory = g_pSystem->GetMemory(g_DisplayName);	// if not exist search with "Display"

		if(!pMemory) pMemory = m_pMemory;							// if not exist this too, set to system memory

		if(!pMemory) {												// if system memory is not existed too, create display memory
			pMemory = g_pSystem->GetMemory(g_DisplayName, TRUE);
			assert(pMemory != NULL);
			pMemory->Create(2048 * 2048 * 4);
		}

		// Set Display window handle
		m_pConfig				= (DisplayConfig*)(pMemory->GetConfig());
		m_pConfig->hWndHandle	= pDoc->GetWindowHandle();

		if(!m_pMemory) m_pMemory = pMemory;
	}
	m_pScreen	= pDoc->CreateBuffer(NULL, 0, 0, 10, 10);
	m_pScreen->SetEnable(BUFFER_INTERFACE_OUTLINE);
	m_pScreen->SetEnable(BUFFER_INTERFACE_AUTOFIT);
	m_pScreen->SetEnable(BUFFER_INTERFACE_LOAD_FROM_FILE);
	m_pScreen->SetManager(this);
	m_pBtnPlay	= pDoc->CreateButton(NULL, 0, 0, 25, 19);
	m_pBtnPlay->SetManager(this);
	m_pBtnPlay->SetText(_L(PLAY));
	m_pBtnPlay->GetObject()->SetEnable(FALSE);
	m_pReport	= pDoc->CreateReport(NULL, 0, 0, 10, 10);
	m_pReport->EnableEdit(FALSE);
	m_pReport->SetFont(_L(FONT));
	m_pReport->SetText(_T("."));
	m_pReport->SetHeight(10);
	UpdateWiseSay();
	m_pScreen->Create(10, 10);
	{
		ITDPropertyData* pProperty;
		// coordinates
		m_sCoordinateType.GetBuffer(MAX_PATH);
		m_iCoordinateType	= COORDINATE_TYPE_SCREEN;
		m_sCoordinateType	= g_sCoordinateType[m_iCoordinateType];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_AXIS, _L(SCREEN_COORDINATE), (DWORD_PTR)m_sCoordinateType.GetBuffer(), _L(DESC_SCREEN_COORDINATE));
		pProperty->AllowEdit(FALSE);

		for(int i = 0; i < COORDINATE_TYPE_SIZE; i++) pProperty->AddOption(g_sCoordinateType[i]);

		pProperty->UpdateConfigFile();

		for(int i = 0; i < COORDINATE_TYPE_SIZE; i++)
			if(!m_sCoordinateType.Compare(g_sCoordinateType[i])) {
				m_iCoordinateType	= i;
				break;
			}

		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_SHOW_FRONT, _L(FRONT_BUFFER), (DWORD_PTR)&m_pConfig->bShowFrontBuffer, _L(DESC_FRONT_BUFFER));
		pProperty->UpdateConfigFile();
		// use alpha channel
// 		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_USE_ALPHA, _T("Alpha channel"), (DWORD_PTR)&m_bUseAlphaChannel, _T("Appears on the screen using the alpha channel.\nNormal display buffer will be assumed alpha as zero."));
// 		pProperty->UpdateConfigFile();
// 		m_pScreen->UseAlphaChannel(m_bUseAlphaChannel);
		m_pMovie			= NULL;
		m_pConfig->bMovieSave	= FALSE;
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_MOVIE_SAVE, _L(MOVIE_RECORD), (DWORD_PTR)&m_pConfig->bMovieSave, _L(DESC_MOVIE_RECORD));
		m_iMovieFPS			= 30;
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_INT, PROPERTY_ID_MOVIE_FPS, _L(MOVIE_FPS), (DWORD_PTR)&m_iMovieFPS, _L(DESC_MOVIE_FPS));
		pProperty->UpdateConfigFile();
		m_sMovieFilePath.GetBuffer(1024);
		m_sMovieFilePath	= _T("result.avi");
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_MOVIE_FILE_PATH, _L(MOVIE_FILE), (DWORD_PTR)m_sMovieFilePath.GetBuffer(), _L(DESC_MOVIE_FILE));
		pProperty->UpdateConfigFile();
		m_pConfig->bFramePause	= FALSE;
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_STOP_EACH_FRAME, _L(MOVIE_STOP), (DWORD_PTR)&m_pConfig->bFramePause, _L(DESC_MOVIE_STOP));
		m_sPreferredScreenMode.GetBuffer(MAX_PATH);
		m_sPreferredScreenMode		= __sListPreferredScreenMode[0];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_PREFERRED_SCREEN_MODE, _L(PREFERRED_SCREEN_MODE), (DWORD_PTR)m_sPreferredScreenMode.GetBuffer(), _L(DESC_PREFERRED_SCREEN_MODE));
		pProperty->UpdateConfigFile();

		for(int i = 0; __sListPreferredScreenMode[i]; i++) pProperty->AddOption(__sListPreferredScreenMode[i]);

		m_sPreferredScreenSize.GetBuffer(MAX_PATH);
		m_sPreferredScreenSize	= __sListPreferredScreenSize[0];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_PREFERRED_SCREEN_SIZE, _L(PREFERRED_SCREEN_SIZE), (DWORD_PTR)m_sPreferredScreenSize.GetBuffer(), _L(DESC_PREFERRED_SCREEN_SIZE));
		pProperty->UpdateConfigFile();

		for(int i = 0; __sListPreferredScreenSize[i]; i++) pProperty->AddOption(__sListPreferredScreenSize[i]);
	}
	m_pScreen->SetSmoothModeHighQuality();
	m_pScreen->CreateFromFile(_T("default.png"));
	OnCommand(COMMAND_ID_CHECK_DISPLAY);
}

VirtualDisplay::~VirtualDisplay(void)
{
	m_pConfig->Front.bUpdate	= FALSE;

	if(m_pMovie) {
		m_pMovie->Release();
		m_pMovie	= NULL;
	}

	m_pConfig->hWndHandle	= NULL;

	if(m_Wise.pUsed) {
		delete [] m_Wise.pUsed;
		m_Wise.pUsed	= NULL;
	}
}

void VirtualDisplay::UpdateWiseSay(void)
{
	if(!m_Wise.dwTotalCount) return;

	{
		//refresh index(random)
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		m_Wise.dwIndex	+= (sys_time.wHour * 60 * 60 + sys_time.wMinute * 60 + sys_time.wSecond);
	}
	{
		// Avoid duplication
		if(!m_Wise.dwRestCount) {
			ZeroMemory(m_Wise.pUsed, m_Wise.dwTotalCount * sizeof(BYTE));
			m_Wise.dwRestCount = m_Wise.dwTotalCount;
			_tremove(m_sWiseSayConfigFile);
		}

		for(DWORD i = 0; i < m_Wise.dwTotalCount; i++) {
			m_Wise.dwIndex++;
			m_Wise.dwIndex %= m_Wise.dwTotalCount;

			if(!m_Wise.pUsed[m_Wise.dwIndex]) {
				m_Wise.pUsed[m_Wise.dwIndex] = 1;
				break;
			}
		}

		m_Wise.dwRestCount--;
	}
	{
		// Output
		CString keyname;
		TCHAR wise_say[1024];
		keyname.Format(_T("SAY%d"), m_Wise.dwIndex);
		GetPrivateProfileString(g_sWiseSay, keyname, _T(""), wise_say, 1024, m_sWiseSayFile);
		WritePrivateProfileString(g_sWiseSay, keyname, _T("1"), m_sWiseSayConfigFile);
		m_pReport->SetText(_T("%s"), wise_say);
	}
}

BOOL VirtualDisplay::CheckDisplayUpdate(void)
{
	BOOL			bRet			= FALSE;
	BOOL			bFrontBuffer	= m_pConfig->bShowFrontBuffer;
	DisplayBuffer*	pBuffer			= bFrontBuffer ? &m_pConfig->Front : &m_pConfig->Back;

	if(!bFrontBuffer) {	// bypass front buffer update, if you set the back buffer.
		if(m_pConfig->Front.bUpdate)
			m_pConfig->Front.bUpdate	= FALSE;
	} else if(m_bWaitPlayBtn) return FALSE;

	// Initialize movie control, if it's needed.
	if(!m_pMovie && m_pConfig->bMovieSave && m_pConfig->iWidth && m_pConfig->iHeight) {
		m_pMovie	= g_pSystem->CreateMovieInterface();
		CString	sProjectPath(g_pSystem->GetProjectPath());
		m_pMovie->Create(sProjectPath + m_sMovieFilePath, m_pConfig->iWidth, m_pConfig->iHeight, m_iMovieFPS);
	}

	// Update framebuffer
	if(pBuffer->bUpdate) {
		if(!m_pMovie || !bFrontBuffer)
			pBuffer->bUpdate = FALSE;

		DWORD	dwWidth			= (DWORD)m_pConfig->iWidth;
		DWORD	dwHeight		= (DWORD)m_pConfig->iHeight;
		DWORD	dwByteStride	= m_pConfig->dwByteStride;
		COLORFORMAT format		= (COLORFORMAT)m_pConfig->ColorFormat;
		BOOL	bDraw			= FALSE;

		if(dwWidth && dwHeight) {
			bDraw	= TRUE;

			// update resolution
			if(m_pScreen->Width() != dwWidth || m_pScreen->Height() != dwHeight || m_pScreen->ColorFormat() != format) {
				m_pScreen->Create(dwWidth, dwHeight, format);
				m_pDoc->InvalidateLayout();
			}

			// copy buffer
			{
				BYTE* pMem	= m_pMemory->GetPointer(pBuffer->dwAddress, dwByteStride * dwHeight);

				if(pMem) {
					if(m_pConfig->decoder) {
						if(!m_pConfig->decoder((void*)m_pScreen->GetPointer(), pMem))
							return FALSE;
					} else {
						if(!m_pScreen->CopyFromMemory(pMem, dwByteStride, m_pConfig->bReverse))
							return FALSE;
					}

					m_pScreen->Present();
				}
			}
		}

		if(bFrontBuffer) {
			if(m_pMovie && bDraw) {	// If the movie is activated.
				m_pMovie->Buffer()->CopyFromBuffer(m_pScreen);
				m_pMovie->SetFrame();
			}

			if(m_pConfig->bFramePause) {
				m_pReport->SetText(_L(PRESS_BUTTON_TO_CONTINUE));
				m_bWaitPlayBtn		= TRUE;
				m_pBtnPlay->GetObject()->SetEnable();
			} else if(m_pMovie)
				pBuffer->bUpdate	= FALSE;
		}

		bRet = TRUE;
	}

	return bRet;
}

BOOL VirtualDisplay::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData();

	switch(pProperty->GetID()) {
	case PROPERTY_ID_AXIS:	// coordinates
		for(int i = 0; i < COORDINATE_TYPE_SIZE; i++)
			if(!m_sCoordinateType.Compare(g_sCoordinateType[i])) {
				m_iCoordinateType	= i;
				break;
			}

		break;

	case PROPERTY_ID_SHOW_FRONT:	// Front / Back buffer
		if(m_pConfig->bShowFrontBuffer) m_pConfig->Front.bUpdate	= TRUE;
		else m_pConfig->Back.bUpdate	= TRUE;

		UpdateWiseSay();
		break;

	case PROPERTY_ID_USE_ALPHA:	// Use alpha channel
		//g_pSystem->LogInfo(_T("m_bUseAlphaChannel = %d"), m_bUseAlphaChannel);
		m_pScreen->UseAlphaChannel(m_bUseAlphaChannel);
		m_pScreen->Present();
		break;

	case PROPERTY_ID_MOVIE_SAVE:
		if(m_pConfig->bMovieSave) {
			// start record
			if(!m_pConfig->bShowFrontBuffer) {
				m_pConfig->bMovieSave	= FALSE;
				pProperty->UpdateData(FALSE);
				g_pSystem->LogWarning(_L(CHOOSE_FRONTBUFFER_FIRST));
			} else {
				g_pSystem->LogInfo(_L(ACTIVATE_MOVIE_RECOORD));
			}
		} else {
			// end record
			if(m_pMovie) {
				m_pMovie->Release();
				m_pMovie	= NULL;
				g_pSystem->LogInfo(_L(COMPLETE_MOVIE_RECOORD));
			} else {
				g_pSystem->LogInfo(_L(CANCEL_MOVIE_RECOORD));
			}
		}

		break;

	case PROPERTY_ID_STOP_EACH_FRAME:
		if(!m_pConfig->bFramePause) OnButtonClick(0);
		else m_pDoc->SetForegroundDocument();

		m_pDoc->InvalidateLayout();
		return TRUE;

	case PROPERTY_ID_PREFERRED_SCREEN_MODE:
		SetEnvironmentVariable(_T("PREFERRED_SCREEN_MODE"), m_sPreferredScreenMode);
		break;

	case PROPERTY_ID_PREFERRED_SCREEN_SIZE:
		SetEnvironmentVariable(_T("PREFERRED_SCREEN_SIZE"), m_sPreferredScreenSize);
		break;
	}

	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

CString GetStringFromSystemMemory(DWORD offset)
{
	char*	str_ansi	= (char*)g_pSystem->GetMemory()->GetPointer(offset);
	int		str_size	= MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, NULL, NULL);
	TCHAR*	str_unicode	= new TCHAR[str_size];
	MultiByteToWideChar(CP_ACP, 0, str_ansi, strlen(str_ansi) + 1, str_unicode, str_size);
	CString	str(str_unicode);
	delete [] str_unicode;
	return str;
}

void VirtualDisplay::ExternalCommand(DWORD dwCommand, DWORD dwParam)
{
	WORD	op	= HIWORD(dwCommand);
	CheckDisplayUpdate();

	switch(LOWORD(dwCommand)) {
	case DISPLAY_CMD_SAVE_TO_FILE:
		m_pScreen->SaveToFile(GetStringFromSystemMemory(dwParam));
		break;

	case DISPLAY_CMD_LOAD_FROM_FILE: {
		DWORD*	pMem	= (DWORD*)m_pMemory->GetPointer(dwParam);
		CString	sPath	= g_pSystem->RetrieveFullPath(GetStringFromSystemMemory(dwParam + 12));

		if(m_pScreen->CreateFromFile(sPath, (COLORFORMAT)pMem[0])) {
			m_pScreen->CopyToMemory(m_pMemory->GetPointer(pMem[1]), pMem[2]);
			m_pConfig->ColorFormat	= (DISPLAY_FORMAT)m_pScreen->ColorFormat();
			m_pConfig->iWidth		= m_pScreen->Width();
			m_pConfig->iHeight		= m_pScreen->Height();
			m_pConfig->dwByteStride	= m_pScreen->GetBytesStride();
			*pMem	= TRUE;
		} else {
			*pMem	= FALSE;
		}
	}
	break;

	case DISPLAY_CMD_SET_FOREGROUND:
		m_pDoc->SetForegroundDocument();
		break;

	default:
		g_pSystem->LogError(_T("Invalid external command."));
	}
}

BOOL VirtualDisplay::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch(command) {
	case 0:
		UpdateWiseSay();
		break;

	case COMMAND_ID_CHECK_DISPLAY:
		m_pDoc->KillTimer(COMMAND_ID_CHECK_DISPLAY);

		if(!m_bShow) break;

		{
			DWORD dwDelay;

			if(CheckDisplayUpdate()) {
				m_dwUpdateTimeout	= 500;
				dwDelay				= 5;
				m_Wise.dwTime		= 0;
			} else {
				if(m_dwUpdateTimeout) {
					m_dwUpdateTimeout--;
					dwDelay			= 5;
				} else {
					dwDelay			= 250;
				}

				if(m_Wise.dwTime >= 600000) { // Updated the quote one minute after
					UpdateWiseSay();
					m_Wise.dwTime	= 0;
				}
			}

			m_pDoc->SetTimer(COMMAND_ID_CHECK_DISPLAY, dwDelay);
			m_Wise.dwTime += dwDelay;
		}
		break;

	case TD_EXTERNAL_COMMAND:
		ExternalCommand((DWORD)wParam, (DWORD)lParam);
		break;

	default:
		break;
	}

	return TRUE;
}

void VirtualDisplay::OnBufferClick(DWORD dwID)
{
}

void VirtualDisplay::OnButtonClick(DWORD dwID)
{
	m_pBtnPlay->GetObject()->SetEnable(FALSE);
	m_pConfig->Front.bUpdate	= FALSE;
	m_bWaitPlayBtn				= FALSE;
}

void VirtualDisplay::OnShow(BOOL bShow)
{
	m_bShow	= bShow;
	OnCommand(COMMAND_ID_CHECK_DISPLAY);
}

void VirtualDisplay::OnSize(int width, int height)
{
	ITDLayout* pLayout;
	int	report_height;
	int x = 0;
	// Leave space for pixel output information
	report_height	= width < 800 ? 34 : 23;
	height -= report_height;

	if(m_pScreen) {
		// Apply
		pLayout = m_pScreen->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
		{
			// Update button
			ITDObject* pObject	= m_pBtnPlay->GetObject();

			if(m_pConfig->bFramePause != pObject->IsVisible())
				pObject->Show(m_pConfig->bFramePause);

			if(m_pConfig->bFramePause) {
				pLayout	= pObject->GetLayout();
				pLayout->SetPosition(0, height + ((report_height - 19) >> 1));
				x += 27;
				width -= 27;
			}
		}
		{
			// Update report
			pLayout	= m_pReport->GetObject()->GetLayout();
			pLayout->SetPosition(x, height);
			pLayout->SetSize(width, report_height);
		}
	}
}

void VirtualDisplay::OnBufferMouseEvent(DWORD dwID, DWORD x, DWORD y, DWORD dwFlag)
{
	if(!m_pScreen->IsInitialize() || !m_pConfig->iWidth || !m_pConfig->iHeight) {
		return;
	}

	int		coord_x, coord_y;
	{
		// Print coordinate
		coord_x		= x;
		coord_y		= m_pScreen->Height() - y - 1;

		if(m_iCoordinateType == COORDINATE_TYPE_OPENGL) y = coord_y;

		m_pReport->SetText(_T("%s_%s_%dx%d [X:%4d,Y:%4d] = "), m_pConfig->bShowFrontBuffer ? _T("Front") : _T("Back"), g_sColorFormat[m_pConfig->ColorFormat], m_pScreen->Width(), m_pScreen->Height(), x, y);
	}
	{
		// Print Color
		UINT	r, g, b, a;
		float	fr, fg, fb, fa;
		DWORD	offset	= m_pScreen->Width() * coord_y + coord_x;
		{
			// pick color
			switch(m_pConfig->ColorFormat) {
			case DISPLAY_FORMAT_ALPHA: {
				r = g = b = 0;
				fr = fg = fb = 0;
				a = ((BYTE*)m_pScreen->GetPointer())[offset];
				fa = a / 255.f;
			}
			break;

			case DISPLAY_FORMAT_LUMINANCE: {
				r = g = b = ((BYTE*)m_pScreen->GetPointer())[offset];
				fr = fg = fb = r / 255.f;
				a = 0;
				fa = 0;
			}
			break;

			case DISPLAY_FORMAT_LUMINANCE_ALPHA: {
				WORD color = ((WORD*)m_pScreen->GetPointer())[offset];
				r = g = b = color >> 8;
				fr = fg = fb = r / 255.f;
				a = color & 0xFF;
				fa = a / 255.f;
			}
			break;

			case DISPLAY_FORMAT_RGB_565: {
				WORD color = ((WORD*)m_pScreen->GetPointer())[offset];
				r = ((color >> 11) & 0x1F);
				g = ((color >> 5) & 0x3F);
				b = (color & 0x1F);
				a = 0;
				fr = r / 31.f;
				fg = g / 63.f;
				fb = b / 31.f;
				fa = 0;
			}
			break;

			case DISPLAY_FORMAT_RGBA_4444: {
				WORD color = ((WORD*)m_pScreen->GetPointer())[offset];
				r = ((color >> 12) & 0xF);
				g = ((color >> 8) & 0xF);
				b = ((color >> 4) & 0xF);
				a = (color & 0xF);
				fr = r / 15.f;
				fg = g / 15.f;
				fb = b / 15.f;
				fa = a / 15.f;
			}
			break;

			case DISPLAY_FORMAT_RGBA_5551: {
				WORD color = ((WORD*)m_pScreen->GetPointer())[offset];
				r = ((color >> 11) & 0x1F);
				g = ((color >> 6) & 0x1F);
				b = ((color >> 1) & 0x1F);
				a = (color & 0x1);
				fr = r / 15.f;
				fg = g / 15.f;
				fb = b / 15.f;
				fa = a / 1.f;
			}
			break;

			case DISPLAY_FORMAT_RGB_888: {
				BYTE* pColor = &(((BYTE*)m_pScreen->GetPointer())[offset * 3]);
				r = pColor[0];
				g = pColor[1];
				b = pColor[2];
				a = 0;
				goto GET_32BIT_COLOR;
			}
			break;

			case DISPLAY_FORMAT_ABGR_8888: {
				BYTE* pColor = (BYTE*) & (((DWORD*)m_pScreen->GetPointer())[offset]);
				r = pColor[0];
				g = pColor[1];
				b = pColor[2];
				a = pColor[3];
				goto GET_32BIT_COLOR;
			}

			case DISPLAY_FORMAT_RGBA_8888: {
				BYTE* pColor = (BYTE*) & (((DWORD*)m_pScreen->GetPointer())[offset]);
				r = pColor[3];
				g = pColor[2];
				b = pColor[1];
				a = pColor[0];
				goto GET_32BIT_COLOR;
			}

			case DISPLAY_FORMAT_ARGB_8888: {
				BYTE* pColor = (BYTE*) & (((DWORD*)m_pScreen->GetPointer())[offset]);
				r = pColor[2];
				g = pColor[1];
				b = pColor[0];
				a = pColor[3];
			}

		GET_32BIT_COLOR:
			fr = r / 255.f;
			fg = g / 255.f;
			fb = b / 255.f;
			fa = a / 255.f;
			break;
			}
		}
		// Show RED
		m_pReport->SetColor(RGB(fr * 255, 0, 0));
		m_pReport->AppendText(_L(BOX));
		m_pReport->SetColor(RGB(255, 0, 0));
		m_pReport->AppendText(_T("R(%3d/%.3f/0x%02X)"), r, fr, r);
		m_pReport->SetColor(RGB(0, 0, 0));
		m_pReport->AppendText(_T("  "));
		// Show GREEN
		m_pReport->SetColor(RGB(0, fg * 255, 0));
		m_pReport->AppendText(_L(BOX));
		m_pReport->SetColor(RGB(0, 128, 0));
		m_pReport->AppendText(_T("G(%3d/%.3f/0x%02X)"), g, fg, g);
		m_pReport->SetColor(RGB(0, 0, 0));
		m_pReport->AppendText(_T("  "));
		// Show Blue
		m_pReport->SetColor(RGB(0, 0, fb * 255));
		m_pReport->AppendText(_L(BOX));
		m_pReport->SetColor(RGB(0, 0, 255));
		m_pReport->AppendText(_T("B(%3d/%.3f/0x%02X)"), b, fb, b);
		m_pReport->SetColor(RGB(0, 0, 0));
		m_pReport->AppendText(_T("  "));
		// Show Alpha
		{
			BYTE alpha = (BYTE)(fa * 255);
			m_pReport->SetColor(RGB(alpha, alpha, alpha));
		}
		m_pReport->AppendText(_L(BOX));
		m_pReport->SetColor(RGB(120, 120, 120));
		m_pReport->AppendText(_T("A(%3d/%.3f/0x%02X)"), a, fa, a);
		m_pReport->SetColor(RGB(0, 0, 0));
	}
	m_Wise.dwTime = 0;
}

BOOL VirtualDisplay::OnBufferBeforeFileOpen(DWORD dwID, LPCTSTR sFilename, DWORD dwWidth, DWORD dwHeight)
{
	if(!dwWidth || !dwHeight) {
		// custom load image...
		return FALSE;
	} else {
		if(dwWidth > 4096) dwWidth	= 4096;

		if(dwHeight > 4096) dwHeight	= 4096;

		m_pScreen->Create(dwWidth, dwHeight);
		m_pDoc->InvalidateLayout();
		m_pReport->SetText(_L(CURRENT_DISPLAY_RESOLUTION), dwWidth, dwHeight);
	}

	return TRUE;
}
