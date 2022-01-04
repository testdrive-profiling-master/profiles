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
#include "RegmapNumericDisplay.h"

RegmapNumericDisplay::RegmapNumericDisplay(void) : Regmap(_T("NUM"))
{
	m_pNum		= &m_pReg->numeric_display;
	memset(m_fSegments, 0, sizeof(m_fSegments));
	m_fMid		= 0;
}

RegmapNumericDisplay::~RegmapNumericDisplay(void)
{
}

BOOL RegmapNumericDisplay::OnUpdate(void)
{
	return UpdateData();
}

void RegmapNumericDisplay::OnBroadcast(LPVOID pData)
{
	if(!pData) {
		UpdateData();
	}
}

BOOL RegmapNumericDisplay::OnCommand(LPCTSTR lpszURL)
{
	return FALSE;
}

bool RegmapNumericDisplay::UpdateData(void)
{
	bool	bRet	= false;

	// seven segments
	for(int i = 0; i < 4; i++) {
		bool	bUpdate	= false;

		for(int t = 0; t < 8; t++) {
			ACCUMULATE_DATA	post, recv;
			post.m	= m_pNum->num[i].segment[t].post.m;
			recv.m	= m_pNum->num[i].segment[t].recv.m;
			DWORD	dwClock	= post.clk - recv.clk;
			DWORD	dwAcc	= post.acc - recv.acc;
			m_pNum->num[i].segment[t].recv.m	= post.m;

			if(!dwClock) return false;

			float	val	= (double)dwAcc	/ dwClock;

			if(m_fSegments[i][t] != val) {
				m_fSegments[i][t] = val;
				bUpdate	= true;
			}
		}

		if(bUpdate) {
			g_pHtml->CallJScript(_T("SetNumericDisplay(%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f)"),
								 i,
								 m_fSegments[i][0],
								 m_fSegments[i][1],
								 m_fSegments[i][2],
								 m_fSegments[i][3],
								 m_fSegments[i][4],
								 m_fSegments[i][5],
								 m_fSegments[i][6],
								 m_fSegments[i][7]);
			bRet	= true;
		}
	}

	// mid
	{
		ACCUMULATE_DATA	post, recv;
		post.m	= m_pNum->mid.post.m;
		recv.m	= m_pNum->mid.recv.m;
		DWORD	dwClock	= post.clk - recv.clk;
		DWORD	dwAcc	= post.acc - recv.acc;
		m_pNum->mid.recv.m	= post.m;

		if(!dwClock) return false;

		float	val	= (double)dwAcc	/ dwClock;

		if(val != m_fMid) {
			g_pHtml->CallJScript(_T("SetNumericDisplay(4,0,0,0,0,0,0,0,%.2f)"), val);
			m_fMid	= val;
			bRet	= true;
		}
	}
	return bRet;
}

