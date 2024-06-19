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
#include "Regmap.h"
#include <filesystem>

Regmap			   *Regmap::m_pHead			  = NULL;
MTSP_REGMAP		   *Regmap::m_pReg			  = NULL;
MTSP_REGMAP_CORE   *Regmap::m_pCore			  = NULL;
MTSP_REGMAP_THREAD *Regmap::m_pThread		  = NULL;
UINT64				Regmap::m_lSimulationTime = 0;
ENV					Regmap::env				  = {FALSE, FALSE, "", 0};

Regmap::Regmap(LPCTSTR sName)
{
	m_pNext		   = m_pHead;
	m_pHead		   = this;
	m_sName		   = sName;
	m_sTableName   = sName;
	m_bTableNewRow = NULL;
}

Regmap::~Regmap(void)
{
	if (m_pNext) {
		delete m_pNext;
		m_pNext = NULL;
	}
}

void Regmap::ReleaseAll(void)
{
	if (m_pHead) {
		delete m_pHead;
		m_pHead = NULL;
	}
}

BOOL Regmap::Update(void)
{
	BOOL	bUpdate = FALSE;
	Regmap *pRegmap = m_pHead;

	if (m_pReg->magic_code == MTSP_MAGIC_CODE) { // check correct register map.
		while (pRegmap) {
			if (pRegmap->OnUpdate())
				bUpdate = TRUE;

			pRegmap = pRegmap->m_pNext;
		}

		UpdateSimulationTime();
	}

	return bUpdate;
}

void Regmap::PostUpdate(void)
{
	PostMessage(g_pDoc->GetWindowHandle(), WM_TIMER, 10, 0);
}

void Regmap::Command(LPCTSTR lpszURL)
{
	Regmap *pRegmap = m_pHead;

	while (pRegmap) {
		if (pRegmap->Name() && (_tcsstr(lpszURL, pRegmap->Name()) == lpszURL)) {
			pRegmap->OnCommand(lpszURL + _tcslen(pRegmap->Name()) + 1);
			break;
		}

		pRegmap = pRegmap->m_pNext;
	}
}

void Regmap::Initialize(void)
{
	Broadcast(NULL);
}

void Regmap::Broadcast(LPVOID pData)
{
	Regmap *pRegmap = m_pHead;

	while (pRegmap) {
		pRegmap->OnBroadcast(pData);
		pRegmap = pRegmap->m_pNext;
	}
}

void RetrieveSimulationTime(CString &sTime, UINT64 lTime)
{
	if (!lTime) {
		sTime = _T("0 ps");
	} else {
		int ps = lTime % 1000;
		lTime /= 1000;

		if (lTime) {
			int ns = lTime % 1000;
			lTime /= 1000;

			if (lTime) {
				int us = lTime % 1000;
				lTime /= 1000;

				if (lTime) {
					int ms = lTime % 1000;
					lTime /= 1000;

					if (lTime) {
						int s = lTime % 1000;
						lTime /= 60;

						if (lTime)
							sTime.AppendFormat(_T("%lld min"), lTime);

						if (s)
							sTime.AppendFormat(_T(" %d sec"), s);
					}

					if (ms)
						sTime.AppendFormat(_T(" %d ms"), ms);
				}

				if (us)
					sTime.AppendFormat(_T(" %d us"), us);
			}

			if (ns)
				sTime.AppendFormat(_T(" %d ns"), ns);
		}

		if (ps)
			sTime.AppendFormat(_T(" %d ps"), ps);
	}
}

void RetrieveBytes(CString &sBytes, UINT64 lByteSize)
{
	if (lByteSize > 1024 * 1024 * 1024) {
		sBytes.Format(_T("%.2f GB"), lByteSize / (double)(1024 * 1024 * 1024));
	} else if (lByteSize > 1024 * 1024) {
		sBytes.Format(_T("%.1f MB"), lByteSize / (double)(1024 * 1024));
	} else if (lByteSize > 1024) {
		sBytes.Format(_T("%.1f KB"), lByteSize / 1024.0);
	} else {
		sBytes.Format(_T("%lld Byte"), lByteSize);
	}
}

