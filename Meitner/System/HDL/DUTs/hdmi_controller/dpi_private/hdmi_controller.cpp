//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : HDMI controller
// Rev.  : 3/8/2021 Mon (clonextop@gmail.com)
//================================================================================
#include "dpi_common.h"

typedef struct {
	int		width, height;
} HDMI_DISPLAY;

static HDMI_DISPLAY		__display;

DPI_FUNCTION void hdmi_out(unsigned char de, unsigned char hsync, unsigned char vsync, const svBitVecVal* data)
{
	static BYTE prev_hsync = 0;
	static BYTE prev_vsync = 0;
	static int x			= 0;
	static int y			= 0;
	static BOOL bSet		= FALSE;
	static DisplayConfig*	pDisplayConfig	= NULL;
	static DisplayColor*	pBuffer			= NULL;

	if(!pDisplayConfig) {
		pDisplayConfig	= GetDisplayConfig();
		pBuffer			= (DisplayColor*)GetMemoryPointer(GetMemoryBaseAddress(), 0, FALSE);
		pDisplayConfig->bReverse		= FALSE;
		pDisplayConfig->ColorFormat		= DISPLAY_FORMAT_ARGB_8888;
		__display.width					= 640;
		__display.height				= 480;
		pDisplayConfig->dwByteStride	= 0;
		pDisplayConfig->iWidth			= __display.width;
		pDisplayConfig->iHeight			= __display.height;
		pDisplayConfig->Back.dwAddress	= 0;
		pDisplayConfig->Back.bUpdate	= TRUE;
		memset(pBuffer, 0, __display.width * __display.height * sizeof(DWORD));
	}

	if(!hsync && !vsync) {
		if(y) {	// frame end
			__display.height				= y;
			pDisplayConfig->iHeight			= y;
			pDisplayConfig->Back.bUpdate	= TRUE;
		}

		x	= 0;
		y	= 0;
	} else {
		if(de && hsync && vsync) {
			if(pBuffer) pBuffer[x + y * __display.width].color	= *(DWORD*)data;

			bSet	= TRUE;
			pDisplayConfig->Back.bUpdate	= TRUE;
			x++;
		}

		if(vsync && !hsync && prev_hsync) {	// line end
			if(x && x != __display.width) {
				__display.width					= x;
				pDisplayConfig->iWidth			= x;
				pDisplayConfig->ColorFormat		= DISPLAY_FORMAT_ARGB_8888;
				pDisplayConfig->Back.bUpdate	= TRUE;
			}

			if(bSet) {
				y++;
				bSet	= FALSE;
			}

			if(y > __display.height) {
				__display.height				= y;
				pDisplayConfig->iHeight			= y;
				pDisplayConfig->ColorFormat		= DISPLAY_FORMAT_ARGB_8888;
				pDisplayConfig->Back.bUpdate	= TRUE;
			}

			x = 0;
		}
	}

	prev_hsync	= hsync;
	prev_vsync	= vsync;
}
