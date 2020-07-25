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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "GePrimitive.h"

//-------------------------------------- 공용변수 --------------------------------------
#define DEFAULT_SCREEN_WIDTH	800
#define DEFAULT_SCREEN_HEIGHT	600

HWND		CGePrimitive::m_hWndWindow			= NULL;						// 윈도우 핸들
HWND		CGePrimitive::m_hWndFullscreen		= NULL;						// 풀스크린 윈도우 핸들
BOOL		CGePrimitive::m_bWindow				= FALSE;					// 현재 윈도우 모드 상태
BOOL		CGePrimitive::m_bFullScreenOnly		= TRUE;						// 풀스크린 전용 구동 활성화
BOOL		CGePrimitive::m_bActive				= FALSE;					// 출력 활성화
BOOL		CGePrimitive::m_bDeviceLost			= FALSE;					// 디바이스 잃음
int			CGePrimitive::m_iScreenWidth		= DEFAULT_SCREEN_WIDTH;		// 화면의 너비
int			CGePrimitive::m_iScreenHeight		= DEFAULT_SCREEN_HEIGHT;	// 화면의 높이

LPDIRECT3D9				CGePrimitive::m_pD3D		= NULL;		// D3D 생성 객체
LPDIRECT3DDEVICE9		CGePrimitive::m_pd3dDevice	= NULL;		// D3D 디바이스 객체
D3DPRESENT_PARAMETERS	CGePrimitive::m_d3dpp		= {			// D3D 디바이스 파라메터
	DEFAULT_SCREEN_WIDTH,	// BackBufferWidth
	DEFAULT_SCREEN_HEIGHT,	// BackBufferHeight
	D3DFMT_X8R8G8B8,		// BackBufferFormat
	NULL,					// BackBufferCount

	D3DMULTISAMPLE_NONE,	// MultiSampleType
	NULL,					// MultiSampleQuality

	D3DSWAPEFFECT_DISCARD,	// SwapEffect
	NULL,					// hDeviceWindow
	FALSE,					// Windowed
	FALSE,					// EnableAutoDepthStencil
	D3DFMT_D16,				// AutoDepthStencilFormat
	NULL,					// Flags

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	0,						// FullScreen_RefreshRateInHz
	0						// PresentationInterval
};

_D3DFORMAT	CGePrimitive::m_ColorFormat	= D3DFMT_X8R8G8B8;			// 풀 스크린시 ColorBits

double		CGePrimitive::m_fElpasedTime	= 0;			// 프레임간 경과 시간
double		CGePrimitive::m_fProceedTime	= 0;			// 진행 시간
DDK*		CGePrimitive::m_pDDK			= NULL;			// 디바이스 드라이버 킷

CGePrimitive::CGePrimitive(void) {}
CGePrimitive::~CGePrimitive(void) {}

void CGePrimitive::SetParam(HWND hWndWindow, HWND hWndFullscreen, DWORD screen_width, DWORD screen_height, DWORD full_screen_ColorBits, BOOL bFullScreenOnly, BOOL bStartFullScreen)
{
	m_hWndWindow			= hWndWindow;
	m_hWndFullscreen		= hWndFullscreen;
	m_iScreenWidth			= screen_width;
	m_iScreenHeight			= screen_height;
	m_ColorFormat			= (full_screen_ColorBits == 16 ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8);
	m_bFullScreenOnly		= bFullScreenOnly;
	m_bWindow				= (bFullScreenOnly ? FALSE : !bStartFullScreen);
}

BOOL CGePrimitive::CheckTextureFormat(D3DFORMAT format, DWORD usage)
{
	return m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_ColorFormat, usage, D3DRTYPE_TEXTURE, format) == D3D_OK;
}

void CGePrimitive::UpdateTime(void)
{
	static BOOL		bInitialize = FALSE;
	static BOOL		bUsingQPF;
	static DWORD	dwPrevTime, dwCurTime;
	static LONGLONG llLastElapsedTime = 0;
	static LONGLONG llQPFTicksPerSec  = 0;
	LARGE_INTEGER qwTime;
	double fElapsedTime;

	if(!bInitialize) {
		bInitialize = TRUE;
		dwPrevTime = timeGetTime();
		bUsingQPF = QueryPerformanceFrequency(&qwTime);

		if(bUsingQPF) llQPFTicksPerSec = qwTime.QuadPart;

		QueryPerformanceCounter(&qwTime);
		llLastElapsedTime = qwTime.QuadPart;
	}

	dwCurTime = timeGetTime();

	if(bUsingQPF) {
		QueryPerformanceCounter(&qwTime);
		m_fElpasedTime = (double)(qwTime.QuadPart - llLastElapsedTime) / (double) llQPFTicksPerSec;
		llLastElapsedTime = qwTime.QuadPart;
	} else {
		m_fElpasedTime = (double)(dwCurTime - dwPrevTime) * 0.001;
	}

	m_fProceedTime += m_fElpasedTime;
	dwPrevTime = dwCurTime;
}

int	FixtoTextureSize(int length)
{
	int i, cmp = 1;

	for(i = 0; i < 15; i++) {
		if(length <= cmp) break;

		cmp <<= 1;
	}

	return cmp;
}
