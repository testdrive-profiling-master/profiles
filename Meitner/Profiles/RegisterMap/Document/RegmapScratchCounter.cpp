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
#include "RegmapScratchCounter.h"

RegmapScratchCounter::RegmapScratchCounter(void) : Regmap(_T("Scratch"))
{
	m_pGlobal			= &m_pReg->scratch;
	m_pLocal			= &m_pReg->core[m_pReg->system.iCoreID].scratch;
	m_pLocal->bUpdate	= TRUE;
}

RegmapScratchCounter::~RegmapScratchCounter(void)
{
}

BOOL RegmapScratchCounter::OnUpdate(void)
{
	if(m_pGlobal->bUpdate) {
		m_pGlobal->bUpdate	= FALSE;
		UpdateData(SCRATCH_COUNTER_ID_GLOBAL);
	}

	if(m_pLocal->bUpdate) {
		m_pLocal->bUpdate	= FALSE;
		UpdateData(SCRATCH_COUNTER_ID_LOCAL);
	}

	return FALSE;
}

void RegmapScratchCounter::OnBroadcast(LPVOID pData)
{
	switch(*(REGMAP_BROADCAST*)(&pData)) {
	case REGMAP_BROADCAST_INITIALIZATION:
		InitTable();
		break;

	case REGMAP_BROADCAST_SYSTEM_REBOOT:
		m_pLocal			= &m_pReg->core[m_pReg->system.iCoreID].scratch;
		m_pLocal->bUpdate	= TRUE;
		UpdateTagName();
		break;
	}
}

BOOL RegmapScratchCounter::OnCommand(LPCTSTR lpszURL)
{
	return FALSE;
}

void RegmapScratchCounter::InitTable(void)
{
	ClearTable();
	// headers
	NewRow();
	NewTH();
	SetTID(_T("LSC_tag"));
	SetTStyle(_T("font-weight"), _T("bold"));
	//SetTBody(_T("Local Scratch counter"));
	NewTH();
	SetTStyle(_T("font-weight"), _T("bold"));
	SetTBody(_T("Global Scratch counter"));
	// data
	NewRow();
	NewTD();
	SetTID(_T("LSC"));
	NewTD();
	SetTID(_T("GSC"));
	UpdateTagName();
}

void RegmapScratchCounter::UpdateTagName(void)
{
	SetText(_T("LSC_tag"), _T("Local Scratch counter - Core #%d"), m_pReg->system.iCoreID);
}

void RegmapScratchCounter::UpdateData(SCRATCH_COUNTER_ID id)
{
	switch(id) {
	case SCRATCH_COUNTER_ID_GLOBAL:
		break;

	case SCRATCH_COUNTER_ID_LOCAL:
		break;
	}
}

