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
// Title : Testbench
// Rev.  : 8/7/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "Ge.h"
#include "GeUtil.h"
#include "TestDriver.inl"

static CGe *__pGE = NULL;

void		SysLog(DWORD dwColor, const char *sFormat, ...)
{
	char	sMsg[1024];
	va_list ap;
	va_start(ap, sFormat);
	vsprintf(sMsg, sFormat, ap);
	va_end(ap);

	if (__pGE)
		__pGE->Log().SetMessage(sMsg, dwColor, 4000);
}

CGe::CGe(void)
{
	m_bDeviceObjectsRestored = FALSE;
	m_bDeviceObjectsInited	 = FALSE;
	m_pFont					 = new CGeFont(_T("Arial"), 14, D3DFONT_BOLD | D3DFONT_FILTERED);
	m_pRankFont =
		new CGeFont(_T("Alien Encounters Solid"), 18, D3DFONT_BOLD | D3DFONT_FILTERED); // ref. (https://www.dafont.com/alien-encounters.font)
	m_pResultFont = new CGeFont(_T("Alien Encounters Solid"), 14, D3DFONT_BOLD | D3DFONT_FILTERED);
	D3DXMatrixIdentity(&m_matIdentity);
	m_matW			  = m_matIdentity;
	m_matIW			  = m_matW;
	m_matIW._22		  = -m_matW._22;
	m_fProcessingTime = 0;
	m_fDNNMax		  = 0;
	m_bFound		  = FALSE;
	m_DNNType		  = DNN_TYPE_1;
	m_fChangeImpact	  = 0;
	{
		int t = 0;

		for (int i = 0; i < 128; i++) { // black to blue
			DWORD factor		 = 2 * i;
			m_ColorBarTable[t++] = 0xFF000000 | factor;
		}

		for (int i = 0; i < 128; i++) { // blue to cyan
			DWORD factor		 = 2 * i;
			m_ColorBarTable[t++] = 0xFF0000FF | (factor << 8);
		}

		for (int i = 0; i < 128; i++) { // cyan to green
			DWORD factor		 = 255 - 2 * i;
			m_ColorBarTable[t++] = 0xFF00FF00 | factor;
		}

		for (int i = 0; i < 128; i++) { // green to yellow
			DWORD factor		 = 2 * i;
			m_ColorBarTable[t++] = 0xFF00FF00 | (factor << 16);
		}

		for (int i = 0; i < 512; i++) { // yellow to red
			DWORD factor		 = (511 - i) >> 1;
			m_ColorBarTable[t++] = 0xFFFF0000 | (factor << 8);
		}
	}
	__pGE = this;
}

CGe::~CGe(void)
{
	__pGE = NULL;
	ReleaseAll();
}

