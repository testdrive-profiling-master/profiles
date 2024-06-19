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
// Title : Meitner processor register map document
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "RegmapGPRs.h"

static LPCTSTR __sBoolean[2] = {_T("False"), _T("True")};

static LPCTSTR __TypePreFix[] = {_T("a"), _T("i"), _T("f"), _T("h"), _T("b")};

RegmapGPRs::RegmapGPRs(void) : Regmap(_T("GPRs"))
{
	memset(&m_DataType, 0, sizeof(m_DataType));
}

RegmapGPRs::~RegmapGPRs(void) {}

BOOL RegmapGPRs::OnUpdate(void)
{
	BOOL bRet = FALSE;

	if (m_pThread->GPRs.bUpdate) {
		m_pThread->GPRs.bUpdate = false;
		UpdateVectorAll();
		bRet = TRUE;
	}

	return bRet;
}

void RegmapGPRs::OnBroadcast(LPVOID pData)
{
	if (!pData) { // on initialization
		ClearTable();

		for (int i = 0; i < MTSP_GPR_SIZE / 2; i++) {
			NewRow();

			for (int t = 0; t < 2; t++) {
				int index = (t * (MTSP_GPR_SIZE / 2)) + i;
				NewTH();
				SetTClassAdd(_T("text-center"));
				SetTID(_T("GPR_I%d"), index);
				SetTBody(_T("%d"), index);
				NewTD();
				SetTClassAdd(_T("text-end"));
				SetTID(_T("GPR%d_D0"), index);
				NewTD();
				SetTClassAdd(_T("text-end"));
				SetTID(_T("GPR%d_D1"), index);
				NewTD();
				SetTClassAdd(_T("text-end"));
				SetTID(_T("GPR%d_D2"), index);
				NewTD();
				SetTClassAdd(_T("text-end"));
				SetTID(_T("GPR%d_D3"), index);
			}
		}
	}

	/*
	if(!pData) {	// on initialization
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
	}*/
}

BOOL RegmapGPRs::OnCommand(LPCTSTR lpszURL)
{
	if (_tcsstr(lpszURL, _T("type/")) == lpszURL) {
		int iIndex	 = _ttoi(lpszURL + 5);
		int iElement = iIndex & 3;
		iIndex /= 4;
		int iMode = m_DataType[iIndex][iElement] + 1;

		if (iMode >= DATA_TYPE_SIZE)
			iMode = 0;

		m_DataType[iIndex][iElement] = (DATA_TYPE)iMode;
		UpdateData(iIndex, iElement);
	}

	return FALSE;
}

void RegmapGPRs::UpdateTagAll(void)
{
	/*for(int i = 0; i < MTSP_GPR_SIZE; i++) {
		UpdateTag(i);
	}

	UpdateTagName();*/
}

void RegmapGPRs::UpdateTagName(void)
{
	// SetText(_T("GPRs_tag"), _T("- GPRs (Core #%d, Thread #%d)"), m_pReg->system.iCoreID, m_pReg->system.iThreadID);
}

void RegmapGPRs::UpdateVectorAll(void)
{
	for (int i = 0; i < MTSP_GPR_SIZE; i++) {
		UpdateVector(i);
	}
}

void RegmapGPRs::UpdateVector(int iIndex)
{
	REG_VALUE4 *pGPRs = &(m_pThread->GPRs.m[iIndex]);

	for (int i = 0; i < 4; i++) {
		UpdateData(iIndex, i);
	}
}

void RegmapGPRs::UpdateData(int iIndex, int iElement)
{
	CString	   sID;
	HString	   sData;
	DWORD	   dwColor = 0xAFAFAF;
	DATA_TYPE  type	   = m_DataType[iIndex][iElement];
	REG_VALUE &val	   = m_pThread->GPRs.m[iIndex].m[iElement];
	sID.Format(_T("GPR%d_D%d"), iIndex, iElement);
	{
		CString sNativeData;
	RETRY_TYPE_SET:

		switch (type) {
		case DATA_TYPE_AUTO: {
			if (((val.udata & 0xFFFF0000) == 0) || ((val.udata & 0xFFFF0000) == 0xFFFF0000))
				goto __TYPE_INT;
			else if ((((val.udata >> 23) & 0xFF) < (0x7F + 12)) && (((val.udata >> 23) & 0xFF) > (0x7F - 12)))
				goto __TYPE_FLOAT;
			else
				goto __TYPE_HEX;
		}

		case DATA_TYPE_HEX:
		__TYPE_HEX:
			sNativeData.Format(_T("%04X_%04X"), val.udata >> 16, val.udata & 0xFFFF);
			break;

		case DATA_TYPE_INT:
		__TYPE_INT:
			sNativeData.Format(_T("%d"), val.idata);
			break;

		case DATA_TYPE_FLOAT:
		__TYPE_FLOAT:
			if (!(val.udata & 0x7FFF8000))
				sNativeData = _T("0");
			else
				sNativeData.Format(_T("%f"), val.fdata);

			break;

		case DATA_TYPE_BOOLEAN:
			sNativeData = val.udata ? __sBoolean[1] : __sBoolean[0];
			break;
		}

		if (val.uUpdateID) {
			dwColor = (val.uUpdateID == m_pThread->GPRs.uLastUpdateID) ? 0xFF7F7F : 0x7F0000;
		}

		sData.AppendColoredFormat(dwColor, (LPCTSTR)sNativeData);

		if (dwColor == 0xFF7F7F)
			sData.underline();

		sData.AppendFormat(_T(".<small><a href='cmd://GPRs/type/%d'>%s</a></small>"), (iIndex * 4) + iElement,
						   __TypePreFix[type]);
	}
	SetText(sID, sData);
}

bool RegmapGPRs::IsValidID(uint32_t id)
{
	return (id & 0x80000000) == 0;
}
