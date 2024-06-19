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
#include "RegmapMB.h"

#define MB_SHOWUP_SIZE 16

RegmapMB::RegmapMB(LPCTSTR sName) : Regmap(sName)
{
	m_iStartIndex = 0;
	m_iHoverIndex = 0;
	m_sTrace.Format(_T("TRACE_%s"), sName);
	m_bTrace = g_pDoc->GetConfigInt(m_sTrace, FALSE);
}

RegmapMB::~RegmapMB(void)
{
	g_pDoc->SetConfigInt(m_sTrace, m_bTrace);
}

void RegmapMB::OnBroadcast(LPVOID pData)
{
	switch (*(REGMAP_BROADCAST *)(&pData)) {
	case REGMAP_BROADCAST_INITIALIZATION:
		InitTable();
		break;

	case REGMAP_BROADCAST_SYSTEM_REBOOT:
		UpdateIndex(true);
		UpdateVectors();
		break;
	}
}

BOOL RegmapMB::OnCommand(LPCTSTR lpszURL)
{
	if (_tcsstr(lpszURL, _T("offset/")) == lpszURL) {
		m_iStartIndex = _tstoi(lpszURL + 7);
		UpdateIndex();
		UpdateVectors(false);
	} else if (_tcsstr(lpszURL, _T("scroll/")) == lpszURL) {
		int iNewIndex = m_iStartIndex + _tstoi(lpszURL + 7);

		if (iNewIndex < 0)
			iNewIndex = 0;
		else if (iNewIndex > (GetMaxCount() - MB_SHOWUP_SIZE))
			iNewIndex = GetMaxCount() - MB_SHOWUP_SIZE;

		if (iNewIndex != m_iStartIndex) {
			m_iStartIndex = iNewIndex;
			UpdateIndex(true);
			UpdateVectors(false);
			OnHoverData();
		}
	} else if (!_tcscmp(lpszURL, _T("latest"))) {
		// search latest changed data
		if (TraceLatest(true)) {
			UpdateIndex(true);
			UpdateVectors();
		}
	} else if (_tcsstr(lpszURL, _T("trace/")) == lpszURL) {
		m_bTrace = _tstoi(lpszURL + 6);
	} else if (_tcsstr(lpszURL, _T("cell/")) == lpszURL) {
		OnHoverData(_tstoi(lpszURL + 5));
	}

	return TRUE;
}

void RegmapMB::InitTable(void)
{
	ClearTable();
	m_iHoverIndex = 0;

	// setup Header : index, table #
	for (int i = 0; i < MB_SHOWUP_SIZE; i++) {
		NewRow();
		NewTH();
		SetTClassAdd(_T("text-center"));
		SetTID(_T("%s_I%d"), Name(), i);

		for (int t = 0; t < 4; t++) {
			NewTD();
			SetTID(_T("%s_D%d_%d"), Name(), i, 3 - t);
		}

		if (!i) {
			NewTH();
			SetRowSpan(MB_SHOWUP_SIZE);
			SetTClassAdd(_T("no-hover"));
			SetTID(_T("%s_slide"), Name());
		}
	}

	if (m_bTrace)
		g_pHtml->CallJScript(_T("$('#%s_trace').prop('checked', true);"), Name());

	UpdateIndex(true);
	UpdateVectors();
}

void RegmapMB::UpdateIndex(bool bUpdateSlider)
{
	for (int i = 0; i < MB_SHOWUP_SIZE; i++) {
		CString sID, sIndex;
		sID.Format(_T("%s_I%d"), Name(), i);
		sIndex.Format(_T("%d"), i + m_iStartIndex);
		SetText(sID, sIndex);
	}

	if (bUpdateSlider) {
		// setup slider
		CString sID, sSlide;
		int		iMax = GetMaxCount();
		sID.Format(_T("%s_slide"), Name());
		sSlide.Format(_T("<div><input type='range' class='form-range' min='0' max='%d' value='%d' id='%s_slider' ")
					  _T("oninput='post_message(\\\"%s/offset/\\\" + this.value);'></div>"),
					  iMax - MB_SHOWUP_SIZE, m_iStartIndex, Name(), Name());
		SetText(sID, sSlide);
	}
}

void RegmapMB::UpdateVectors(bool bUseTrace)
{
	if (m_bTrace && bUseTrace) {
		if (TraceLatest()) {
			UpdateIndex(true);
		}
	}

	int		 iIndex		   = m_iStartIndex;
	uint32_t uLastUpdateID = GetLastUpdateID();

	for (int i = 0; i < MB_SHOWUP_SIZE; i++) {
		REG_VALUE4 *pValue = GetValue(iIndex + i);

		for (int t = 0; t < 4; t++) {
			CString	 sID;
			HString	 sData;
			uint32_t uUpdateID = pValue->m[t].uUpdateID;
			sID.Format(_T("%s_D%d_%d"), Name(), i, t);
			sData.Format(_T("%08X"), pValue->m[t].udata);

			if (!IsValidID(uUpdateID)) {
				if (!uUpdateID)
					sData.SetColor(0xCFCFCF);
				else {
					sData.SetColor(0xAFAFAF);
					sData.bold();
				}
			} else if (uUpdateID == uLastUpdateID) {
				sData.SetColor(0xFF0000);
				sData.underline();
			}

			SetText(sID, sData);
		}
	}
}

bool RegmapMB::IsValidID(uint32_t id)
{
	return id > 0;
}

static void __commify(CString &sNum)
{
	int iLen   = sNum.length();
	int iPoint = sNum.Find(_T('.'));

	if (iPoint < 0)
		iPoint = iLen;

	while (iPoint > 0) {
		iPoint -= 3;

		if ((iPoint <= 0) || (iPoint == 1 && !_istdigit(sNum.GetAt(0))))
			break;

		sNum.Insert(iPoint, _T(','));
	}
}