//------------------------------------- �������̽� -------------------------------------
//--------------------------------------------
//	Create : ����.
//--------------------------------------------
HRESULT CGe::Create(void)
{
	m_pDDK = CreateDDK();

	if (!m_pDDK) {
		MessageBox(NULL, "Driver is not initialized!", "DDK Error", MB_ICONHAND | MB_OK);
		return E_FAIL;
	}

	if (m_pDDK) {
		const char *sDesc = m_pDDK->GetSystemDescription();
		SysLog(0xFFFFFF, "Current system is '%s'", sDesc);

		if (strstr(sDesc, "Simplified"))
			SysLog(0xFF3F3F, "    - Warning : Maybe, this 'Simplified' system is not workable...");
	}

	//--------------------------------------------------------------------
	// 3D ������Ʈ �ʱ�ȭ
	//
	m_bActive = FALSE;
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	// ������ ������ ũ�⸦ �����Ѵ�.
	GetWindowRect(m_hWndWindow, &m_rcWindowBounds);

	// D3D ��ü ����
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// D3D ����̽� �Ķ���͸� �����մϴ�.
	m_d3dpp.Windowed		 = m_bWindow;
	m_d3dpp.BackBufferWidth	 = m_iScreenWidth;
	m_d3dpp.BackBufferHeight = m_iScreenHeight;
	m_d3dpp.BackBufferFormat = m_bWindow ? D3DFMT_UNKNOWN : m_ColorFormat;

	// Create the D3DDevice
	if (!m_hWndFullscreen)
		m_hWndFullscreen = m_hWndWindow;

	if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWndFullscreen, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice))) {
		return E_FAIL;
	}

	g_pd3dDevice = m_pd3dDevice;

	if (FAILED(InitDeviceObjects())) {
		DeleteDeviceObjects();
		return E_FAIL;
	} else if (FAILED(RestoreDeviceObjects())) {
		InvalidateDeviceObjects();
		return E_FAIL;
	}

	if (m_bWindow) {
		SetWindowPos(
			m_hWndFullscreen, HWND_NOTOPMOST, m_rcWindowBounds.left, m_rcWindowBounds.top, (m_rcWindowBounds.right - m_rcWindowBounds.left),
			(m_rcWindowBounds.bottom - m_rcWindowBounds.top), SWP_SHOWWINDOW);
	} else {
	}

	m_bActive = TRUE;
	return S_OK;
}
//--------------------------------------------
//	ReleaseAll : ����.
//--------------------------------------------
void CGe::ReleaseAll()
{
	m_bActive = FALSE;
	InvalidateDeviceObjects();
	DeleteDeviceObjects();

	for (int i = 0; i < STATE_BLOCK_SIZE; i++) m_StateBlock[i].ReleaseAll();

	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pRankFont);
	SAFE_DELETE(m_pResultFont);
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pDDK);
}

//--------------------------------------------
//	3DEnvironment : ȭ�� ���.
//--------------------------------------------
HRESULT CGe::Render3DEnvironment(void)
{
	HRESULT hr;

	if (!Main())
		return E_FAIL;

	if (!m_bActive)
		return S_OK;

	if (m_bDeviceLost) { // ������ ����̽��� �Ҿ������ ������ �ʿ��� �� ����.
		if (FAILED(hr = m_pd3dDevice->TestCooperativeLevel())) {
			if (D3DERR_DEVICELOST == hr) // ����̽��� �� �Ҿ���ȴ�. �����ϴµ� �ణ�� �ð��� �ʿ��� �� �ֽ��ϴ�.
				return S_OK;

			// ����̽� ����
			if (D3DERR_DEVICENOTRESET == hr) {
				if (FAILED(hr = Reset3DEnvironment()))
					return hr;
			}

			return hr;
		}

		m_bDeviceLost = FALSE; // �ذ����.
	} else {
		m_imCamera.Lock();

		if (FAILED(hr = Render())) {
			m_imCamera.Unlock();
			return hr;
		}

		hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL); // ȭ�鿡 ���

		if (D3DERR_DEVICELOST == hr) // ����̽��� �Ҿ����.
			m_bDeviceLost = TRUE;

		m_imCamera.Unlock();
	}

	return S_OK;
}

