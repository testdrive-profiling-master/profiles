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

	static HWND						m_hWndWindow;		// ������ �ڵ�
	static HWND						m_hWndFullscreen;	// Ǯ��ũ�� ������ �ڵ�
	static BOOL						m_bWindow;			// ���� ������ ��� ����
	static BOOL						m_bFullScreenOnly;	// Full Screen���� Ȱ��ȭ��.
	static BOOL						m_bActive;			// ��� Ȱ��ȭ
	static int						m_iScreenWidth;		// ȭ���� �ʺ�
	static int						m_iScreenHeight;	// ȭ���� ����
	static _D3DFORMAT				m_ColorFormat;		// Ǯ ��ũ���� ColorBits
protected:
	static LPDIRECT3D9				m_pD3D;				// D3D ���� ��ü
	static LPDIRECT3DDEVICE9		m_pd3dDevice;		// D3D ����̽� ��ü
	static D3DPRESENT_PARAMETERS	m_d3dpp;			// D3D ����̽� �Ķ����
	static BOOL						m_bDeviceLost;		// ����̽� ����

	static double					m_fElpasedTime;		// �����Ӱ� ��� �ð� (�� ����)
	static double					m_fProceedTime;		// ���� �ð� (�� ����)
	static DDK*						m_pDDK;				// ����̽� ����̹� Ŷ

	BOOL CheckTextureFormat(D3DFORMAT format, DWORD usage);
	static void UpdateTime(void);
};

int	FixtoTextureSize(int length);

#endif//__GE_PRIMITIVE_H__
