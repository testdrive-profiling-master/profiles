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
#ifndef __GE_PRIMITIVE_H__
#define __GE_PRIMITIVE_H__
#include "common.h"
#include <d3dx9.h>
#include <mmsystem.h>
#include <tchar.h>
#include <algorithm>
using namespace std;

class CGePrimitive  
{
public:
	CGePrimitive(void);
	virtual ~CGePrimitive(void);

	static void SetParam(HWND hWndWindow, HWND hWndFullscreen, DWORD full_screen_width, DWORD full_screen_height, DWORD full_screen_ColorBits, BOOL bFullScreenOnly, BOOL bStartFullScreen);

	static HWND						m_hWndWindow;		// 윈도우 핸들
	static HWND						m_hWndFullscreen;	// 풀스크린 윈도우 핸들
	static BOOL						m_bWindow;			// 현재 윈도우 모드 상태
	static BOOL						m_bFullScreenOnly;	// Full Screen만을 활성화함.
	static BOOL						m_bActive;			// 출력 활성화
	static int						m_iScreenWidth;		// 화면의 너비
	static int						m_iScreenHeight;	// 화면의 높이
	static _D3DFORMAT				m_ColorFormat;		// 풀 스크린시 ColorBits
protected:
	static LPDIRECT3D9				m_pD3D;				// D3D 생성 객체
	static LPDIRECT3DDEVICE9		m_pd3dDevice;		// D3D 디바이스 객체
	static D3DPRESENT_PARAMETERS	m_d3dpp;			// D3D 디바이스 파라메터
	static BOOL						m_bDeviceLost;		// 디바이스 잃음

	static double					m_fElpasedTime;		// 프레임간 경과 시간 (초 단위)
	static double					m_fProceedTime;		// 진행 시간 (초 단위)
	static DDK*						m_pDDK;				// 디바이스 드라이버 킷

	BOOL CheckTextureFormat(D3DFORMAT format, DWORD usage);
	static void UpdateTime(void);
};

int	FixtoTextureSize(int length);

#endif//__GE_PRIMITIVE_H__
