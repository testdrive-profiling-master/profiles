//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Global system configuration
// Rev.  : 12/31/2021 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_CONFIG_VIRTUAL_FPGA_STARTER_KIT_H__
#define __SYSTEM_CONFIG_VIRTUAL_FPGA_STARTER_KIT_H__
#include "SystemConfig.h"

#define	SYSTEM_MAGIC_CODE					0x54494B53	// System magic code : "SKIT"

typedef struct REGMAP_LED {
	bool	bUpdate;
	float	val[9];		// 0 ~ 1.0 (max)
};

typedef struct REGMAP_NUMERIC_DISPLAY {
	bool	bUpdate;
	struct {
		float	segment[8];		// A~G, DP
	} num[4];
	float	mid;
};

#define	TFT_LCD_DISPLAY_WIDTH		480
#define	TFT_LCD_DISPLAY_HEIGHT		272
typedef struct REGMAP_TFT_LCD_DISPLAY {
	bool	bUpdate;
	DWORD	buff[TFT_LCD_DISPLAY_WIDTH * TFT_LCD_DISPLAY_HEIGHT];
};

typedef struct : public SYSTEM_REGMAP {
	REGMAP_LED					led;
	REGMAP_NUMERIC_DISPLAY		numeric_display;
	DWORD						buttons;			// active low button
	DWORD						switches;
	REGMAP_TFT_LCD_DISPLAY		tft_lcd_display;
} STARTERKIT_REGMAP;

#endif//__SYSTEM_CONFIG_VIRTUAL_FPGA_STARTER_KIT_H__
