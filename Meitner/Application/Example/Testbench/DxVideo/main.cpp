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
#include "Ge.h"
#include <windows.h>
#include <windowsx.h>
CGe		GE;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GE.MsgProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;
	wc.hInstance		= hInstance;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wc.lpszClassName	= "D3DTestbench";
	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(NULL,
						  wc.lpszClassName,
						  "Testbench DirectX",
						  WS_OVERLAPPEDWINDOW,
						  0, 0,
						  1024, 576,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);
	HACCEL hAccelerators	= LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	ShowWindow(hWnd, nCmdShow);
	// enter the main loop:
	MSG msg;
	GE.SetParam(hWnd, NULL, 1920, 1080, 32, TRUE, TRUE);

	if(FAILED(GE.Create())) return 0;

	while(TRUE) {
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(!TranslateAccelerator(msg.hwnd, hAccelerators, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if(msg.message == WM_QUIT)
			break;

		GE.Render3DEnvironment();
	}

	GE.ReleaseAll();
	return msg.wParam;
}