void Regmap::UpdateSimulationTime(void)
{
	if (m_lSimulationTime != m_pReg->lSimulationTime) {
		UINT64	lTime = m_pReg->lSimulationTime;
		CString sTime;
		m_lSimulationTime = lTime;
		RetrieveSimulationTime(sTime, lTime);
		g_pHtml->CallJScript(_T("$('#sim_time').val('%s');"), (LPCTSTR)sTime);
	}
}

void Regmap::OnBroadcast(LPVOID pData) {}

BOOL Regmap::OnCommand(LPCTSTR lpszURL)
{
	return FALSE;
}

void Regmap::SetText(LPCTSTR lpszTarget, LPCTSTR lpszFormat, ...)
{
	CString cmd;
	{
		CString str;
		va_list args;
		va_start(args, lpszFormat);
		str.FormatV(lpszFormat, args);
		va_end(args);
		cmd.Format(_T("SetBody(\"%s\", \"%s\");"), lpszTarget, str);
	}
	g_pHtml->CallJScript(cmd);
}

void Regmap::SetTableID(LPCTSTR sTableName)
{
	m_sTableName = sTableName;
}

void Regmap::ClearTable(LPCTSTR sTableName)
{
	g_pHtml->CallJScript(_T("ClearTable('%s');"), sTableName ? sTableName : (LPCTSTR)m_sTableName);
	m_bTableNewRow = TRUE;
}

void Regmap::AppendCell(LPCTSTR sCell, LPCTSTR sBody)
{
	if (m_bTableNewRow) {
		g_pHtml->CallJScript(_T("NewRow();"));
		m_bTableNewRow = FALSE;
	}

	g_pHtml->CallJScript(_T("NewElement(\"<%s>\");"), sCell);

	if (sBody)
		g_pHtml->CallJScript(_T("SetTBody(\"%s\");"), sBody);

	g_pHtml->CallJScript(_T("AppendCell();"));
}

void Regmap::NewRow(LPCTSTR sTableName)
{
	g_pHtml->CallJScript(_T("NewRow(\"%s\");"), (!sTableName) ? (LPCTSTR)m_sTableName : sTableName);
}

void Regmap::NewTD(void)
{
	g_pHtml->CallJScript(_T("NewTD();"));
}

void Regmap::NewTH(void)
{
	g_pHtml->CallJScript(_T("NewTH();"));
}

void Regmap::SetTBody(LPCTSTR lpszFormat, ...)
{
	CString str;
	va_list args;
	va_start(args, lpszFormat);
	str.FormatV(lpszFormat, args);
	va_end(args);
	g_pHtml->CallJScript(_T("SetTBody(\"%s\");"), (LPCTSTR)str);
}

void Regmap::SetTStyle(LPCTSTR sTarget, LPCTSTR sAttrib)
{
	g_pHtml->CallJScript(_T("SetTStyle(\"%s\", \"%s\");"), sTarget, sAttrib);
}

void Regmap::SetTBorderRight(LPCTSTR sStyle)
{
	g_pHtml->CallJScript(_T("SetTBorderRight(\"%s\");"), sStyle);
}
void Regmap::SetTBorderLeft(LPCTSTR sStyle)
{
	g_pHtml->CallJScript(_T("SetTBorderLeft(\"%s\");"), sStyle);
}
void Regmap::SetTBorderTop(LPCTSTR sStyle)
{
	g_pHtml->CallJScript(_T("SetTBorderTop(\"%s\");"), sStyle);
}
void Regmap::SetTBorderBottom(LPCTSTR sStyle)
{
	g_pHtml->CallJScript(_T("SetTBorderBottom(\"%s\");"), sStyle);
}
void Regmap::SetTClassAdd(LPCTSTR sName)
{
	g_pHtml->CallJScript(_T("SetTClassAdd(\"%s\");"), sName);
}
void Regmap::SetTClassRemove(LPCTSTR sName)
{
	g_pHtml->CallJScript(_T("SetTClassRemove(\"%s\");"), sName);
}

void Regmap::SetTID(LPCTSTR lpszFormat, ...)
{
	CString str;
	va_list args;
	va_start(args, lpszFormat);
	str.FormatV(lpszFormat, args);
	va_end(args);
	g_pHtml->CallJScript(_T("SetTID(\"%s\");"), (LPCTSTR)str);
}

