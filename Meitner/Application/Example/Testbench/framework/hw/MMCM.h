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
#ifndef __MMCM_H__
#define __MMCM_H__
#include "STDInterface.h"

typedef enum{
	MMCM_CLK_0,
	MMCM_CLK_1,
	MMCM_CLK_2,
	MMCM_CLK_3,
	MMCM_CLK_4,
	MMCM_CLK_5,
	MMCM_CLK_6
}MMCM_CLK;

typedef struct{
	WORD		rdata;
	BOOL		bReady;
	BOOL		bLocked;
}MMCM_READ;

interface IMMCM{
public:
	STDMETHOD_(void, MMCM_Set)(DWORD dwID, BOOL bRST, BOOL bEN, BOOL bWE, DWORD dwAddr, DWORD dwData) PURE;
	STDMETHOD_(void, MMCM_Get)(DWORD dwID, MMCM_READ& info) PURE;
};

class MMCM{
public:
	MMCM(IMMCM* pMMCM, float CLKIN_MHz);
	~MMCM(void);

	void Enable(BOOL bEnable = TRUE);
	inline BOOL IsEnable(void)		{return m_bEnable;}

	float SetClock(DWORD dwID, float mHz);
	float GetReferenceClock(void)	{return m_fCLKIN;}
protected:
	void Reset(BOOL bRST = TRUE);
	float FindClock(float ReqMHz, DWORD& D, DWORD& M, DWORD& O);
	void SetDivide(DWORD dwNum);
	void SetOutputDivide(DWORD dwID, DWORD dwNum);
	inline void SetMultiplier(DWORD dwNum)	{SetOutputDivide((DWORD)-1, dwNum);}

private:
	void UpdateSignal(void);
	void Write(DWORD dwAddr, DWORD dwData);
	DWORD Read(DWORD dwAddr);
	void WaitLock(void);
	void WaitReady(void);

	struct{
		BOOL		bRST;
		BOOL		bEN;
		BOOL		bWE;
		DWORD		dwAddr;
		DWORD		dwData;
	}m_Signal;

	DWORD		m_dwID;			// MMCM ID
	float		m_fCLKIN;		// input clock mhz
	IMMCM*		m_pMMCM;
	BOOL		m_bEnable;
};

#endif//__MMCM_H__
