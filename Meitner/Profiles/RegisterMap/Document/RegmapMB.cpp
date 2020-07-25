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
#include "RegmapMB.h"

#define MB_SHOWUP_SIZE		40

static LPCTSTR	__sMemoryName[] = {
	_T("LMB"),
	_T("GMB"),
};

static LPCTSTR	__sMemoryDescription[] = {
	_T("Local Memory Block"),
	_T("Global Memory Block"),
};

static LPCTSTR	__sLMB_tag = _T("LMB_tag");

RegmapMB::RegmapMB(void) : Regmap(_T("MB"))
{
	m_pGMB	= &m_pReg->GMB;
	m_pLMB	= &m_pReg->core[m_pReg->system.iCoreID].LMB;
	memset(m_iStartupIndex, 0, sizeof(m_iStartupIndex));
}

RegmapMB::~RegmapMB(void)
{
}

BOOL RegmapMB::OnUpdate(void)
{
	if(m_pGMB->bUpdate) {
		m_pGMB->bUpdate	= FALSE;
		UpdateVectors(MB_TYPE_GLOBAL);
	}

	if(m_pLMB->bUpdate) {
		m_pLMB->bUpdate	= FALSE;
		UpdateVectors(MB_TYPE_LOCAL);
	}

	return FALSE;
}

void RegmapMB::OnBroadcast(LPVOID pData)
{
	switch(*(REGMAP_BROADCAST*)(&pData)) {
	case REGMAP_BROADCAST_INITIALIZATION:
		InitTable();
		m_pGMB->bUpdate	= TRUE;
		m_pLMB->bUpdate	= TRUE;
		break;

	case REGMAP_BROADCAST_SYSTEM_REBOOT: {
		g_pHtml->CallJScript(_T("SetSlideMax('%s_slider',%d);"), __sMemoryName[0], m_pReg->system.iLMBSize - MB_SHOWUP_SIZE);
		g_pHtml->CallJScript(_T("SetSlideMax('%s_slider',%d);"), __sMemoryName[1], m_pReg->system.iGMBSize - MB_SHOWUP_SIZE);
	}

	UpdateIndex(MB_TYPE_LOCAL);
	UpdateIndex(MB_TYPE_GLOBAL);
	UpdateVectors(MB_TYPE_GLOBAL);
	UpdateVectors(MB_TYPE_LOCAL);
	UpdateLMBName();
	break;
	}
}

BOOL RegmapMB::OnCommand(LPCTSTR lpszURL)
{
	if(_tcsstr(lpszURL, _T("slide"))) {
		lpszURL	+= 6;

		for(int i = 0; i < 2; i++) {
			if(_tcsstr(lpszURL, __sMemoryName[i]) == lpszURL) {
				m_iStartupIndex[i]	= _tstoi(lpszURL + 4);
				UpdateIndex((MB_TYPE)i);
				UpdateVectors((MB_TYPE)i);
				break;
			}
		}
	} else if(_tcsstr(lpszURL, _T("recent"))) {
		lpszURL	+= 7;

		for(int i = 0; i < 2; i++) {
			if(_tcsstr(lpszURL, __sMemoryName[i]) == lpszURL) {
				UINT64	uLastUpdateTime;
				int		iRecentUsed;
				int		iMaxLimit;

				switch(i) {
				case MB_TYPE_LOCAL:
					uLastUpdateTime	= m_pLMB->uLastUpdateTime;
					iRecentUsed		= m_pLMB->dwRecentUsed;
					iMaxLimit		= m_pReg->system.iLMBSize - MB_SHOWUP_SIZE;
					break;

				case MB_TYPE_GLOBAL:
					uLastUpdateTime	= m_pGMB->uLastUpdateTime;
					iRecentUsed		= m_pGMB->dwRecentUsed;
					iMaxLimit		= m_pReg->system.iGMBSize - MB_SHOWUP_SIZE;
					break;
				}

				if(!uLastUpdateTime) {
					g_pSystem->LogWarning(_T("No recent modified of %s!"), __sMemoryName[i]);
					break;
				}

				iRecentUsed		-= (MB_SHOWUP_SIZE / 2);

				if(iRecentUsed <= 0) iRecentUsed = 0;
				else if(iRecentUsed >= iMaxLimit) iMaxLimit = 0;

				m_iStartupIndex[i]	= iRecentUsed;
				UpdateIndex((MB_TYPE)i);
				UpdateVectors((MB_TYPE)i);
				break;
			}
		}
	}

	return TRUE;
}

void RegmapMB::UpdateLMBName(void)
{
	HString sTag;
	sTag.AppendFormat(_T("%s (%s - Core #%d)"), __sMemoryName[0], __sMemoryDescription[0], m_pReg->system.iCoreID);
	sTag.bold();
	SetText(__sLMB_tag, sTag);
}

