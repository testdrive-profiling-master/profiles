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
// Title : Scenario test
// Rev.  : 4/30/2024 Tue (clonextop@gmail.com)
//================================================================================
#include "TestVector.h"
#include "ScenarioTest.inl"
#include "TestGroup.h"
#include <float.h>
#include <stdlib.h>

LPCTSTR	   g_sAppName = _T("Scenario");
LPCTSTR	   g_sEmpty	  = _T("");

CString	   TestResource::m_sProgramPath;
CString	   TestResource::m_sScenarioPath;
CString	   TestResource::m_sLogPath;
HtmlTable *TestResource::m_pHtmlTable		 = NULL;
BOOL	   TestResource::m_bSuppressWaveform = TRUE;
TestResource::TestResource(void) {}
TestResource::~TestResource(void) {}

TestVector *TestVector::m_pCurrentTestVector = NULL;

TestVector::TestVector(TestGroup *pGroup, int iID, LPCTSTR sRelativePath, LPCTSTR sFileName, LPCTSTR sPathName)
{
	m_pGroup		= pGroup;
	m_sRelativePath = sRelativePath;
	m_sFileName		= sFileName;
	m_Status		= TEST_STATUS_NOT_TESTED;
	m_iID			= iID;
	m_fScore		= 0;
	int iGroupID	= m_pGroup->GroupID();
	m_sRelativePath.Replace(_T('\\'), _T('/'));
	{
		// Add table
		m_pHtmlTable->NewRow();
		m_pHtmlTable->NewCell(CELL_TD);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->SetSpan(TABLE_SPAN_COL, 4);
		{
			CString sTitle	 = GetConfigString(_T("TITLE"));
			CString sPrefix	 = GetConfigString(_T("PREFIX"));
			CString sPostfix = GetConfigString(_T("POSTFIX"));

			if (sTitle.empty()) {
				sTitle.Format(_T("%s%s"), sPathName ? sPathName : _T(""), sFileName);
			}

			if (sPrefix.empty()) {
				sPrefix = pGroup->GetConfig(TG_DESC_PREFIX);
			}

			if (sPostfix.empty()) {
				sPostfix = pGroup->GetConfig(TG_DESC_POSTFIX);
			}

			ExtensionString(sPrefix);
			ExtensionString(sPostfix);
			m_pHtmlTable->Control()->CallJScript(_T("SetTBody(\"%s <a href='test:%s'>%d-%d. %s</a> %s\");"),
												 (LPCTSTR)sPrefix, (LPCTSTR)FullName(), iGroupID + 1, iID + 1,
												 (LPCTSTR)sTitle, (LPCTSTR)sPostfix);
		}
		m_pHtmlTable->NewCell(CELL_TD, _T("date_%d_%d"), pGroup->GroupID(), m_iID);
		m_pHtmlTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
		m_pHtmlTable->NewCell(CELL_TD, _T("result_%d_%d"), pGroup->GroupID(), m_iID);
		m_pHtmlTable->SetTextAlignment(TABLE_ALIGN_RIGHT);
	}
	// m_sRelativePath.Replace(_T('/'), _T('\\'));
	UpdateProfile();
	UpdateTable();
}

TestVector::~TestVector(void) {}

void TestVector::ExtensionString(CString &sStr)
{
	if (sStr.IsEmpty())
		return;
	else {
		CString sFileHead(m_sFileName);
		CString sFileExt;
		{
			int iPos = sFileHead.ReverseFind(_T('.'));

			if (iPos > 0) {
				sFileExt = m_sFileName;
				sFileExt.Delete(0, iPos + 1);
				sFileHead.Delete(iPos, sFileHead.GetLength() - iPos);
			}
		}
		sStr.Replace(_T("$(FILE_NAME)"), m_sFileName);
		sStr.Replace(_T("$(FILE_HEAD)"), sFileHead);
		sStr.Replace(_T("$(FILE_EXT)"), sFileExt);
		sStr.Replace(_T("$(FILE_PATH)"), m_sRelativePath);
		sStr.Replace(_T("$(ROOT_PATH)"), m_pGroup->ScenarioPath());
		sStr.Replace(_T("\\"), _T("/"));
	}
}