//--------------------------------------- ������ ---------------------------------------
//--------------------------------------------
//	MsgProc : �޽��� ���ν���.
//--------------------------------------------
LRESULT CGe::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_ACTIVATE: // ALT + TAB ���� �������ų� ���콺�� Ȱ��&��Ȱ��ȭ ��ų���
		if (m_bFullScreenOnly)
			m_bActive = (wParam & (WA_ACTIVE | WA_CLICKACTIVE)) != 0;

		// if(!m_bActive) ShowCursor(TRUE);
		// else ShowCursor(FALSE);
		break;

	case WM_SETCURSOR:

		// Ǯ��ũ���� ���Ƿ� �����찡 �𷡽ð� Ŀ���� ���̰� �ϴ� ����
		// �����ݴϴ�.
		if (m_bActive && !m_bWindow) {
			SetCursor(NULL);

			if (m_pd3dDevice)
				m_pd3dDevice->ShowCursor(FALSE);
		}

		return true; // �⺻ ���ν����� Ŀ�������� ���´�.

	case WM_NCHITTEST:

		// �޴��� ����� ��� ��üȭ�鿡�� �޴��� ������ �� �ִ� ����
		// ����.
		if (!m_bWindow)
			return HTCLIENT;

		break;

	case WM_POWERBROADCAST:
		switch (wParam) {
#ifndef PBT_APMQUERYSUSPEND
#	define PBT_APMQUERYSUSPEND 0x0000
#endif

		case PBT_APMQUERYSUSPEND:
			// �ý����� ������� ��忡 ���� �����Ѵ�.
			// ��Ʈ�� �����̴ٸ��� ���� �����̶���� ������� ��带 �غ��ؾ� �Ѵ�.
			return true;
#ifndef PBT_APMRESUMESUSPEND
#	define PBT_APMRESUMESUSPEND 0x0007
#endif

		case PBT_APMRESUMESUSPEND:
			// ������� ��忡�� ���� �Ǿ���.
			// ���� �غ� �ϵ��� ���Ḧ �ϵ���... ���� �ؾ��Ѵ�.--;;; �� ���� �̷� ��� ������.
			return true;
		}

		break;

	case WM_SYSCOMMAND:

		// Ǯ��ũ���� ��� ������ ������ ����, �̵�, ���� ��� ������ ���� �ʴ´�.
		switch (wParam) {
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_KEYMENU:
		case SC_MONITORPOWER:
			if (!m_bWindow)
				return 1;

			break;
		}

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_TOGGLE_FULLSCREEN: // "ALT + ENTER" �� �����Ѵ�. ������ ���� Ǯ��Ű�� ��带 ����.
			if (!m_bFullScreenOnly)
				ToggleFullScreen();
			else {
				m_bActive = FALSE;
				PostMessage(m_hWndFullscreen, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
			}

			return 0;

		case IDC_CAPTURE_SCREEN: // "F12" ��ũ���� Ű
			CaptureScreenShot();
			return 0;

		case IDC_EXIT: // "ESC" ���α׷� ����.
			PostMessage(m_hWndWindow, WM_DESTROY, 0, 0);
			return 0;
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------------------------
//	Reset3DEnvironment : ����̽��� ���� ���,
//  �Ǵ� 3Dȯ���� �ٲ� ��� ȣ��ǰ� �˴ϴ�.
//--------------------------------------------
HRESULT CGe::Reset3DEnvironment()
{
	HRESULT hr;
	// ���� �޸��� ��� ��ġ�� �����մϴ�.
	InvalidateDeviceObjects();

	// ��ġ�� �����մϴ�.
	if (FAILED(hr = m_pd3dDevice->Reset(&m_d3dpp)))
		return hr;

	// ��ġ�� �����մϴ�.
	hr = RestoreDeviceObjects();

	if (FAILED(hr)) {
		InvalidateDeviceObjects();
		return hr;
	}

	return S_OK;
}

//--------------------------------------------
//	ToggleFullScreen : â���� Ǯ��ũ�� ��带
//  ��� ��ȯ�ϰ� �˴ϴ�.
//--------------------------------------------
HRESULT CGe::ToggleFullScreen(BOOL bToggle)
{
	HRESULT hr = S_OK;

	if (!m_pd3dDevice)
		return E_FAIL;

	if (bToggle) {
		SysLog(0xFFFFFF, "Toggle FullScreen");
		m_bWindow = !m_bWindow;
	}

	if (m_bWindow) {
		SetWindowPos(
			m_hWndFullscreen, HWND_NOTOPMOST, m_rcWindowBounds.left, m_rcWindowBounds.top, (m_rcWindowBounds.right - m_rcWindowBounds.left),
			(m_rcWindowBounds.bottom - m_rcWindowBounds.top), SWP_SHOWWINDOW);
	} else {
		if (IsIconic(m_hWndFullscreen))
			ShowWindow(m_hWndFullscreen, SW_RESTORE);

		ShowWindow(m_hWndFullscreen, SW_SHOW);
		GetWindowRect(m_hWndWindow, &m_rcWindowBounds);
	}

	if (bToggle) {
		m_bActive				 = FALSE;
		m_d3dpp.Windowed		 = m_bWindow;
		m_d3dpp.BackBufferWidth	 = m_iScreenWidth;
		m_d3dpp.BackBufferHeight = m_iScreenHeight;
		m_d3dpp.BackBufferFormat = m_bWindow ? D3DFMT_UNKNOWN : m_ColorFormat;
		hr						 = Reset3DEnvironment();
	}

	ShowCursor(m_bWindow);

	if (m_pd3dDevice)
		m_pd3dDevice->ShowCursor(m_bWindow);

	m_bActive = TRUE;
	return hr;
}

//--------------------------------------------
//	CaptureScreenShot : ȭ���� ĸ���Ͽ� ���Ϸ� �����մϴ�.
//  ��ũ���� ��� SHOT(XXXX).png
//--------------------------------------------
const char *SCREENSHOT_DIR	= "ScreenShot";
const char *SCREENSHOT_NAME = "ScreenShot\\SHOT%04d.png";
HRESULT		CGe::CaptureScreenShot(void)
{
	static DWORD dwShotCount = 0;

	if (!m_pd3dDevice)
		return E_FAIL;

	char filename[32];

	// "ScreenShot" ���丮 ���� ���� Ȯ��
	switch (GetFileAttributes(SCREENSHOT_DIR)) {
	case FILE_ATTRIBUTE_DIRECTORY:
		break;

	case INVALID_FILE_ATTRIBUTES:
		CreateDirectory(SCREENSHOT_DIR, NULL);

		if (GetFileAttributes(SCREENSHOT_DIR) != FILE_ATTRIBUTE_DIRECTORY)
			return E_FAIL;

		break;

	default:
		return E_FAIL;
	}

	// ���� �̸� �ߺ� ����
	do {
		dwShotCount++;
		wsprintf(filename, SCREENSHOT_NAME, dwShotCount);
	} while (GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES);

	// PNG ���Ϸ� ��ũ���� ����
	IDirect3DSurface9 *pSurface = NULL;
	m_imCamera.Lock();
	m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface); // Back Surface�� ���ͼ�...
	D3DXSaveSurfaceToFile(filename, D3DXIFF_PNG, pSurface, NULL, NULL);	   // ������
	SAFE_RELEASE(pSurface);												   // �޸� ���� ����
	SysLog(0xFFFFFF, "Capture Image to '%s'...", filename);
	m_imCamera.Unlock();
	return S_OK;
}

