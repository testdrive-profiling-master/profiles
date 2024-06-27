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
// Title : Virtual FPGA Starter Kit Template design
// Rev.  : 5/10/2024 Fri (clonextop@gmail.com)
//================================================================================
#include "StarterKit.h"
#include "Buzzer.h"

static StarterKit __starter_kit;
static Buzzer	  __Buzzer;

static bool		  __Initialize(void)
{
	SetSystemDescription("FPGA Starter Kit");

	if (!g_Sound.Initialize())
		return false;

	__starter_kit.Initialize();
	return true;
}

static void __Finalize(void)
{
	g_Sound.ReleaseAll();
}

StarterKit::StarterKit(void)
{
	DPI_Initialize = __Initialize;
	DPI_Finalize   = __Finalize;
	m_pReg		   = NULL;
}

StarterKit::~StarterKit(void) {}

void StarterKit::Initialize(void)
{
	m_pReg			   = (STARTERKIT_REGMAP *)GET_SYSTEM_REGMAP;
	m_pReg->magic_code = SYSTEM_MAGIC_CODE;
	memset(&m_pReg->led, 0, sizeof(m_pReg->led));
	m_pReg->buttons = 0xFFFFFFFF;
	memset(m_pReg->tft_lcd_display.buffer.front, 0, sizeof(m_pReg->tft_lcd_display.buffer.front));

	// randomize screen
	for (int y = 0; y < TFT_LCD_DISPLAY_HEIGHT; y++)
		for (int x = 0; x < TFT_LCD_DISPLAY_WIDTH; x++) {
			uint32_t dwColor = (rand() & 0xFF) | ((rand() & 0xFF)) << 8 | ((rand() & 0xFF) << 16);
			m_pReg->tft_lcd_display.buffer.front[x + y * TFT_LCD_DISPLAY_WIDTH] = dwColor;
			m_pReg->tft_lcd_display.buffer.back[x + y * TFT_LCD_DISPLAY_WIDTH]	= dwColor;
		}

	m_pReg->tft_lcd_display.bUpdate = true;
}

void StarterKit::LED(uint32_t pins)
{
	for (int i = 0; i < 9; i++) {
		ACCUMULATE_DATA acc;
		acc.m = m_pReg->led.power[i].post.m;
		acc.clk++;

		if (pins & 1)
			acc.acc++;

		m_pReg->led.power[i].post.m = acc.m;
		pins >>= 1;
	}
}

void StarterKit::NumericDisplay(uint32_t pins)
{
	union { // KW4-56NCWB-P-Y
		uint32_t pins;
		struct {
			uint32_t E			  : 1; // pin #1
			uint32_t D			  : 1; // pin #2
			uint32_t DP			  : 1; // pin #3
			uint32_t C			  : 1; // pin #4
			uint32_t G			  : 1; // pin #5
			uint32_t COM1		  : 1; // pin #6
			uint32_t COM3		  : 1; // pin #7
			uint32_t MID		  : 1; // pin #8
			uint32_t B			  : 1; // pin #9
			uint32_t COM2		  : 1; // pin #10
			uint32_t COM4		  : 1; // pin #11
			uint32_t F			  : 1; // pin #12
			uint32_t A			  : 1; // pin #13
			uint32_t COM5		  : 1; // pin #14
			uint32_t /*reserved*/ : 0;
		};
	} nd;
	nd.pins			   = pins;
	uint32_t num_en[4] = {
		1 ^ (uint32_t)nd.COM1, // COM1
		1 ^ (uint32_t)nd.COM2, // COM2
		1 ^ (uint32_t)nd.COM4, // COM4
		1 ^ (uint32_t)nd.COM5  // COM5
	};
	uint32_t pin_en[8] = {
		nd.A, // A
		nd.B, // B
		nd.C, // C
		nd.D, // D
		nd.E, // E
		nd.F, // F
		nd.G, // G
		nd.DP // DP
	};

	for (int i = 0; i < 4; i++) {
		for (int t = 0; t < 8; t++) {
			ACCUMULATE_DATA acc;
			acc.m = m_pReg->numeric_display.num[i].segment[t].post.m;
			acc.clk++;

			if (pin_en[t] & num_en[i])
				acc.acc++;

			m_pReg->numeric_display.num[i].segment[t].post.m = acc.m;
		}

		/*m_NumericDisplay.num[i].segment[0].Eval(nd.A & num_en[i]);
		m_NumericDisplay.num[i].segment[1].Eval(nd.B & num_en[i]);
		m_NumericDisplay.num[i].segment[2].Eval(nd.C & num_en[i]);
		m_NumericDisplay.num[i].segment[3].Eval(nd.D & num_en[i]);
		m_NumericDisplay.num[i].segment[4].Eval(nd.E & num_en[i]);
		m_NumericDisplay.num[i].segment[5].Eval(nd.F & num_en[i]);
		m_NumericDisplay.num[i].segment[6].Eval(nd.G & num_en[i]);
		m_NumericDisplay.num[i].segment[7].Eval(nd.DP & num_en[i]);*/
	}

	{
		ACCUMULATE_DATA acc;
		acc.m = m_pReg->numeric_display.mid.post.m;
		acc.clk++;

		if (nd.MID & ~nd.COM3)
			acc.acc++;
	}
}

