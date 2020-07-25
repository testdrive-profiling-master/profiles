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
#include "Regmap.h"

Regmap*			Regmap::m_pHead		= NULL;
MTSP_REGMAP*	Regmap::m_pReg		= NULL;

Regmap::Regmap(LPCTSTR sName)
{
	m_pNext			= m_pHead;
	m_pHead			= this;
	m_sName			= sName;
	m_bTableNewRow	= NULL;
}

Regmap::~Regmap(void)
{
	if(m_pNext) {
		delete m_pNext;
		m_pNext	= NULL;
	}
}

void Regmap::ReleaseAll(void)
{
	if(m_pHead) {
		delete m_pHead;
		m_pHead	= NULL;
	}
}

BOOL Regmap::Update(void)
{
	BOOL	bUpdate	= FALSE;
	Regmap* pRegmap	= m_pHead;

	if(m_pReg->magic_code == SYSTEM_MAGIC_CODE)	// check correct register map.
		while(pRegmap) {
			if(pRegmap->OnUpdate()) bUpdate = TRUE;

			pRegmap	= pRegmap->m_pNext;
		}

	return bUpdate;
}

void Regmap::PostUpdate(void)
{
	PostMessage(g_pDoc->GetWindowHandle(), WM_TIMER, 10, 0);
}

void Regmap::Command(LPCTSTR lpszURL)
{
	Regmap* pRegmap	= m_pHead;

	while(pRegmap) {
		if(pRegmap->Name() && (_tcsstr(lpszURL, pRegmap->Name()) == lpszURL)) {
			pRegmap->OnCommand(lpszURL + _tcslen(pRegmap->Name()) + 1);
			break;
		}

		pRegmap	= pRegmap->m_pNext;
	}
}

void Regmap::Initialize(void)
{
	Broadcast(NULL);
}

void Regmap::Broadcast(LPVOID pData)
{
	Regmap* pRegmap	= m_pHead;

	while(pRegmap) {
		pRegmap->OnBroadcast(pData);
		pRegmap	= pRegmap->m_pNext;
	}
}

void Regmap::OnBroadcast(LPVOID pData)
{
}

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

void Regmap::ClearTable(void)
{
	g_pHtml->CallJScript(_T("ClearTable('%s');"), Name());
	m_bTableNewRow	= TRUE;
}

void Regmap::AppendCell(LPCTSTR sCell, LPCTSTR sBody)
{
	if(m_bTableNewRow) {
		g_pHtml->CallJScript(_T("NewRow();"));
		m_bTableNewRow	= FALSE;
	}

	g_pHtml->CallJScript(_T("NewElement(\"<%s>\");"), sCell);

	if(sBody) g_pHtml->CallJScript(_T("SetTBody(\"%s\");"), sBody);

	g_pHtml->CallJScript(_T("AppendCell();"));
}

void Regmap::NewRow(void)
{
	g_pHtml->CallJScript(_T("NewRow(\"%s\");"), Name());
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
	static LPCTSTR __sAlign[] = {
		_T("left"),
		_T("right"),
		_T("center")
	};
	SetTStyle(_T("text-align"), __sAlign[(int)align - 1]);
}

void Regmap::SetBoarderWidth(TABLE_BOARDER side, int iWidth)
{
	static LPCTSTR __SIDE[] = {
		_T("left"),
		_T("right"),
		_T("top"),
		_T("bottom")
	};
	g_pHtml->CallJScript(_T("SetTStyle(\"border-%s\",\"%dpx\");"), __SIDE[side], iWidth);
}

void Regmap::AppendRow(void)
{
	g_pHtml->CallJScript(_T("AppendRow(\"%s\");"), Name());
	m_bTableNewRow	= TRUE;
}

CString GetBitsString(DWORD data, DWORD bits_count)
{
	CString str;
	data <<= 32 - bits_count;

	for(int i = bits_count - 1; i >= 0; i--) {
		str.AppendChar(data & 0x80000000 ? _T('1') : _T('0'));

		if(!(i % 4) && i != 0) str.AppendChar(_T('_'));

		data <<= 1;
	}

	return str;
}

#define	MAX_BUFFER_FORMAT	60

const DWORD g_dwBufferFormatCode[MAX_BUFFER_FORMAT] = {
	0x321501E5,
	0x321001E5,
	0x321003E5,
	0x21050194,
	0x321001D4,
	0x321001C4,
	0x00050533,
	0x321500E5,
	0x321000E5,
	0x321002E5,
	0x00050433,
	0x55500033,
	0x00050400,
	0x210501E5,
	0x210301E5,
	0x210303E5,
	0x210301B4,
	0x210301C4,
	0x210500E5,
	0x210300E5,
	0x210302E5,
	0x123501E5,
	0x123001E5,
	0x123003E5,
	0x01250194,
	0x123001D4,
	0x123001C4,
	0x123500E5,
	0x123000E5,
	0x123002E5,
	0x012501E5,
	0x012301E5,
	0x012303E5,
	0x012301B4,
	0x012301C4,
	0x012500E5,
	0x012300E5,
	0x012302E5,
	0x44400033,
	0x00050033,
	0x00010064,
	0x21050094,
	0x321000C4,
	0x321000D4,
	0x012300E5,
	0x321000E5,
	0x210300E5,
	0x123000E5,
	0x00000033,
	0x00000044,
	0x00000055,
	0x000000F5,
	0x00000033,
	0x013208E5,
	0x031208E5,
	0x102308E5,
	0x120308E5,
	0x00050400,
	0x00050411,
	0x00050422
};