//--------------------------------------------
//	InitDeviceObjects : ���� �׷��� ��ġ��
//  �ʱ�ȭ�մϴ�.
//--------------------------------------------
HRESULT CGe::InitDeviceObjects(void)
{
	if (m_bDeviceObjectsInited)
		return S_OK;

	m_bDeviceObjectsInited = TRUE;
	m_pFont->InitDeviceObjects();
	m_pRankFont->InitDeviceObjects();
	m_pResultFont->InitDeviceObjects();
	m_imTitle.LoadImage("media\\DxVideo\\title.png");
	{
		const char *_sNode[] = {"Net1", "Net2"};

		for (int i = 0; i < DNN_TYPE_SIZE; i++) {
			char sPath[1024];
			sprintf(sPath, "media\\DxVideo\\%s\\pannel.png", _sNode[i]);
			m_imSidePannel[i].LoadImage(sPath);
			m_imSidePannel[i].SetUpDraw(SCREEN_POINT_RT, -m_imSidePannel[i].Width(), 0);
		}
	}
	m_imTarget.LoadImage("media\\DxVideo\\target.png");
	m_imTargets.LoadImage("media\\DxVideo\\target_s.png");
	m_imWhiteBox.LoadImage("media\\DxVideo\\white.png");
	m_imBoard.LoadImage("media\\DxVideo\\board.png");
	m_imPreview.LoadImage("media\\DxVideo\\preview.png");
	m_imRedLight.LoadImage("media\\DxVideo\\red_light.png");
	m_imTitle.SetUpDraw(SCREEN_POINT_LT, 0, 0);
	m_imTarget.SetUpDraw(SCREEN_POINT_LT, 0, 40);
	m_imTargets.SetUpDraw(SCREEN_POINT_LT, 0, 40);
	m_imWhiteBox.SetUpDraw(SCREEN_POINT_LT, 0, 0);
	m_imBoard.SetUpDraw(SCREEN_POINT_RD, -880, -330);
	m_imPreview.SetUpDraw(SCREEN_POINT_RD, -380, -270);
	m_imRedLight.SetUpDraw(SCREEN_POINT_RD, -396, -297);

	if (m_imCamera.Create(1920, 1080) || m_imCamera.Create(1280, 720) || m_imCamera.Create(640, 480)) {
		m_imCamera.RegisterPost(this);
		SysLog(0xFFFFFF, "Camera is initialized...");
	} else
		SysLog(0xFF3F3F, "Camera initialization is failed!");

	m_imFrameFullScreen.InitVertexBuffer();
	m_imFrameFullScreen.SetUpDraw(SCREEN_POINT_LD, 0, 0, m_iScreenWidth, -m_iScreenHeight);
	m_imFrameProcess.InitVertexBuffer();
	m_imFrameProcess.SetUpDraw(SCREEN_POINT_RD, -367, -38, 321, -181);
	return S_OK;
}