void StarterKit::Motor(uint8_t PWM, uint8_t DIR, uint8_t &SENSOR)
{
	static double fValocity = 0;
	int			  iRatio	= m_pReg->motor.iRatio;
	m_Motor.cw.Eval(PWM & ~DIR);
	m_Motor.ccw.Eval(PWM & DIR);
	fValocity =
		((fValocity * (1024 * 1024 - 1)) + ((double)(m_Motor.cw.Level() - m_Motor.ccw.Level()))) / (1024 * 1024);
	iRatio += (int)(fValocity * (1024 - 32 + (rand() & 31))); // randomize forced.

	if (iRatio < 0) {
		iRatio += 1024 * 1024 * 1024;
	} else if (iRatio >= 1024 * 1024 * 1024) {
		iRatio -= 1024 * 1024 * 1024;
	}

	if (iRatio != m_pReg->motor.iRatio) {
		m_pReg->motor.iRatio  = iRatio;
		m_pReg->motor.rad	  = (iRatio * (3.14159265359 * 2)) / (1024 * 1024 * 1024);
		m_pReg->motor.bUpdate = true;
	}
}

void StarterKit::TFTLCD_Display(uint8_t EN, uint8_t DE, uint8_t VSYNC, uint8_t HSYNC, uint32_t dwRGB)
{
	static int	 x = 0, y = 0;
	static uint8_t	 vsync_pre = 0, hsync_pre = 0;
	static uint32_t dwOffset = 0;

	if (EN) {
		if (VSYNC) {
			if (HSYNC) {
				if (DE) {
					m_pReg->tft_lcd_display.buffer.back[dwOffset + x] = dwRGB;
					x++;

					if (x >= TFT_LCD_DISPLAY_WIDTH)
						x = 0;
				}
			} else if (hsync_pre) {
				x = 0;
				y++;
				dwOffset += TFT_LCD_DISPLAY_WIDTH;

				if (y >= TFT_LCD_DISPLAY_HEIGHT) {
					y		 = 0;
					dwOffset = 0;
				}
			}
		} else if (vsync_pre) {
			y		 = 0;
			x		 = 0;
			dwOffset = 0;
			memcpy(m_pReg->tft_lcd_display.buffer.front, m_pReg->tft_lcd_display.buffer.back,
				   sizeof(m_pReg->tft_lcd_display.buffer.front));
			m_pReg->tft_lcd_display.bUpdate = true;
		}

		hsync_pre = HSYNC;
		vsync_pre = VSYNC;
	} else {
		x		  = 0;
		y		  = 0;
		vsync_pre = 0;
		hsync_pre = 0;
	}
}

void StarterKit::GetButtons(uint32_t &dwButtons)
{
	dwButtons = m_pReg->buttons;
}

void StarterKit::GetSwitches(uint32_t &dwSwitches)
{
	dwSwitches = m_pReg->switches;
}

//-------------------------------------------------------------------------
// DPI functions
//-------------------------------------------------------------------------
void StarterKit_MainLoop(void)
{
	{
		uint32_t dwFrequency;
		float fVolume;
		__Buzzer.Get(dwFrequency, fVolume);

		if (dwFrequency && fVolume) {
		}
	}
}

void StarterKit_LED(const svBitVecVal *pins)
{
	__starter_kit.LED(*pins);
}

void StarterKit_NumericDisplay(const svBitVecVal *pins)
{
	__starter_kit.NumericDisplay(*(uint32_t *)pins);
}

void StarterKit_GetButtons(svBitVecVal *pins)
{
	__starter_kit.GetButtons(*(uint32_t *)pins);
}

void StarterKit_GetSwitches(svBitVecVal *pins)
{
	__starter_kit.GetSwitches(*(uint32_t *)pins);
}

void StarterKit_Motor(svBit PWM, svBit DIR, svBit *SENSOR)
{
	__starter_kit.Motor(PWM, DIR, *(uint8_t *)SENSOR);
}

void StarterKit_TFT_LCD(svBit DISP, svBit HSYNC, svBit VSYNC, svBit DE, const svBitVecVal *RGB)
{
	__starter_kit.TFTLCD_Display(DISP, HSYNC, VSYNC, DE, *RGB);
}

void StarterKit_Buzzer(svBit pin)
{
	__Buzzer.Eval(pin);
}
