//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Template design
// Rev.  : 1/2/2022 Sun (clonextop@gmail.com)
//================================================================================
#include "StarterKit.h"

static StarterKit		__starter_kit;

StarterKit::StarterKit(void)
{
	m_pReg				= NULL;
}

StarterKit::~StarterKit(void)
{
}

void StarterKit::Initialize(void)
{
	srand(time(NULL));			// randomize seed
	m_pReg				= (STARTERKIT_REGMAP*)GET_SYSTEM_REGMAP;
	m_pReg->magic_code	= SYSTEM_MAGIC_CODE;
	memset(&m_pReg->led, 0, sizeof(m_pReg->led));
	m_pReg->led.val[8]	= 32;
	m_pReg->buttons		= 0xFFFFFFFF;
	m_pReg->led.bUpdate	= true;
	memset(m_pReg->tft_lcd_display.buffer.front, 0, sizeof(m_pReg->tft_lcd_display.buffer.front));

	// randomize screen
	for(int y = 0; y < TFT_LCD_DISPLAY_HEIGHT; y++)
		for(int x = 0; x < TFT_LCD_DISPLAY_WIDTH; x++) {
			DWORD	dwColor	= (rand() & 0xFF) | ((rand() & 0xFF)) << 8 | ((rand() & 0xFF) << 16);
			m_pReg->tft_lcd_display.buffer.front[x + y * TFT_LCD_DISPLAY_WIDTH]	= dwColor;
			m_pReg->tft_lcd_display.buffer.back[x + y * TFT_LCD_DISPLAY_WIDTH]	= dwColor;
		}

	m_pReg->tft_lcd_display.bUpdate	= true;
}

void StarterKit::LED(DWORD pins)
{
	// power LED
	m_pReg->led.val[8]	= (pins & 0x100) ? 32 : 0;

	// LED array
	for(int i = 0; i < 8; i++) {
		m_LEDs[i].Eval(pins);
		pins	>>= 1;
	}
}

void StarterKit::NumericDisplay(DWORD pins)
{
	union {		// KW4-56NCWB-P-Y
		DWORD		pins;
		struct {
			DWORD	E		: 1;	// pin #1
			DWORD	D		: 1;	// pin #2
			DWORD	DP		: 1;	// pin #3
			DWORD	C		: 1;	// pin #4
			DWORD	G		: 1;	// pin #5
			DWORD	COM1	: 1;	// pin #6
			DWORD	COM3	: 1;	// pin #7
			DWORD	MID		: 1;	// pin #8
			DWORD	B		: 1;	// pin #9
			DWORD	COM2	: 1;	// pin #10
			DWORD	COM4	: 1;	// pin #11
			DWORD	F		: 1;	// pin #12
			DWORD	A		: 1;	// pin #13
			DWORD	COM5	: 1;	// pin #14
			DWORD	/*reserved*/	: 0;
		};
	} nd;
	nd.pins	= pins;
	DWORD	num_en[4] = {
		~nd.COM1,
		~nd.COM2,
		~nd.COM4,
		~nd.COM5
	};

	for(int i = 0; i < 4; i++) {
		m_NumericDisplay.num[i].segment[0].Eval(nd.A & num_en[i]);
		m_NumericDisplay.num[i].segment[1].Eval(nd.B & num_en[i]);
		m_NumericDisplay.num[i].segment[2].Eval(nd.C & num_en[i]);
		m_NumericDisplay.num[i].segment[3].Eval(nd.D & num_en[i]);
		m_NumericDisplay.num[i].segment[4].Eval(nd.E & num_en[i]);
		m_NumericDisplay.num[i].segment[5].Eval(nd.F & num_en[i]);
		m_NumericDisplay.num[i].segment[6].Eval(nd.G & num_en[i]);
		m_NumericDisplay.num[i].segment[7].Eval(nd.DP & num_en[i]);
	}

	m_NumericDisplay.mid.Eval(nd.MID & ~nd.COM3);
}

void StarterKit::Motor(BYTE PWM, BYTE DIR, BYTE& SENSOR)
{
	static double	fValocity	= 0;
	int				iRatio		= m_pReg->motor.iRatio;
	m_Motor.cw.Eval(PWM & ~DIR);
	m_Motor.ccw.Eval(PWM & DIR);
	fValocity	= ((fValocity * (1024 * 1024 - 1)) + ((double)(m_Motor.cw.Level() - m_Motor.ccw.Level()))) / (1024 * 1024);
	iRatio		+= (int)(fValocity * (1024 - 32 + (rand() & 31)));	// randomize forced.

	if(iRatio < 0) {
		iRatio		+= 1024 * 1024 * 1024;
	} else if(iRatio >= 1024 * 1024 * 1024) {
		iRatio		-= 1024 * 1024 * 1024;
	}

	if(iRatio != m_pReg->motor.iRatio) {
		m_pReg->motor.iRatio	= iRatio;
		m_pReg->motor.rad		= (iRatio * (3.14159265359 * 2)) / (1024 * 1024 * 1024);
		m_pReg->motor.bUpdate	= true;
	}
}

