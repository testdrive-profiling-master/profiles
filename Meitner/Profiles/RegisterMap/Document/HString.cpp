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
#include "HString.h"

HString::HString(void)
{
}

HString::~HString(void)
{
}

void HString::AppendColoredFormat(DWORD dwColor, LPCTSTR format, ...)
{
	va_list argList;
	va_start(argList, format);
	AppendFormat(_T("<font color='#%06X'>"), dwColor);
	AppendFormatV(format, argList);
	va_end(argList);
	Append(_T("</font>"));
}

void HString::AppendImage(LPCTSTR format, ...)
{
	va_list argList;
	va_start(argList, format);
	AppendFormat(_T("<img style='border:none' src='"));
	AppendFormatV(format, argList);
	va_end(argList);
	Append(_T("'/>"));
}

void HString::SetColor(DWORD dwColor)
{
	CString sColor;
	sColor.Format(_T("<font color='#%06X'>"), dwColor);
	Insert(0, sColor);
	Append(_T("</font>"));
}

void HString::SetLink(LPCTSTR format, ...)
{
	va_list argList;
	Insert(0, _T("'>"));
	va_start(argList, format);
	{
		CString sLink;
		sLink.FormatV(format, argList);
		Insert(0, sLink);
	}
	va_end(argList);
	Insert(0, _T("<a href='"));
	Append(_T("</a>"));
}

void HString::AppendEnter(void)
{
	Append(_T("<br>"));
}

void HString::SetTag(LPCTSTR sTag)
{
	CString str;
	str.Format(_T("<%s>"), sTag);
	Insert(0, str);
	str.Format(_T("</%s>"), sTag);
	Append(str);
}
