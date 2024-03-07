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
// Title : Common profiles
// Rev.  : 3/7/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifdef EXCEL_NUM_FORMAT_TEST
#	include "ExcelNumFormat.h"
using namespace excel_number_format;

bool DoFormat(const char *sFormat, double fValue, const char *sValue = NULL)
{
	ExcelNumFormat fmt(sFormat);
	if (!fmt.IsValid()) {
		printf("*E : not valid format ['%s']\n", sFormat);
		return false;
	}
	printf("	: Excel Format('%s', %.9f) = '%s'\n", sFormat, fValue, fmt.Format(fValue, sValue).c_str());
	string sColor = fmt.Color();
	if (!sColor.empty()) {
		printf("	  -> color : %s\n", sColor.c_str());
	}

	return true;
}

void Excel_DateToYMD(int iDate, int &iYear, int &iMonth, int &iDay)
{
	// Modified Julian to YMD calculation with an addition of 2415019
	int l  = iDate + 68569 + 2415019;
	int n  = int((4 * l) / 146097);
	l	   = l - int((146097 * n + 3) / 4);
	int i  = int((4000 * (l + 1)) / 1461001);
	l	   = l - int((1461 * i) / 4) + 31;
	int j  = int((80 * l) / 2447);
	iDay   = l - int((2447 * j) / 80);
	l	   = int(j / 11);
	iMonth = j + 2 - (12 * l);
	iYear  = 100 * (n - 49) + i + l;
}

int main(int argc, const char *argv[])
{
	DoFormat("#,##0.0", 1120.12);
	DoFormat("#/##", 0.32);
	DoFormat("#0;[red]#0", -3.12);
	DoFormat("#.00;(#.00)", -302.1);
	DoFormat("#,##0,,\" M\"", -12545678);
	DoFormat("#,##0.0%", -0.12);
	DoFormat("#,##0;-#,##0;", 0); // hide zero
	DoFormat("0.0#\"°C\"", 12.32);
	DoFormat("#,##0 %", 23112.321);
	DoFormat("mm/dd/yyyy dddd h:mm AM/PM", 24862.283587963); // 01/25/1968 Thursday 6:48 AM
	DoFormat("[h]:mm:ss", 0.750833333);						 // 18:01:72
	DoFormat("\"Mr.\" @", 0.750833333, "Marty");
	DoFormat("#,##0.0;#,##0.0;", 0.750833333, "Marty");
	DoFormat("#,##0.0;#,##0.0;#;@\"'s item\"", 0.750833333, "Marty");
	return 0;
}

#endif