CString TestVector::GetConfigString(LPCTSTR sKey, LPCTSTR sDefault)
{
	CString sConfigFileName;
	CString sRet;
	sConfigFileName.Format(_T("%s%s\\%s.%s"), (LPCTSTR)m_sScenarioPath, (LPCTSTR)m_sRelativePath, (LPCTSTR)m_sFileName,
						   (LPCTSTR)g_sGlobalName);
	sConfigFileName.Replace(_T('/'), _T('\\'));
	{
		TCHAR sLine[4096];
		GetPrivateProfileString(g_sGlobalName, sKey, sDefault ? sDefault : _T(""), sLine, 4096, sConfigFileName);
		sRet = sLine;
	}
	ExtensionString(sRet);
	return sRet;
}

double TestVector::Score(void)
{
	double fScore = 0;

	switch (m_Status) {
	case TEST_STATUS_RUN_PASSED:
		fScore = 100.0;
		break;

	case TEST_STATUS_SCORE: {
		TG_SCORE_FORMAT *pScoreFormat = m_pGroup->ScoreFormat();
		fScore = ((m_fScore - pScoreFormat->min) * 100.0) / (pScoreFormat->max - pScoreFormat->min);
	} break;

	default:
		fScore = 0;
	}

	return fScore;
}

CString TestVector::FullName(void)
{
	CString sPath;
	sPath.Format(_T("%s/%s"), (LPCTSTR)m_sRelativePath, (LPCTSTR)m_sFileName);
	sPath.Replace(_T('\\'), _T('/'));
	return sPath;
}

static void PrintTime(CString &str, int r, int g, int b, LPCTSTR sFormat, int iTime = 0, BOOL bBold = FALSE)
{
	CString sExFormat;
	sExFormat.Format(_T("%s%s%s%s%s"), bBold ? _T("<b>") : _T(""), _T("<font color='#%02X%02X%02X'>"), sFormat,
					 _T("</font>"), bBold ? _T("</b>") : _T(""));
	str.Format(sExFormat, r, g, b, iTime);
}

void TestVector::UpdateProfile(BOOL bUpdate)
{
	CString sKey;
	{
		// result
		sKey.Format(_T("result_%s"), (LPCTSTR)FullName());

		if (bUpdate) {
			// read
			int iRes = GetPrivateProfileInt(g_sAppName, sKey, TEST_STATUS_NOT_TESTED, m_sLogPath);

			if (iRes < 0 || iRes >= TEST_STATUS_SIZE)
				iRes = 0;

			m_Status = (TEST_STATUS)iRes;
		} else {
			// write
			CString sValue;
			sValue.Format(_T("%d"), m_Status);
			WritePrivateProfileString(g_sAppName, sKey, sValue, m_sLogPath);
		}
	}

	if (m_Status == TEST_STATUS_SCORE) { // score
		sKey.Format(_T("score_%s"), (LPCTSTR)FullName());

		if (bUpdate) {
			// read
			TCHAR sScore[MAX_PATH];
			GetPrivateProfileString(g_sAppName, sKey, _T("0"), sScore, MAX_PATH, m_sLogPath);
			_stscanf(sScore, _T("%I64x"), (unsigned long long *)&m_fScore);
		} else {
			// write
			CString sValue;
			sValue.Format(_T("%I64x"), *(unsigned long long *)&m_fScore);
			WritePrivateProfileString(g_sAppName, sKey, sValue, m_sLogPath);
		}
	} else {
		m_fScore = 0;
	}

	{
		// date
		sKey.Format(_T("date_%s"), (LPCTSTR)FullName());

		if (bUpdate) {
			// read
			TCHAR sTime[MAX_PATH];
			GetPrivateProfileString(g_sAppName, sKey, _T(""), sTime, MAX_PATH, m_sLogPath);

			if (*sTime) {
				if (sizeof(time_t) == sizeof(DWORD))
					_stscanf(sTime, _T("%x"), &m_LastTime);
				else
					_stscanf(sTime, _T("%llx"), &m_LastTime);
			}
		} else {
			// write
			CString sTime;

			if (sizeof(time_t) == sizeof(DWORD))
				sTime.Format(_T("%x"), m_LastTime);
			else
				sTime.Format(_T("%llx"), m_LastTime);

			WritePrivateProfileString(g_sAppName, sKey, (LPCTSTR)sTime, m_sLogPath);
		}
	}
}

