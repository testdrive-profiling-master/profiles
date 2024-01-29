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
// Rev.  : 1/29/2024 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __DUT_H__
#define __DUT_H__
#include "common.h"
#include "MMCM.h"			// mmcm controller interface

#define	BUS_CLOCK_MHZ		250					// 250MHz
#define	DUT_BASE			0x20000
#define	DUT_CLOCKGEN_BASE	(DUT_BASE | 0x100)

typedef union {
	DWORD		all;
	struct {
		DWORD		clkgen_rdata	: 16;
		DWORD		clkgen_locked	: 1;
		DWORD		clkgen_ready	: 1;
		DWORD		intr_count		: 10;
		DWORD		rfifo_ready		: 1;
		DWORD		wfifo_ready		: 1;
		DWORD		intr			: 1;
		DWORD		busy			: 1;
	};
} INTR_STATUS;

// Design under target
class DUT : public IMMCM {
public:
	DUT(DDK* pDDK);
	virtual ~DUT(void);

	void SetClock(float mHz);			// set process clock
	BOOL IsBusy(void);					// Is process busy?

	inline DWORD RegRead(DWORD dwAddr)					{
		return m_pDDK->RegRead(DUT_BASE | dwAddr);
	}
	inline void RegWrite(DWORD dwAddr, DWORD dwData)	{
		m_pDDK->RegWrite(DUT_BASE | dwAddr, dwData);
	}
	inline INTR_STATUS ClearInterrupt(void)	{
		INTR_STATUS	status;
		status.all	= RegRead(DUT_CLOCKGEN_BASE);
		return status;
	}

protected:
	DDK*	m_pDDK;

private:
	virtual void MMCM_Set(DWORD dwID, BOOL bRST, BOOL bEN, BOOL bWE, DWORD dwAddr, DWORD dwData);
	virtual void MMCM_Get(DWORD dwID, MMCM_READ& info);

	MMCM	m_MMCM;
};

#endif//__DUT_H__