void StarterKit::TFTLCD_Display(BYTE EN, BYTE DE, BYTE VSYNC, BYTE HSYNC, DWORD dwRGB)
{
	static int	x = 0, y = 0;
	static BYTE	vsync_pre = 0, hsync_pre = 0;
	static DWORD	dwOffset	= 0;

	if(EN) {
		if(VSYNC) {
			if(HSYNC) {
				if(DE) {
					m_pReg->tft_lcd_display.buffer.back[dwOffset + x]	= dwRGB;
					x++;

					if(x >= TFT_LCD_DISPLAY_WIDTH) x = 0;
				}
			} else if(hsync_pre) {
				x = 0;
				y++;
				dwOffset	+= TFT_LCD_DISPLAY_WIDTH;

				if(y >= TFT_LCD_DISPLAY_HEIGHT) {
					y			= 0;
					dwOffset	= 0;
				}
			}
		} else if(vsync_pre) {
			y	= 0;
			x	= 0;
			dwOffset	= 0;
			memcpy(m_pReg->tft_lcd_display.buffer.front, m_pReg->tft_lcd_display.buffer.back, sizeof(m_pReg->tft_lcd_display.buffer.front));
			m_pReg->tft_lcd_display.bUpdate	= true;
		}

		hsync_pre	= HSYNC;
		vsync_pre	= VSYNC;
	} else {
		x	= 0;
		y	= 0;
		vsync_pre	= 0;
		hsync_pre	= 0;
	}
}

void StarterKit::Eval(void)
{
	if(!m_pReg) return;

	// power LED
	// LED array
	for(int i = 0; i < 8; i++) {
		float	fVal	= m_LEDs[i].Level();

		if(m_pReg->led.val[i] != fVal) {
			m_pReg->led.val[i]		= fVal;
			m_pReg->led.bUpdate		= true;
		}
	}

	// numeric display
	for(int i = 0; i < 4; i++) {
		for(int t = 0; t < 8; t++) {
			float	fVal	= m_NumericDisplay.num[i].segment[t].Level();

			if(m_pReg->numeric_display.num[i].segment[t] != fVal) {
				m_pReg->numeric_display.num[i].segment[t]	= fVal;
				m_pReg->numeric_display.bUpdate	= true;
			}
		}
	}

	{
		float	fVal	= m_NumericDisplay.mid.Level();

		if(m_pReg->numeric_display.mid != fVal) {
			m_pReg->numeric_display.mid		= fVal;
			m_pReg->numeric_display.bUpdate	= true;
		}
	}
}

void StarterKit::GetButtons(DWORD& dwButtons)
{
	dwButtons	= m_pReg->buttons;
}

void StarterKit::GetSwitches(DWORD& dwSwitches)
{
	dwSwitches	= m_pReg->switches;
}

//-------------------------------------------------------------------------
// DPI functions
//-------------------------------------------------------------------------
void StarterKit_Initialize(void)
{
	__starter_kit.Initialize();
}

void StarterKit_LED(const svBitVecVal* pins)
{
	__starter_kit.LED(*pins);
}

void StarterKit_NumericDisplay(const svBitVecVal* pins)
{
	__starter_kit.NumericDisplay(*(DWORD*)pins);
}

void StarterKit_Eval(void)
{
	__starter_kit.Eval();
}

void StarterKit_GetButtons(svBitVecVal* pins)
{
	__starter_kit.GetButtons(*(DWORD*)pins);
}

void StarterKit_GetSwitches(svBitVecVal* pins)
{
	__starter_kit.GetSwitches(*(DWORD*)pins);
}

void StarterKit_Motor(svBit PWM, svBit DIR, svBit* SENSOR)
{
	__starter_kit.Motor(PWM, DIR, *(BYTE*)SENSOR);
}

void StarterKit_TFT_LCD(svBit DISP, svBit HSYNC, svBit VSYNC, svBit DE, const svBitVecVal* RGB)
{
	__starter_kit.TFTLCD_Display(DISP, HSYNC, VSYNC, DE, *RGB);
}