//--------------------------------------------
//	RestoreDeviceObjects : ���� �޸� ������
//  �ٽ� ������ �ݴϴ�.
//  ���� ���� �޸𸮴� ���� ���ư��� �ʽ��ϴ�.
//  ���� �ַ� RenderState������ ������ ���¸�
//  �ٽ� ������ �ֽø� �˴ϴ�.
//--------------------------------------------
HRESULT CGe::RestoreDeviceObjects(void)
{
	if (m_bDeviceObjectsRestored)
		return S_OK;

	m_bDeviceObjectsRestored = TRUE;
	// Set the projection matrix
	D3DXMATRIX	matView;
	D3DXVECTOR3 vFromPt	  = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec	  = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, (float)m_iScreenWidth, (float)m_iScreenHeight, 0.0001f, 100.f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	m_pFont->RestoreDeviceObjects();
	m_pRankFont->RestoreDeviceObjects();
	m_pResultFont->RestoreDeviceObjects();
	m_imFrameFullScreen.SetUpDraw(SCREEN_POINT_LD, 0, 0, m_iScreenWidth, -m_iScreenHeight);
	m_imFrameProcess.Restore();
	m_imTitle.Restore();
	m_imWhiteBox.Restore();
	m_imBoard.Restore();
	m_imPreview.Restore();
	m_imRedLight.Restore();

	for (int i = 0; i < DNN_TYPE_SIZE; i++) m_imSidePannel[i].Restore();

	m_imTarget.Restore();
	m_imTargets.Restore();
	// state blocks
	{
		// for background image (linear only)
		m_StateBlock[STATE_BLOCK_BACKGROUND].Begin();
		m_StateBlock[STATE_BLOCK_BACKGROUND].SetTextureFilter(STATE_TEXTURE_FILTER_LINEAR);
		m_StateBlock[STATE_BLOCK_BACKGROUND].End();
		// for image (with alpha)
		m_StateBlock[STATE_BLOCK_IMAGE].Begin();
		m_StateBlock[STATE_BLOCK_IMAGE].SetAlphaBlend(STATE_ALPHA_BLEND_NORMAL);
		m_StateBlock[STATE_BLOCK_IMAGE].End();
		// for image (with alpha & blend)
		m_StateBlock[STATE_BLOCK_IMAGE_BLNED].Begin();
		m_StateBlock[STATE_BLOCK_IMAGE_BLNED].SetAlphaBlend(STATE_ALPHA_BLEND_NORMAL);
		m_StateBlock[STATE_BLOCK_IMAGE_BLNED].SetTexture(0, STATE_TEXTURE_MODULATE_A);
		m_StateBlock[STATE_BLOCK_IMAGE_BLNED].End();
		// for image (with alpha/color & blend)
		m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].Begin();
		m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].SetAlphaBlend(STATE_ALPHA_BLEND_NORMAL);
		m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].SetTexture(0, STATE_TEXTURE_MODULATE_ARGB);
		m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].End();
		// for light
		m_StateBlock[STATE_BLOCK_LIGHT].Begin();
		m_StateBlock[STATE_BLOCK_LIGHT].SetAlphaBlend(STATE_ALPHA_BLEND_LIGHT);
		m_StateBlock[STATE_BLOCK_LIGHT].End();
		// for light (with blend)
		m_StateBlock[STATE_BLOCK_LIGHT_BLEND].Begin();
		m_StateBlock[STATE_BLOCK_LIGHT_BLEND].SetAlphaBlend(STATE_ALPHA_BLEND_LIGHT);
		m_StateBlock[STATE_BLOCK_LIGHT_BLEND].SetTexture(0, STATE_TEXTURE_MODULATE_A);
		m_StateBlock[STATE_BLOCK_LIGHT_BLEND].End();
		// for camera
		m_StateBlock[STATE_BLOCK_CAMERA].Begin();
		m_StateBlock[STATE_BLOCK_CAMERA].SetTextureFilter(STATE_TEXTURE_FILTER_HIGH);
		m_StateBlock[STATE_BLOCK_IMAGE].SetAlphaBlend(STATE_ALPHA_BLEND_NORMAL);
		m_StateBlock[STATE_BLOCK_CAMERA].End();
	}
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matIdentity);
	m_StateBlock[STATE_BLOCK_BACKGROUND].Apply();
	return S_OK;
}

