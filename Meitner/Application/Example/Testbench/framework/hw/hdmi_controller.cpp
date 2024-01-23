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
// Title : Testbench
// Rev.  : 1/23/2024 Tue (clonextop@gmail.com)
//================================================================================
#include "hdmi_controller.h"
#include "CVT.h"
#include <assert.h>

HDMI_Controller::HDMI_Controller(DDK* pDDK, DWORD dwBaseAddress, float CLKIN_MHz) : m_I2C(this), m_MMCM(this, CLKIN_MHz)
{
	assert(pDDK != NULL);
	m_pDDK			= pDDK;
	m_dwBaseAddress	= dwBaseAddress;
	m_Reg.i2c.all	= 0;
	m_Reg.mmcm.all	= ((DWORD)1 << 31) | (1 << (16 + 9));	// set (HDMI_nEN = 1), (RST = 1)
	{
		BOOL	bEnable	= strstr(m_pDDK->GetSystemDescription(), "Simplified") == NULL;
		m_I2C.Enable(bEnable);
		m_MMCM.Enable(bEnable);
	}
}

HDMI_Controller::~HDMI_Controller(void)
{
}

void HDMI_Controller::Reset(void)
{
	VideoEnable(FALSE);
	SetFrameSize(1);
	m_I2C.Reset();
}

BOOL HDMI_Controller::SetScreen(int iWidth, int iHeight, int iRefreshFreq, BOOL bReducedBlank)
{
	printf("HDMI(%dx%dx%d-'%s') initialization sequencing...\n", iWidth, iHeight, iRefreshFreq, bReducedBlank ? "Reduced Blank" : "Normal CVT");
	Reset();
	m_I2C.Write8Direct(0xE8, 0x20);	// set switch to HDMI
	{
		printf("\tWaiting for HDMI's HPD...\n");
		BYTE				data;

		if(m_I2C.IsEnable())
			for(;;) {	// wait for HPD
				m_I2C.Read8(0x72, 0x96, &data);

				if((data & 0x80)) break;
			}
	}
	m_I2C.Set8(0x72, 0x96, 0);
	{
		CVT	cvt;
		CVT_MODE* pMode = cvt.Refresh(iWidth, iHeight, iRefreshFreq, bReducedBlank);
		printf("\tSetup video timing.\n");
		m_I2C.Set8(0x72, 0x41, 0x50);	// all circuits powered down
		Sleep(10);
		// update video timing...
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->v.sync + pMode->v.back + pMode->v.active);		// V_END
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->h.sync + pMode->h.back + pMode->h.active - 2);	// H_END
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->v.sync + pMode->v.back);						// V_START
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->h.sync + pMode->h.back - 2);					// H_START
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->v.sync);										// V_GAP
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->h.sync);										// H_GAP
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->v.total - 1);									// V_SYNC
		m_pDDK->RegWrite(m_dwBaseAddress + 4, pMode->h.total - 1);									// H_SYNC
		// setup video clock
		printf("\tSetup video clock to %.2fMhz(actual %.2fMHz)\n\t", cvt.Get()->pixel_clk, m_MMCM.SetClock(0, cvt.Get()->pixel_clk));
		m_I2C.Set8(0x72, 0x41, 0x10);	// all circuits powered up
		Sleep(10);
		// must be set
		m_I2C.Set8(0x72, 0x98, 0x03);	// Must be set for proper operation
		m_I2C.Set8(0x72, 0x9A, 0xE0);	// Must be set for proper operation
		m_I2C.Set8(0x72, 0x9C, 0x30);	// Must be set for proper operation
		m_I2C.Set8(0x72, 0x9D, 0x61);	// Must be set for proper operation, no clock divide
		m_I2C.Set8(0x72, 0xA2, 0xA4);	// Must be set for proper operation
		m_I2C.Set8(0x72, 0xA3, 0xA4);	// Must be set for proper operation
		m_I2C.Set8(0x72, 0xE0, 0xD0);	// Must be set for proper operation
		m_I2C.Set8(0x72, 0xF9, 0x00);	// Fixed I2C address : 0x00
		// set up the video input mode
		m_I2C.Set8(0x72, 0x15, 0x00);	// I2S Sampling Frequency (44.1Khz), Input Video Format(24 bit RGB 4:4:4 or YCbCr 4:4:4 (separate syncs))
		m_I2C.Set8(0x72, 0x16, 0x30);	// 4:4:4 8bit color RGB mode
		m_I2C.Set8(0x72, 0x17, 0x02);	// Aspect ratio of input video : 16:9
		// set up video output mode
		m_I2C.Set8(0x72, 0x18, 0x46);	// disable CSC(color space converter)
		m_I2C.Set8(0x72, 0xAF, 0x04);	// DVI mode
		m_I2C.Set8(0x72, 0x40, 0x00);	// all packet disable
		m_I2C.Set8(0x72, 0x4C, 0x04);	// Color Depth : 24bits/Pixel
		SetFrameSize((iWidth * iHeight) / 256);			// set buffer size
		VideoEnable();
		printf("\tDone!\n\n");
	}
	return TRUE;
}

void HDMI_Controller::SetBaseAddress(uint64_t lFrameBaseAddress)
{
	m_pDDK->RegWrite(m_dwBaseAddress + (3 << 2), (DWORD)(lFrameBaseAddress));
	m_pDDK->RegWrite(m_dwBaseAddress + (4 << 2), (DWORD)(lFrameBaseAddress >> 32));
}


void HDMI_Controller::SetFrameSize(DWORD dwSize)
{
	m_pDDK->RegWrite(m_dwBaseAddress + (5 << 2), dwSize);
}

void HDMI_Controller::VideoEnable(BOOL bEnable)
{
	m_Reg.mmcm.HDMI_nEN	= ~bEnable;
	m_pDDK->RegWrite(m_dwBaseAddress + 8, m_Reg.mmcm.all);
}

void HDMI_Controller::I2C_Set(int SCL, int SDA, int nRST)
{
	m_Reg.i2c.SCL	= SCL;
	m_Reg.i2c.SDA	= SDA;
	m_Reg.i2c.nRST	= nRST;
	m_pDDK->RegWrite(m_dwBaseAddress, m_Reg.i2c.all);
}

int HDMI_Controller::I2C_Get(void)
{
	DWORD	data	= m_pDDK->RegRead(m_dwBaseAddress);
	m_Reg.i2c.SCL	= (data >> 1) & 1;
	m_Reg.i2c.SDA	= data & 1;
	return m_Reg.i2c.SDA;
}

void HDMI_Controller::MMCM_Set(DWORD dwID, BOOL bRST, BOOL bEN, BOOL bWE, DWORD dwAddr, DWORD dwData)
{
	m_Reg.mmcm.RST		= bRST;
	m_Reg.mmcm.EN		= bEN;
	m_Reg.mmcm.WE		= bWE;
	m_Reg.mmcm.ADDR		= dwAddr;
	m_Reg.mmcm.DATA		= dwData;
	m_pDDK->RegWrite(m_dwBaseAddress + 8, m_Reg.mmcm.all);
}

void HDMI_Controller::MMCM_Get(DWORD dwID, MMCM_READ& info)
{
	DWORD	data		= m_pDDK->RegRead(m_dwBaseAddress);
	info.rdata			= data >> 16;
	info.bReady			= (data >> 14) & 1;
	info.bLocked		= (data >> 13) & 1;
}
