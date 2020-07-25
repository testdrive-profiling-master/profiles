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
// Title : Meitner processor register map document
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "RegmapTrace.h"

RegmapTrace::RegmapTrace(void) : Regmap(_T("Trace"))
{
	m_bEnable	= FALSE;
}

RegmapTrace::~RegmapTrace(void)
{
}

BOOL RegmapTrace::OnUpdate(void)
{
	return FALSE;
}

void RegmapTrace::OnBroadcast(LPVOID pData)
{
	switch(*(REGMAP_BROADCAST*)(&pData)) {
	case REGMAP_BROADCAST_INITIALIZATION:
		InitTable();
		break;

	case REGMAP_BROADCAST_SYSTEM_REBOOT:
		break;
	}

	UpdateTraceTag();
}

BOOL RegmapTrace::OnCommand(LPCTSTR lpszURL)
{
	if(_tcsstr(lpszURL, _T("Enable"))) {
		m_bEnable	= _tstoi(lpszURL + 7);
	}

	return FALSE;
}

void RegmapTrace::InitTable(void)
{
	ClearTable();
	NewRow();

	for(int i = 0; i < THREAD_SIZE; i++) {
		NewTH();
		SetTID(_T("TRD_%d"), i);
	}

	NewTH();
	SetTBody(_T("<div class='tooltip'><a href='cmd://Trace/latest'><img src='here.png' border='none'></a><span class='tooltiptext'>%s</span></div>"), _L(GOTO_LATEST));
	NewRow();

	for(int i = 0; i < THREAD_SIZE; i++) {
		NewTD();
		SetTID(_T("TRACE_%d"), i);
		SetTextAlignment(TEXT_ALIGN_LEFT);
		SetTBody(_T("-"));
	}

	NewTH();
	SetTStyle(_T("width"), _T("20px"));
	SetTBody(_T("<input type='range' id='trace_slider' value='0' min='0' max='100' onchange='PostSlideValue(\\\"Trace\\\", \\\"Pos\\\", this.value)'>"));
}

void RegmapTrace::UpdateTraceTag(void)
{
	SetText(_T("THREAD_tag"), _T("- Threads (Core #%d)"), m_pReg->system.iCoreID);
}
