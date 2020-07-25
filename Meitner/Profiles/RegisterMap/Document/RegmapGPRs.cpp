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
#include "RegmapGPRs.h"

static LPCTSTR __sBoolean[2] = {
	_T("False"),
	_T("True")
};

static LPCTSTR __TypePreFix[] = {
	_T("a"),
	_T("i"),
	_T("f"),
	_T("h"),
	_T("b")
};

RegmapGPRs::RegmapGPRs(void) : Regmap(_T("GPRs"))
{
	memset(m_Type, 0, sizeof(m_Type));
	memset(&m_uiUpdateTime, 0xCC, sizeof(m_uiUpdateTime));
	m_pGPRs			= NULL;
	m_iGPRsSize		= 0;
	m_bFlushAll		= TRUE;
}

RegmapGPRs::~RegmapGPRs(void)
{
}

BOOL RegmapGPRs::OnUpdate(void)
{
	{
		GPRs_T*	pGPRs	= &m_pReg->core[m_pReg->system.iCoreID].thread[m_pReg->system.iThreadID].GPRs;

		if(pGPRs != m_pGPRs) {
			m_pGPRs			= pGPRs;
			m_bFlushAll		= TRUE;
		}
	}

	if(m_pReg->system.iGPRsSize != m_iGPRsSize) {
		OnBroadcast(NULL);
	}

	if(m_pGPRs->bUpdate) {
		m_pGPRs->bUpdate	= FALSE;
		UpdateVectorAll();
	}

	return FALSE;
}

void RegmapGPRs::OnBroadcast(LPVOID pData)
{
	if(!pData) {	// on initialization
		m_iGPRsSize			= m_pReg->system.iGPRsSize;
		int iRowSize		= m_iGPRsSize / 4;
		m_pGPRs				= &m_pReg->core[m_pReg->system.iCoreID].thread[m_pReg->system.iThreadID].GPRs;
		m_pGPRs->bUpdate	= TRUE;
		ClearTable();
		{
			// setup header
			NewRow();

			for(int i = 0; i < 4; i++) {
				NewTH();
				SetTBody(_T("Index"));

				for(int t = 0; t < 4; t++) {
					NewTH();
					SetTBody(g_s4DElement[t]);
				}
			}
		}

		for(int i = 0; i < iRowSize; i++) {
			NewRow();

			for(int t = 0; t < 4; t++) {
				CString sTag;
				DWORD dwID	= i + (t * iRowSize);
				NewTH();
				SetTStyle(_T("font-size"), _T("11"));
				SetTID(_T("GPR_%d"), dwID);

				for(int t = 0; t < 4; t++) {
					NewTD();
					SetTStyle(_T("font-size"), _T("11"));
					SetTID(_T("GPR_%d_%d"), dwID, t);
				}
			}
		}

		m_bFlushAll		= TRUE;
		UpdateTagAll();
	} else {
		m_bFlushAll		= TRUE;
		UpdateTagName();
	}
}

BOOL RegmapGPRs::OnCommand(LPCTSTR lpszURL)
{
	int iIndex = _ttoi(lpszURL);
	m_Type[iIndex] = (DATA_TYPE)((int)m_Type[iIndex] + 1);

	if(m_Type[iIndex] >= DATA_TYPE_SIZE) m_Type[iIndex] = DATA_TYPE_AUTO;

	{
		m_bFlushAll		= TRUE;
		UpdateTag(iIndex);
		UpdateVector(iIndex);
		m_bFlushAll		= FALSE;
	}
	return FALSE;
}

void RegmapGPRs::UpdateTagAll(void)
{
	for(int i = 0; i < m_iGPRsSize; i++) {
		UpdateTag(i);
	}

	UpdateTagName();
}

void RegmapGPRs::UpdateTagName(void)
{
	SetText(_T("GPRs_tag"), _T("- GPRs (Core #%d, Thread #%d)"), m_pReg->system.iCoreID, m_pReg->system.iThreadID);
}

void RegmapGPRs::UpdateTag(int iIndex)
{
	CString sTarget;
	HString sDesc;
	sTarget.Format(_T("GPR_%d"), iIndex);
	sDesc.Format(_T("R%d.<font size=-4>%s</font>"), iIndex, __TypePreFix[m_Type[iIndex]]);
	sDesc.SetLink(_T("cmd://GPRs/%d"), iIndex);
	SetText(sTarget, sDesc);
}

void RegmapGPRs::UpdateVectorAll(void)
{
	for(int i = 0; i < m_iGPRsSize; i++) {
		UpdateVector(i);
	}

	m_bFlushAll	= FALSE;
}

void RegmapGPRs::UpdateVector(int iIndex)
{
	REG_VALUE4*	pGPRs			= &m_pGPRs->reg[iIndex];
	UINT64*		puiUpdateTime	= m_uiUpdateTime[iIndex];

	for(int i = 0; i < 4; i++) {
		if(m_bFlushAll || (puiUpdateTime[i] != pGPRs->m[i].uUpdateTime)) {
			CString sID;
			sID.Format(_T("GPR_%%d_%d"), i);
			UpdateData(sID, iIndex, pGPRs->m[i].udata, pGPRs->m[i].uUpdateTime);

			if(m_pGPRs->uLastUpdateTime != puiUpdateTime[i]) puiUpdateTime[i] = pGPRs->m[i].uUpdateTime;
		}
	}
}

void RegmapGPRs::UpdateData(LPCTSTR id_format, int iIndex, DWORD dwData, UINT64 uUpdateTime)
{
	CString		sID;
	HString		sData;
	DWORD		dwColor			= 0xAFAFAF;
	DATA_TYPE	type			= m_Type[iIndex];
	sID.Format(id_format, iIndex);
	{
		CString	sNativeData;
	RETRY_TYPE_SET:

		switch(type) {
		case DATA_TYPE_AUTO: {
			if(((dwData & 0xFFFF0000) == 0) || ((dwData & 0xFFFF0000) == 0xFFFF0000))
				type	= DATA_TYPE_INT;
			else if((((dwData >> 23) & 0xFF) < (0x7F + 12)) && (((dwData >> 23) & 0xFF) > (0x7F - 12)))
				type	= DATA_TYPE_FLOAT;
			else
				type	= DATA_TYPE_HEX;
		}

		goto RETRY_TYPE_SET;

		case DATA_TYPE_HEX:
			sNativeData.Format(_T("%04X_%04X"), dwData >> 16, dwData & 0xFFFF);
			break;

		case DATA_TYPE_INT:
			sNativeData.Format(_T("%d"), (int)dwData);
			break;

		case DATA_TYPE_FLOAT:
			if(!(dwData & 0x7FFF8000)) sNativeData	= _T("0");
			else
				sNativeData.Format(_T("%f"), *(float*)&dwData);

			break;

		case DATA_TYPE_BOOLEAN:
			sNativeData	= dwData ? __sBoolean[1] : __sBoolean[0];
			break;
		}

		if(uUpdateTime) {
			dwColor	= (uUpdateTime == m_pGPRs->uLastUpdateTime) ? 0xFF7F7F : 0x7F0000;
		} else {
			sNativeData.Insert(0, _T("<small><small>"));
			sNativeData	+= _T("</small></small>");
		}

		sData.AppendColoredFormat(dwColor, (LPCTSTR)sNativeData);

		if(dwColor == 0xFF7F7F) sData.underline();
	}
	SetText(sID, sData);
}
