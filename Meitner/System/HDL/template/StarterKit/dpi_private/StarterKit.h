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
// Rev.  : 1/19/2022 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __VIRTUAL_FPGA_STARTER_KIT_H__
#define __VIRTUAL_FPGA_STARTER_KIT_H__
#include "dpi_common.h"
#include "SystemConfigStarterKit.h"
#include "Power_Accumulate.h"
#include "DPI_Sound.h"

class StarterKit {
public:
	StarterKit(void);
	~StarterKit(void);

	void Initialize(void);
	void LED(DWORD pins);
	void NumericDisplay(DWORD pins);
	void Motor(BYTE PWM, BYTE DIR, BYTE& SENSOR);
	void TFTLCD_Display(BYTE EN, BYTE HSYNC, BYTE VSYNC, BYTE DE, DWORD dwRGB);
	void GetButtons(DWORD& dwButtons);
	void GetSwitches(DWORD& dwSwitches);

private:
	STARTERKIT_REGMAP*	m_pReg;
	struct {
		PowerAccumulate		cw, ccw;
	} m_Motor;
};

DPI_FUNCTION void StarterKit_MainLoop(void);
DPI_FUNCTION void StarterKit_LED(const svBitVecVal* pins);
DPI_FUNCTION void StarterKit_NumericDisplay(const svBitVecVal* pins);
DPI_FUNCTION void StarterKit_GetButtons(svBitVecVal* pins);
DPI_FUNCTION void StarterKit_GetSwitches(svBitVecVal* pins);
DPI_FUNCTION void StarterKit_Motor(svBit PWM, svBit DIR, svBit* SENSOR);
DPI_FUNCTION void StarterKit_TFT_LCD(svBit DISP, svBit HSYNC, svBit VSYNC, svBit DE, const svBitVecVal* RGB);
DPI_FUNCTION void StarterKit_Buzzer(svBit pin);

#endif//__VIRTUAL_FPGA_STARTER_KIT_H__