void TestVector::UpdateTable(void)
{
	if (m_Status == TEST_STATUS_SCORE) {
		CString sScore;
		{
			TG_SCORE_FORMAT *pScoreFormat = m_pGroup->ScoreFormat();
			DWORD			 dwColor;
			{
				// color set
				double fScore	= m_fScore;
				double min		= pScoreFormat->min;
				double max		= pScoreFormat->max;
				BOOL   bInverse = min > max;

				if (bInverse) {
					double temp = min;
					min			= max;
					max			= temp;
				}

				if (fScore < min)
					fScore = min;

				if (fScore > max)
					fScore = max;

				fScore = ((fScore - min) * 100.0) / (max - min);

				if (bInverse)
					fScore = 100.0 - fScore;

				dwColor = (fScore < 0 ? 0 : fScore > 100 ? 255 : (DWORD)(fScore * 255) / 100);
				dwColor = (dwColor) | ((255 - dwColor) << 16);
			}
			sScore.Format(pScoreFormat->sFormat, dwColor, m_fScore);
		}
		m_pHtmlTable->Control()->CallJScript(_T("SetBody('result_%d_%d', \"%s\");"), m_pGroup->GroupID(), m_iID,
											 (LPCTSTR)sScore);
	} else
		m_pHtmlTable->Control()->CallJScript(_T("SetBody('result_%d_%d', \"%s\");"), m_pGroup->GroupID(), m_iID,
											 g_sTestStatus[m_Status]);

	if (m_Status == TEST_STATUS_NOT_TESTED) {
		m_pHtmlTable->Control()->CallJScript(_T("SetBody('date_%d_%d', \"-\");"), m_pGroup->GroupID(), m_iID);
	} else {
		CString sTime;
		int		days, months, years;
		{
			time_t today;
			today		  = GetCurrentDayTime();
			double d_diff = (long double)(today) -
							(long double)(m_LastTime); // difftime(today, m_LastTime);	// difftime is not working!
			days   = d_diff / (60 * 60 * 24);
			months = (int)((days * 12) / 365);
			years  = days / 365;
		}

		if (years) {
			if (years == 1)
				PrintTime(sTime, 255, 0, 0, _L(A_YEAR_AGO), 0, TRUE);
			else
				PrintTime(sTime, 255, 0, 0, _L(YEARS_AGO), years, TRUE);
		} else if (months) {
			if (months == 1)
				PrintTime(sTime, 255, 0, 0, _L(A_MONTH_AGO));
			else
				PrintTime(sTime, 255, 0, 0, _L(MONTHS_AGO), months);
		} else {
			if (!days)
				PrintTime(sTime, 0, 230, 0, _L(TODAY), 0, TRUE);
			else if (days == 1)
				PrintTime(sTime, 0, 0, 255, _L(YESTERDAY), 0, TRUE);
			else {
				int ratio = (255 * days) / 31;
				PrintTime(sTime, ratio, 0, 255 - ratio, _L(DAYS_AGO), days);
			}
		}

		m_pHtmlTable->Control()->CallJScript(_T("SetBody('date_%d_%d', \"%s\");"), m_pGroup->GroupID(), m_iID,
											 (LPCTSTR)sTime);
	}

	m_pGroup->UpdateTable();
}

