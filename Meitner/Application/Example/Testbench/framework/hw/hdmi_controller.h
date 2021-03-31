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
// Title : Testbench
// Rev.  : 3/31/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __HDMI_CONTROLLER_H__
#define __HDMI_CONTROLLER_H__
#include "common.h"
#include "I2C.h"			// i2c interface
#include "MMCM.h"			// mmcm controller interface
#include "CVT.h"			// coordinate video timing generator

class HDMI_Controller : public II2C, public IMMCM {
public:
	HDMI_Controller(DDK* pDDK, DWORD dwBaseAddress, float CLKIN_MHz);
	virtual ~HDMI_Controller(void);

	void Reset(void);
	BOOL SetScreen(int iWidth, int iHeight, int iRefreshFreq = 60, BOOL bReducedBlank = FALSE);
	void SetBaseAddress(DWORD dwFrameBaseAddress);

protected:
	void SetFrameSize(DWORD dwSize);
	void VideoEnable(BOOL bEnable = TRUE);
	virtual void I2C_Set(int SCL, int SDA, int nRST);
	virtual int I2C_Get(void);
	virtual void MMCM_Set(DWORD dwID, BOOL bRST, BOOL bEN, BOOL bWE, DWORD dwAddr, DWORD dwData);
	virtual void MMCM_Get(DWORD dwID, MMCM_READ& info);

private:
	struct{
		union{
			DWORD	all;
			struct{
				DWORD	SDA			: 1;
				DWORD	SCL			: 1;
				DWORD	nRST		: 1;
				DWORD	/*dummy*/	: 0;
			};
		}i2c;
		union{
			DWORD	all;
			struct{
				DWORD	DATA		: 16;
				DWORD	ADDR		: 7;
				DWORD	WE			: 1;
				DWORD	EN			: 1;
				DWORD	RST			: 1;
				DWORD	/*dummy*/	: 5;
				DWORD	HDMI_nEN	: 1;
			};
		}mmcm;
	}m_Reg;

	DWORD		m_dwBaseAddress;
	DDK*		m_pDDK;
	I2C			m_I2C;
	MMCM		m_MMCM;
};

#endif//__HDMI_CONTROLLER_H__
