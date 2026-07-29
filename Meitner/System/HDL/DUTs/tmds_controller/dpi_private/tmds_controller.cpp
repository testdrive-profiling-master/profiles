//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TMDS controller
// Rev.  : 7/29/2026 Wed (clonextop@gmail.com)
//================================================================================
#include "dpi_common.h"

typedef struct {
	int width, height;
} HDMI_DISPLAY;

static HDMI_DISPLAY __display;
static int			__frame_count = 0;

static uint8_t		tmds_decoding(uint32_t tmds_data)
{
	if (tmds_data & 0x200) { // DC Balance (Inversion flag (1 = Inverted, 0 = Normal))
		tmds_data ^= 0xFF;
	}

	if (tmds_data & 0x100) { // Reverse the Transition Minimization (Logic flag (1 = XOR, 0 = XNOR))
		// XOR mode
		tmds_data ^= (tmds_data << 1);
	} else {
		// XNOR mode
		tmds_data ^= (tmds_data << 1) ^ 0xF7;
	}

	return (uint8_t)tmds_data;
}

DPI_FUNCTION svBit tmds_out(const svBitVecVal *data_0, const svBitVecVal *data_1, const svBitVecVal *data_2)
{
	union {
		uint32_t m;
		struct {
			uint32_t hsync : 1;
			uint32_t vsync : 1;
			uint32_t de	   : 1;
		};
	} video_control;

	switch (*data_0) {
	case 0x354: // 1101010100 : C1(0), C0(0), DE(0)
		video_control.m = 0x0;
		break;
	case 0x0AB: // 0010101011 : C1(0), C0(1), DE(0)
		video_control.m = 0x1;
		break;
	case 0x154: // 0101010100 : C1(1), C0(0), DE(0)
		video_control.m = 0x2;
		break;
	case 0x2AB: // 1010101011 : C1(1), C0(1), DE(0)
		video_control.m = 0x3;
		break;
	default: // C1(1), C0(1), DE(1)
		video_control.m = 0x7;
	}

	static uint8_t		  prev_hsync	 = 0;
	static uint8_t		  prev_vsync	 = 0;
	static int			  x				 = 0;
	static int			  y				 = 0;
	static bool			  bSet			 = false;
	static DisplayConfig *pDisplayConfig = NULL;
	static DisplayColor	 *pBuffer		 = NULL;

	if (!pDisplayConfig) {
		pDisplayConfig				   = GetDisplayConfig();
		pBuffer						   = (DisplayColor *)GetMemoryPointer(GetMemoryBaseAddress(), 0, FALSE);
		pDisplayConfig->bReverse	   = FALSE;
		pDisplayConfig->ColorFormat	   = DISPLAY_FORMAT_ABGR_8888;
		__display.width				   = 640;
		__display.height			   = 480;
		pDisplayConfig->dwByteStride   = 0;
		pDisplayConfig->iWidth		   = __display.width;
		pDisplayConfig->iHeight		   = __display.height;
		pDisplayConfig->Back.dwAddress = 0;
		pDisplayConfig->Back.bUpdate   = TRUE;
		memset(pBuffer, 0, __display.width * __display.height * sizeof(DWORD));
	}

	if (!video_control.hsync && !video_control.vsync) {
		if (y) { // frame end
			__display.height			 = y;
			pDisplayConfig->iHeight		 = y;
			pDisplayConfig->Back.bUpdate = TRUE;
			__frame_count++;
		}

		x = 0;
		y = 0;
	} else {
		if (video_control.de) {
			uint8_t d0 = tmds_decoding(*data_0);
			uint8_t d1 = tmds_decoding(*data_1);
			uint8_t d2 = tmds_decoding(*data_2);

			if (pBuffer)
				pBuffer[x + y * __display.width].color = (d2 << 16) | (d1 << 8) | d0;

			bSet						 = true;
			pDisplayConfig->Back.bUpdate = TRUE;
			x++;
		}

		if (video_control.vsync && !video_control.hsync && prev_hsync) { // line end
			if (x && x != __display.width) {
				__display.width				 = x;
				pDisplayConfig->iWidth		 = x;
				pDisplayConfig->Back.bUpdate = TRUE;
			}

			if (bSet) {
				y++;
				bSet = false;
			}

			if (y > __display.height) {
				__display.height			 = y;
				pDisplayConfig->iHeight		 = y;
				pDisplayConfig->Back.bUpdate = TRUE;
			}

			x = 0;
		}
	}

	prev_hsync = video_control.hsync;
	prev_vsync = video_control.vsync;

	return (__frame_count > 1);
}