void TestVector::DoTest(void)
{
	CString sFullPath;
	CString sArg;
	g_pSystem->ClearLog();
	g_pSystem->LogInfo(_T("Test : %d.%s(%d) - %s"), m_pGroup->GroupID() + 1, m_pGroup->GetConfig(TG_DESC_NAME),
					   m_iID + 1, (LPCTSTR)FullName());
	sFullPath.Format(_T("%s%s\\%s"), (LPCTSTR)m_sScenarioPath, (LPCTSTR)m_sRelativePath, (LPCTSTR)m_sFileName);
	sArg.Format(m_pGroup->GetConfig(TG_DESC_PARAMETERS), (LPCTSTR)sFullPath);
	m_pHtmlTable->JScript(
		_T("ShowWait(\"%s<br><font color=#3F3FFF><b>%d-%d. '%s'</b></font><br><small><small>(%s)</small></small>\");"),
		_L(TEST_IS_RUNNING), m_pGroup->GroupID() + 1, m_iID + 1, (LPCTSTR)FullName(), _L(INFO_FORCE_TO_QUIT_PROGRAM));
	{
		// run test
		CString sProgram;
		TCHAR	sErrorTokenList[1024];
		LPCTSTR sErrorToken[16];
		int		iErrorCode[16];
		// retrieve program path
		{
			sProgram = m_pGroup->GetConfig(TG_DESC_PROGRAM);

			if (sProgram[0] == _T('@')) { // outside tool path
				sProgram.Delete(0, 1);
			} else { // in program
				sProgram.Format(_T("%s\\%s"), (LPCTSTR)m_sProgramPath, m_pGroup->GetConfig(TG_DESC_PROGRAM));

				if (!IsFileExist(sProgram)) {
					g_pSystem->LogError(_L(TEST_PROGRAM_IS_NOT_FOUND), (LPCTSTR)sProgram);
					m_pHtmlTable->JScript(_T("ShowWait(0);"));
					return;
				}
			}
		}
		{
			_tcscpy(sErrorTokenList, m_pGroup->GetConfig(TG_DESC_ERROR_STRING));
			memset(sErrorToken, 0, sizeof(LPCTSTR) * 16);
			LPTSTR sTok = sErrorTokenList;

			for (int i = 0; i < 16; i++) {
				if (!sTok || !*sTok) {
					sErrorToken[i] = 0;
					break;
				}

				sErrorToken[i] = sTok;
				iErrorCode[i]  = -TEST_STATUS_RUN_FAILED;

				// next error token
				if ((sTok = _tcsstr(sTok, _T(";")))) {
					*sTok = _T('\0');
					sTok++;
				}

				// get error code if exist.
				if (sErrorToken[i]) {
					LPTSTR sRetValue = _tcsstr(sErrorToken[i], _T("@E:"));

					if (sRetValue) {
						*sRetValue = _T('\0');
						sRetValue += 3;
						iErrorCode[i] = _ttoi(sRetValue);
					}
				}
			}
		}
		m_pHtmlTable->Control()->CallJScript(_T("SetBody('result_%d_%d', \"<font color='#FF3FFF'>%s</font>\");"),
											 m_pGroup->GroupID(), m_iID, _L(TEST_IN_PROGRESS));
		m_pCurrentTestVector = this;
		double fScore		 = FLT_MAX;

		if (m_bSuppressWaveform) { // suppress simulation output to quick execution
			SetEnvironmentVariable(_T("SIM_OUTPUT_ENABLE"), _T("0"));
		}

		int iRet = g_pSystem->ExecuteFile(
			sProgram, sArg, TRUE, NULL, m_sProgramPath, __sTestStatusList[TEST_STATUS_RUN_PASSED],
			TEST_STATUS_RUN_PASSED, __sTestStatusList[TEST_STATUS_RUN_FAILED], -TEST_STATUS_RUN_FAILED,
			__sTestStatusList[TEST_STATUS_RUN_CRACHED], -TEST_STATUS_RUN_CRACHED,
			__sTestStatusList[TEST_STATUS_COMPILE_FAILED], -TEST_STATUS_COMPILE_FAILED,
			__sTestStatusList[TEST_STATUS_LINK_FAILED], -TEST_STATUS_LINK_FAILED,
			__sTestStatusList[TEST_STATUS_FILE_NOT_FOUND], -TEST_STATUS_FILE_NOT_FOUND,
			__sTestStatusList[TEST_STATUS_SYSTEM_IS_REQUIRED], -TEST_STATUS_SYSTEM_IS_REQUIRED,
			__sTestStatusList[TEST_STATUS_SCORE], REDIRECT_ARGUMENT(1), &fScore, sErrorToken[0], iErrorCode[0],
			sErrorToken[1], iErrorCode[1], sErrorToken[2], iErrorCode[2], sErrorToken[3], iErrorCode[3], sErrorToken[4],
			iErrorCode[4], sErrorToken[5], iErrorCode[5], sErrorToken[6], iErrorCode[6], sErrorToken[7], iErrorCode[7],
			sErrorToken[8], iErrorCode[8], sErrorToken[9], iErrorCode[9], sErrorToken[10], iErrorCode[10],
			sErrorToken[12], iErrorCode[12], sErrorToken[13], iErrorCode[13], sErrorToken[14], iErrorCode[14],
			sErrorToken[15], iErrorCode[15], NULL);
		SetEnvironmentVariable(_T("SIM_OUTPUT_ENABLE"), _T("1")); // restore
		m_pCurrentTestVector = NULL;

		if (iRet < 0)
			iRet = -iRet;

		if (iRet >= TEST_STATUS_SIZE)
			iRet = 0;

		if (fScore != FLT_MAX) {
			iRet	 = TEST_STATUS_SCORE;
			m_fScore = fScore;
		}

		m_Status = (TEST_STATUS)iRet;
	}
	m_pHtmlTable->JScript(_T("ShowWait(0);"));
	m_LastTime = GetCurrentDayTime();
	UpdateProfile(FALSE);
	UpdateTable();
}