void RegmapMB::InitTable(void)
{
	ClearTable();
	// setup Header : index, table #
	NewRow();

	for(int i = 0; i < 2; i++) {
		NewTH();
		SetColSpan(10);
		{
			HString sTag;
			sTag.AppendFormat(_T("%s (%s)"), __sMemoryName[i], __sMemoryDescription[i]);
			sTag.bold();
			SetTBody(sTag);

			if(i == 0) SetTID(__sLMB_tag);
		}
	}

	UpdateLMBName();
	// setup Header : index, table #
	NewRow();

	// memory block name
	for(int i = 0; i < 2; i++) {
		NewTH();
		SetTBody(_T("Index"));

		for(int t = 0; t < 8; t++) {
			NewTH();
			SetTBody(_T("%s<sup>%d</sup>"), g_s4DElement[t & 3], t >> 2);

			if((t & 3) != 3)SetTStyle(_T("border-right"), _T("0"));
		}

		NewTH();	// dummy blank
		SetTBody(_T("<div class='tooltip'><a href='cmd://MB/recent/%s'><img src='slider.png' border='none'></a><span class='tooltiptext'>%s</span></div>"), __sMemoryName[i], _L(SEARCH_RECENT_USE));
	}

	// contents table
	NewRow();

	for(int i = 0; i < 2; i++) {
		// index
		NewTH();
		SetTStyle(_T("color"), _T("#afafaf"));
		SetTStyle(_T("font-size"), _T("#90%"));
		SetTID(_T("%s_I"), __sMemoryName[i]);
		{
			// data cell
			for(int t = 0; t < 8; t++) {
				NewTD();
				SetTStyle(_T("text-align"), _T("center"));
				SetTStyle(_T("font-family"), _T("Courier New"));
				SetTStyle(_T("font-size"), _T("#90%"));
				SetTStyle(_T("color"), _T("#cfcfcf"));
				CString sTag = _T("td style='text-align=center;font-size:90%;color:#cfcfcf");

				if((t & 3) != 3) SetTStyle(_T("border-right"), _T("0"));

				SetTID(_T("%s_D%d"), __sMemoryName[i], t);
			}
		}
		// slider
		NewTH();
		SetTID(_T("%s_S"), __sMemoryName[i]);
		{
			int	iMax;

			switch(i) {
			case MB_TYPE_LOCAL:
				iMax	= m_pReg->system.iLMBSize;
				break;

			case MB_TYPE_GLOBAL:
				iMax	= m_pReg->system.iGMBSize;
				break;
			}

			iMax	-= MB_SHOWUP_SIZE;
			SetTStyle(_T("width"), _T("20px"));
			SetTBody(_T("<input type='range' id='%s_slider' value='0' min='0' max='%d' onchange='PostSlideValue(\\\"MB\\\", \\\"%s\\\", this.value)'>"), __sMemoryName[i], iMax, __sMemoryName[i]);
			m_iStartupIndex[i] = 0;
		}
	}
}

void RegmapMB::UpdateIndex(MB_TYPE type)
{
	int			iStartupIndex	= m_iStartupIndex[(int)type];
	CString	sTarget, sIndex;
	sTarget.Format(_T("%s_I"), __sMemoryName[(int)type]);

	for(int t = 0, idx = m_iStartupIndex[(int)type]; t < MB_SHOWUP_SIZE; t++, idx ++) {
		HString sID;

		if(t) sIndex.Append(_T("<br>"));

		sID.Format(_T("%d"), idx);

		if(!(idx & 0x7)) sID.underline();

		sIndex.Append(sID);
	}

	SetText(sTarget, sIndex);
}

void RegmapMB::UpdateVectors(MB_TYPE type)
{
	int			iStartupIndex	= m_iStartupIndex[(int)type];
	UINT64		uiLastUpdateTime;
	REG_VALUE8* pVectorBase;

	switch(type) {
	case MB_TYPE_LOCAL:
		pVectorBase			= &m_pLMB->reg[iStartupIndex];
		uiLastUpdateTime	= m_pLMB->uLastUpdateTime;
		break;

	case MB_TYPE_GLOBAL:
		pVectorBase			= &m_pGMB->reg[iStartupIndex];
		uiLastUpdateTime	= m_pGMB->uLastUpdateTime;
		break;
	}

	for(int t = 0; t < 8; t++) {
		CString sTarget;
		CString	sData;
		REG_VALUE8* pVector	= pVectorBase;
		sTarget.Format(_T("%s_D%d"), __sMemoryName[(int)type], t);

		for(int x = 0; x < MB_SHOWUP_SIZE; x++, pVector++) {
			HString	sValue;

			if(x) sData.Append(_T("<br>"));

			sValue.Format(_T("%08X"), pVector->udata[t]);

			if(pVector->uUpdateTime) {
				sValue.SetColor(
					(pVector->uUpdateTime ==  uiLastUpdateTime) 	? 0xFF0000 :
					(pVector->uUpdateTime == -uiLastUpdateTime) 	? 0xFF0000 :
					(pVector->uUpdateTime & 0x8000000000000000L)	? 0x00007F : 0x7F0000
				);
			}

			if(!((x + iStartupIndex) & 0x7))sValue.underline();

			sData.Append(sValue);
		}

		SetText(sTarget, sData);
	}
}