//--------------------------------------------
//	InvalidateDeviceObjects : ����̽� ��ġ��
//  �Ҿ������ ��, ���� �޸𸮸� ����ϴ�
//  ��ġ�� �����մϴ�.
//	���� : Managed pool�� �����ϸ� �ؽ��ĳ�
//  ���ؽ� ������ �Ҿ������ �ʽ��ϴ�.
//  ���� ���� ������ �ʿ�� �����ϴ�.
//  �ػ󵵸� �ٲٰų� ����̽� ��ü�� �ٲܶ���
//  ���� �ٽ� �޸𸮸� �ø��� �۾��� �ʿ��մϴ�.
//--------------------------------------------
HRESULT CGe::InvalidateDeviceObjects(void)
{
	if (!m_bDeviceObjectsRestored)
		return S_OK;

	m_bDeviceObjectsRestored = FALSE;

	if (m_pFont)
		m_pFont->InvalidateDeviceObjects();

	if (m_pRankFont)
		m_pRankFont->InitDeviceObjects();

	if (m_pResultFont)
		m_pResultFont->InvalidateDeviceObjects();

	return S_OK;
}

//--------------------------------------------
//	DeleteDeviceObjects : �׷��� ��ġ�� ���
//  �޸𸮿��� �����մϴ�.
//--------------------------------------------
HRESULT CGe::DeleteDeviceObjects(void)
{
	if (!m_bDeviceObjectsInited)
		return S_OK;

	m_bDeviceObjectsInited = FALSE;
	m_pFont->DeleteDeviceObjects();
	m_pRankFont->DeleteDeviceObjects();
	m_pResultFont->DeleteDeviceObjects();
	m_imFrameFullScreen.ReleaseAll();
	m_imFrameProcess.ReleaseAll();
	m_imCamera.ReleaseAll();
	m_imTitle.ReleaseAll();
	m_imWhiteBox.ReleaseAll();
	m_imBoard.ReleaseAll();
	m_imPreview.ReleaseAll();
	m_imRedLight.ReleaseAll();

	for (int i = 0; i < DNN_TYPE_SIZE; i++) m_imSidePannel[i].ReleaseAll();

	m_imTarget.ReleaseAll();
	m_imTargets.ReleaseAll();
	return S_OK;
}

BOOL CGe::Main(void)
{
	UpdateTime();
	m_cSystemMsg.UpdateTime(m_fElpasedTime * 1000.f);
	return TRUE;
}