const LPCTSTR g_sBufferFormatString[MAX_BUFFER_FORMAT] = {
	_T("VG_sRGBX_8888"),
	_T("VG_sRGBA_8888"),
	_T("VG_sRGBA_8888_PRE"),
	_T("VG_sRGB_565"),
	_T("VG_sRGBA_5551"),
	_T("VG_sRGBA_4444"),
	_T("VG_sL_8"),
	_T("VG_lRGBX_8888"),
	_T("VG_lRGBA_8888"),
	_T("VG_lRGBA_8888_PRE"),
	_T("VG_lL_8"),
	_T("VG_A_8"),
	_T("VG_BW_1"),
	_T("VG_sXRGB_8888"),
	_T("VG_sARGB_8888"),
	_T("VG_sARGB_8888_PRE"),
	_T("VG_sARGB_1555"),
	_T("VG_sARGB_4444"),
	_T("VG_lXRGB_8888"),
	_T("VG_lARGB_8888"),
	_T("VG_lARGB_8888_PRE"),
	_T("VG_sBGRX_8888"),
	_T("VG_sBGRA_8888"),
	_T("VG_sBGRA_8888_PRE"),
	_T("VG_sBGR_565"),
	_T("VG_sBGRA_5551"),
	_T("VG_sBGRA_4444"),
	_T("VG_lBGRX_8888"),
	_T("VG_lBGRA_8888"),
	_T("VG_lBGRA_8888_PRE"),
	_T("VG_sXBGR_8888"),
	_T("VG_sABGR_8888"),
	_T("VG_sABGR_8888_PRE"),
	_T("VG_sABGR_1555"),
	_T("VG_sABGR_4444"),
	_T("VG_lXBGR_8888"),
	_T("VG_lABGR_8888"),
	_T("VG_lABGR_8888_PRE"),
	_T("GL_ALPHA"),
	_T("GL_LUMINANCE"),
	_T("GL_LUMINANCE_ALPHA"),
	_T("GL_RGB_565"),
	_T("GL_RGBA_4444"),
	_T("GL_RGBA_5551"),
	_T("GL_RGBA_8888_REV"),
	_T("GL_RGBA_8888"),
	_T("GL_BGRA_8888_REV"),
	_T("GL_BGRA_8888"),
	_T("GL_STENCIL_8"),
	_T("GL_DEPTH_16"),
	_T("GL_DEPTH_32"),
	_T("GL_DEPTH_FLOAT"),
	_T("GL_INTENSITY"),
	_T("YUYV_8888"),
	_T("YVYU_8888"),
	_T("UYVY_8888"),
	_T("VYUY_8888"),
	_T("BW_1"),
	_T("BW_2"),
	_T("BW_4")
};

LPCTSTR g_sInvalid = _T("Invalid");

LPCTSTR g_sBoolean[2] = {
	_T("False"),
	_T("True")
};

LPCTSTR g_s4DElement[4] = {
	_T("X"),
	_T("Y"),
	_T("Z"),
	_T("W"),
};

LPCTSTR GetBufferFormatString(DWORD format)
{
	for(int i = 0; i < MAX_BUFFER_FORMAT; i++)
		if(g_dwBufferFormatCode[i] == format) return g_sBufferFormatString[i];

	return g_sInvalid;
}

float Fixed2Float(DWORD data, BOOL bUseSign, int bits_high, int bits_low)
{
	float	fData;
	DWORD	mask;
	mask	= 1 << bits_low;
	fData	= ((float)(data & (mask - 1))) / (float)mask;
	data	>>= bits_low;
	mask	= 1 << bits_high;
	fData	+= (float)(data & (mask - 1));

	if(bUseSign)
		if(data & mask) fData = -fData;

	return fData;
}

void PresentTime(CString& sTime, UINT64	llTime)
{
	// present simulation time.
	{
		static LPCTSTR	_time_unit[] = {_T("ps"), _T("ns"), _T("us"), _T("ms"), _T("s"), _T("m"), _T("h")};
		DWORD		dwTimeBuff[7];
		int			iMaxTimeScale	= 0;

		for(; iMaxTimeScale < 7; iMaxTimeScale++) {
			switch(iMaxTimeScale) {
			case 4:	// 's' second
			case 5: // 'm' minute
				dwTimeBuff[iMaxTimeScale]	= llTime % 60;
				llTime /= 60;
				break;

			case 6:	// 'h' hour
				dwTimeBuff[iMaxTimeScale]	= llTime % 24;
				llTime /= 24;
				break;

			default:	// ps, ns, us, ms
				dwTimeBuff[iMaxTimeScale]	= llTime % 1000;
				llTime /= 1000;
				break;
			}

			if(!llTime)	break;
		}

		for(int i = iMaxTimeScale; i >= 0; i--) {
			BOOL bFirst	= (i == iMaxTimeScale);

			if(dwTimeBuff[i] || bFirst) {
				if(!bFirst) sTime.Append(_T(" "));

				sTime.AppendFormat((i == iMaxTimeScale) || (i > 3) ? _T("%d") : _T("%d"), dwTimeBuff[i]);
				sTime.Append(_time_unit[i]);
			}
		}
	}
}
