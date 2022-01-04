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
// Title : Starter Kit document
// Rev.  : 1/4/2022 Tue (clonextop@gmail.com)
//================================================================================
#include "RegmapLED.h"

RegmapLED::RegmapLED(void) : Regmap(_T("LED"))
{
	m_pLED		= &m_pReg->led;
	memset(m_fVal, 0, sizeof(m_fVal));
}

RegmapLED::~RegmapLED(void)
{
}

BOOL RegmapLED::OnUpdate(void)
{
	return UpdateData();
}

void RegmapLED::OnBroadcast(LPVOID pData)
{
	if(!pData) {
		UpdateData();
	}
}

BOOL RegmapLED::OnCommand(LPCTSTR lpszURL)
{
	return FALSE;
}

bool RegmapLED::UpdateData(void)
{
	bool	bRet	= false;
	float	val[9];

	for(int i = 0; i < 9; i++) {
		ACCUMULATE_DATA	post, recv;
		post.m	= m_pLED->power[i].post.m;
		recv.m	= m_pLED->power[i].recv.m;
		DWORD	dwClock	= post.clk - recv.clk;
		DWORD	dwAcc	= post.acc - recv.acc;
		m_pLED->power[i].recv.m	= post.m;

		if(!dwClock) return false;

		float	val	= (double)dwAcc	/ dwClock;

		if(m_fVal[i] != val) {
			m_fVal[i] = val;
			bRet	= true;
		}
	}

	if(bRet) {
		g_pHtml->CallJScript(_T("SetLED(%.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f);"),
							 m_fVal[0],
							 m_fVal[1],
							 m_fVal[2],
							 m_fVal[3],
							 m_fVal[4],
							 m_fVal[5],
							 m_fVal[6],
							 m_fVal[7],
							 m_fVal[8]);
	}

	return bRet;
}