HRESULT CGe::Render(void)
{
	// Clear the viewport
	// m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 127), 1.0f, 0);
	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 127), 1.0f, 0L); // no 'Z' buffer

	if (SUCCEEDED(m_pd3dDevice->BeginScene())) {
		// �ΰ� ���
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matIdentity);
		m_StateBlock[STATE_BLOCK_BACKGROUND].Apply();

		if (m_imCamera.IsInitialized())
			m_imFrameFullScreen.Present(m_imCamera.Texture());

		m_StateBlock[STATE_BLOCK_IMAGE].Apply();
		m_imTitle.Present();
		m_imTarget.Present();
		m_imPreview.Present();
		m_imBoard.Present();

		if (m_imCamera.IsInitialized()) {
			// camera online... preview image
			m_StateBlock[STATE_BLOCK_CAMERA].Apply();
			m_imFrameProcess.Present(m_imCamera.Texture());
			// blinking red light
			DWORD dwBlink = DWORD(m_fProceedTime * 1000.f);

			if (dwBlink & 0x100)
				dwBlink ^= 0xFF;

			m_StateBlock[STATE_BLOCK_LIGHT_BLEND].Apply();
			m_StateBlock[STATE_BLOCK_LIGHT_BLEND].SetTextureFactor((dwBlink) << 24);

			if (m_bFound)
				m_imTargets.Present();

			{
				static DWORD dwTime = 0;
				DWORD		 dwFactor;
				dwFactor = (dwTime += 6);

				if (dwFactor & 0x100)
					dwFactor ^= 0xFF;

				dwFactor &= 0xFF;
				m_StateBlock[STATE_BLOCK_LIGHT_BLEND].Apply();
				m_StateBlock[STATE_BLOCK_LIGHT_BLEND].SetTextureFactor((dwFactor) << 24);
			}
			m_imRedLight.Present();
			m_bFound = TRUE; // for test
		}

		// �ý��� �޽��� ���
		{
			int i	  = m_iScreenHeight - 132 - 8;
			int Index = m_cSystemMsg.GetSize();

			while (Index) {
				Index--;
				const char *msgline = m_cSystemMsg.GetMessage(Index);

				if (msgline) {
					m_pFont->DrawText(117 + 1, i + 1, 0xFF000000, msgline);
					m_pFont->DrawText(117, i, m_cSystemMsg.GetColor(Index), msgline);
				}

				i -= 20;
			}
		}
		{
			// result out.
			static float fScore = 0; //@TODO: for test. delete this...

			if ((fScore++) > 100) {
				fScore = 0;
			}

			{
				// show bar graph.
				m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].Apply();

				for (int i = 0; i < 5; i++) {
					float width = (574 * fScore) / 100.f;
					m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].SetTextureFactor(m_ColorBarTable[(int)(fScore * (1023.f / 100.f))]);
					m_imWhiteBox.SetUpDraw(SCREEN_POINT_LT, 1174, 225 + i * 88, width, 9);
					m_imWhiteBox.Present();
				}
			}

			m_StateBlock[STATE_BLOCK_IMAGE_BLNED].Apply();
			m_imSidePannel[m_DNNType].Present();

			if (m_fChangeImpact) {
				DWORD dwFactor = (DWORD)(m_fChangeImpact * 255);
				m_StateBlock[STATE_BLOCK_LIGHT_BLEND].Apply();
				m_StateBlock[STATE_BLOCK_IMAGE_MODULATE].SetTextureFactor(dwFactor << 24);
				m_imSidePannel[m_DNNType].Present();
				m_fChangeImpact -= m_fElpasedTime;

				if (m_fChangeImpact < 0)
					m_fChangeImpact = 0;
			}

			{
				// show label & score
				char sMsg[1024];

				for (int i = 0; i < 5; i++) {
					sprintf(sMsg, "Test Label #1", i);
					m_pRankFont->DrawText(1143, 252 + i * 88, 0xFFFFFFFF, sMsg);
					sprintf(sMsg, "%.2f %%", fScore);
					m_pFont->DrawText(1762, 219 + i * 88, 0xFFFFFFFF, sMsg);
				}
			}
		}
		{
			// show performance
			char sMsg[1024];
			m_StateBlock[STATE_BLOCK_IMAGE].Apply();
			sprintf(
				sMsg, "Elpased Time : %.2f msec \nPerformance : %3.1f fps", m_fProcessingTime * 1000.f,
				m_fProcessingTime ? (1.f / m_fProcessingTime) : 0.f);
			m_pResultFont->DrawText(1065 + 3, 664 + 3, 0xFF000000, sMsg);
			m_pResultFont->DrawText(1065, 664, 0xFFFFFFFF, sMsg);
		}
		m_pd3dDevice->EndScene();
	}

	return S_OK;
}