void RegmapMB::OnHoverData(int iIndex)
{
	if (iIndex == -1)
		iIndex = m_iHoverIndex;
	else
		m_iHoverIndex = iIndex;

	static LPCTSTR __sCell = _T("xyzw");
	int			   iOffset = m_iStartIndex + (iIndex >> 2);
	REG_VALUE4	  *v	   = GetValue(iOffset);
	CString		   sMsg;
	g_pHtml->CallJScript(_T("$('#result_id').html('<b>%s</b>[%d]');"), Name(), iOffset);

	for (int i = 0; i < 4; i++) {
		CString	   sVal;
		REG_VALUE &m = v->m[i];
		sVal.Format(_T("%3d[%02X] %3d[%02X] %3d[%02X] %3d[%02X]"), m.bdata[3], m.bdata[3], m.bdata[2], m.bdata[2],
					m.bdata[1], m.bdata[1], m.bdata[0], m.bdata[0]);
		sVal.Replace(_T(" "), _T("&nbsp;"));
		sVal.Replace(_T("["), _T("<font color=\"#BFBFBF\">["));
		sVal.Replace(_T("]"), _T("]</font>"));
		g_pHtml->CallJScript(_T("$('#%c_hex').html('%s');"), __sCell[i], (LPCTSTR)sVal);
		sVal.Format(_T("%u"), m.udata);
		__commify(sVal);
		g_pHtml->CallJScript(_T("$('#%c_uint').html('%s');"), __sCell[i], (LPCTSTR)sVal);
		sVal.Format(_T("%d"), m.idata);
		__commify(sVal);
		g_pHtml->CallJScript(_T("$('#%c_int').html('<font color=\"#%06X\">%s</font>');"), __sCell[i],
							 (__sCell[i], m.idata < 0) ? 0xFF0000 : 0x0000FF, (LPCTSTR)sVal);

		if ((m.fdata != 0) && !isnan(m.fdata) && !isinf(m.fdata) &&
			(abs(m.fdata) > 1000000000.f || abs(m.fdata) <= 0.0000001f)) {
			sVal.Format(_T("%e"), m.fdata);
			sVal.Replace(_T("e"), _T("<b>e</b>"));
			g_pHtml->CallJScript(_T("$('#%c_fp').html('<font color=\"#%06X\">%s</font>');"), __sCell[i],
								 (__sCell[i], m.fdata < 0) ? 0xFF0000 : 0x0000FF, (LPCTSTR)sVal);
		} else {
			sVal.Format(_T("%f"), m.fdata);
			__commify(sVal);
			g_pHtml->CallJScript(_T("$('#%c_fp').html('<font color=\"#%06X\">%s</font>');"), __sCell[i],
								 (__sCell[i], m.fdata < 0) ? 0xFF0000 : 0x0000FF, (LPCTSTR)sVal);
		}

		if (!(i & 1)) {
			union {
				int32_t i32[2];
				double	f64;
				int64_t i64;
			} data;
			data.i32[0] = m.udata;
			data.i32[1] = v->m[i + 1].udata;
			CString iVal;
			iVal.Format(_T("%lld"), data.i64);
			__commify(iVal);
			iVal.Insert(0, data.i64 >= 0 ? _T("<font color=\"#0000FF\">") : _T("<font color=\"#FF0000\">"));
			iVal += _T("</font><br>");

			if ((data.f64 != 0) && !isnan(data.f64) && !isinf(data.f64) &&
				(abs(data.f64) > 1000000000.f || abs(data.f64) <= 0.0000001f)) {
				sVal.Format(_T("%e"), data.f64);
				sVal.Replace(_T("e"), _T("<b>e</b>"));
				g_pHtml->CallJScript(_T("$('#%c_dp').html('%s<font color=\"#%06X\">%s</font>');"), __sCell[i],
									 (LPCTSTR)iVal, (data.f64 < 0) ? 0xFF0000 : 0x0000FF, (LPCTSTR)sVal);
			} else {
				sVal.Format(_T("%f"), data.f64);
				__commify(sVal);
				g_pHtml->CallJScript(_T("$('#%c_dp').html('%s<font color=\"#%06X\">%s</font>');"), __sCell[i],
									 (LPCTSTR)iVal, (data.f64 < 0) ? 0xFF0000 : 0x0000FF, (LPCTSTR)sVal);
			}
		}
	}
}

bool RegmapMB::TraceLatest(bool bLog)
{
	uint32_t uLastUpdateID = GetLastUpdateID();

	if (uLastUpdateID) {
		int iMaxCount = GetMaxCount();

		for (int i = 0; i < iMaxCount; i++) {
			REG_VALUE4 *pValue = GetValue(i);

			for (int t = 0; t < 4; t++) {
				if (uLastUpdateID == pValue->m[t].uUpdateID) {
					int iStartIndex = i - MB_SHOWUP_SIZE / 2; // move to center

					if (iStartIndex < 0)
						iStartIndex = 0;
					else if (iStartIndex > iMaxCount - MB_SHOWUP_SIZE)
						iStartIndex = iMaxCount - MB_SHOWUP_SIZE;

					if (iStartIndex != m_iStartIndex) {
						m_iStartIndex = iStartIndex;
						UpdateIndex(true);
						UpdateVectors();

						if (bLog)
							g_pHtml->CallJScript(_T("toastr['warning']('[%s#%d] %s');"), Name(), i,
												 _L(FOUND_LATEST_CHANGE));
					}

					return true;
				}
			}
		}
	} else if (bLog) {
		g_pHtml->CallJScript(_T("toastr['warning']('[%s] %s');"), Name(), _L(NO_LATEST_CHANGE));
	}

	return false;
}