void Regmap::SetColSpan(int iSize)
{
	g_pHtml->CallJScript(_T("SetTColSpan(%d);"), iSize);
}

void Regmap::SetRowSpan(int iSize)
{
	g_pHtml->CallJScript(_T("SetTRowSpan(%d);"), iSize);
}

void Regmap::SetTextAlignment(TEXT_ALIGN align)
{
	static LPCTSTR __sAlign[] = {_T("left"), _T("right"), _T("center")};
	SetTStyle(_T("text-align"), __sAlign[(int)align - 1]);
}

void Regmap::SetBoarderWidth(TABLE_BOARDER side, int iWidth)
{
	static LPCTSTR __SIDE[] = {_T("left"), _T("right"), _T("top"), _T("bottom")};
	g_pHtml->CallJScript(_T("SetTStyle(\"border-%s\",\"%dpx\");"), __SIDE[side], iWidth);
}

void Regmap::SetCheckBox(LPCTSTR sName, BOOL bCheck)
{
	g_pHtml->CallJScript(_T("$('#%s').prop('checked', %s);"), sName, bCheck ? _T("true") : _T("false"));
}

void Regmap::ShowCollapse(LPCTSTR sName, BOOL bShow)
{
	g_pHtml->CallJScript(_T("$('#%s').collapse('%s');"), sName, bShow ? _T("show") : _T("hide"));
}

CString GetBitsString(DWORD data, DWORD bits_count)
{
	CString str;
	data <<= 32 - bits_count;

	for (int i = bits_count - 1; i >= 0; i--) {
		str.AppendChar(data & 0x80000000 ? _T('1') : _T('0'));

		if (!(i % 4) && i != 0)
			str.AppendChar(_T('_'));

		data <<= 1;
	}

	return str;
}

#define MAX_BUFFER_FORMAT 60

LPCTSTR g_sInvalid = _T("Invalid");

LPCTSTR g_sBoolean[2] = {_T("False"), _T("True")};

LPCTSTR g_s4DElement[4] = {
	_T("X"),
	_T("Y"),
	_T("Z"),
	_T("W"),
};

float Fixed2Float(DWORD data, BOOL bUseSign, int bits_high, int bits_low)
{
	float fData;
	DWORD mask;
	mask  = 1 << bits_low;
	fData = ((float)(data & (mask - 1))) / (float)mask;
	data >>= bits_low;
	mask = 1 << bits_high;
	fData += (float)(data & (mask - 1));

	if (bUseSign)
		if (data & mask)
			fData = -fData;

	return fData;
}

void PresentTime(CString &sTime, UINT64 llTime)
{
	// present simulation time.
	{
		static LPCTSTR _time_unit[] = {_T("ps"), _T("ns"), _T("us"), _T("ms"), _T("s"), _T("m"), _T("h")};
		DWORD		   dwTimeBuff[7];
		int			   iMaxTimeScale = 0;

		for (; iMaxTimeScale < 7; iMaxTimeScale++) {
			switch (iMaxTimeScale) {
			case 4: // 's' second
			case 5: // 'm' minute
				dwTimeBuff[iMaxTimeScale] = llTime % 60;
				llTime /= 60;
				break;

			case 6: // 'h' hour
				dwTimeBuff[iMaxTimeScale] = llTime % 24;
				llTime /= 24;
				break;

			default: // ps, ns, us, ms
				dwTimeBuff[iMaxTimeScale] = llTime % 1000;
				llTime /= 1000;
				break;
			}

			if (!llTime)
				break;
		}

		for (int i = iMaxTimeScale; i >= 0; i--) {
			BOOL bFirst = (i == iMaxTimeScale);

			if (dwTimeBuff[i] || bFirst) {
				if (!bFirst)
					sTime.Append(_T(" "));

				sTime.AppendFormat((i == iMaxTimeScale) || (i > 3) ? _T("%d") : _T("%d"), dwTimeBuff[i]);
				sTime.Append(_time_unit[i]);
			}
		}
	}
}
