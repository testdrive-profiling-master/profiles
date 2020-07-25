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
#include "GeBlockWinKey.h"
#include "GePrimitive.h"
// 윈도우 키 입력을 차단합니다.

CGeBlockWinKey::CGeBlockWinKey()
{
	StartBlockKeyboard();
}

CGeBlockWinKey::~CGeBlockWinKey()
{
	StopBlockKeyboard();
}

HHOOK	g_hBlockKeyboardHookID = NULL;

LRESULT CALLBACK BlockKeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION) {
		switch(wParam) {
		case WM_KEYDOWN:
		case WM_KEYUP:
			if((!CGePrimitive::m_bWindow) & CGePrimitive::m_bActive)
				switch(*(DWORD*)lParam) {
				case VK_LWIN:
				case VK_RWIN:
				case VK_APPS:
				case 0x5F:	//VK_SLEEP
					return 1;
				}

			break;
		}
	}

	return CallNextHookEx(g_hBlockKeyboardHookID, nCode, wParam, lParam);
}

void CGeBlockWinKey::StartBlockKeyboard()
{
	StopBlockKeyboard();
	// 13은 WH_KEYBOARD_LL을 가리키며 로우레벨 키보드를 제어한다. 0은 시스템 훅킹을 의미
	g_hBlockKeyboardHookID = SetWindowsHookEx(13, (HOOKPROC)BlockKeyboardHook, GetModuleHandle(NULL), 0);
}

void CGeBlockWinKey::StopBlockKeyboard()
{
	if(g_hBlockKeyboardHookID) UnhookWindowsHookEx(g_hBlockKeyboardHookID);

	g_hBlockKeyboardHookID = NULL;
}
