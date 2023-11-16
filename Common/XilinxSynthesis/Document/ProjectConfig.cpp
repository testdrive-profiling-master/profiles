//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Xilinx synthesis
// Rev.  : 11/16/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "ProjectConfig.h"
#include "SourceList.h"
#include "ProfileConfig.inl"

LPCTSTR		g_sGlobalName	= _T("XilinxSynthesis");
LPCTSTR		g_sEmpty		= _T("");
LPCTSTR		g_sNotepadPath	= _T("%TESTDRIVE_DIR%bin/notepad/notepad++.exe");

static LPCTSTR __sKEY[CONFIG_ID_SIZE] = {
	_T("FAMILY"),		// CONFIG_ID_FAMILY
	_T("DEVICE"),		// CONFIG_ID_DEVICE
	_T("PACKAGE"),		// CONFIG_ID_PACKAGE
	_T("SPEED_GRADE"),	// CONFIG_ID_SPEED_GRADE
	_T("TARGET_SPEED"),	// CONFIG_ID_TARGET_SPEED
	_T("CLOCKS"),		// CONFIG_ID_CLOCK_LIST
	_T("RESETS"),		// CONFIG_ID_RESET_LIST
	_T("MAX_PATHS"),	// CONFIG_ID_MAX_PATHS
};

CString		ProjectConfig::m_sNameFilter;
CONFIG		ProjectConfig::m_Config;
DWORD		ProjectConfig::m_dwProjectResourceRefCount	= 0;
HtmlTable*	ProjectConfig::m_pTable						= NULL;
BOOL		ProjectConfig::m_bMustRebuild				= TRUE;

time_t GetCurrentDayTime(void)
{
	static BOOL 	bInitial	= TRUE;
	static time_t	time_diff	= 0;
	time_t	cur_time;
	time(&cur_time);

	if(bInitial) {
		struct tm tm_gmt	= *gmtime(&cur_time);
		time_t	gmt_time	= mktime(&tm_gmt);
		time_diff			= cur_time - gmt_time;
	}

	cur_time	+= time_diff;
	cur_time	-= cur_time % (60 * 60 * 24);
	return cur_time;
}

ProjectConfig::ProjectConfig(void)
{
	if(!m_dwProjectResourceRefCount) {
		m_Config.iMaxPaths			= 1;
		m_Config.sProjectPath[0]	= _T('\0');
		m_Config.sXilinxPath[0]		= _T('\0');
		memset(&m_Config.spec, 0, sizeof(m_Config.spec));
		GetDefaultProjectPath();
	}

	m_dwProjectResourceRefCount++;
}

ProjectConfig::~ProjectConfig(void)
{
	m_dwProjectResourceRefCount--;
}

BOOL ProjectConfig::GetDefaultProjectPath(void)
{
	_tcscpy(m_Config.sProjectPath, g_ProfileConfig.GetString(_T("PROJECT_PATH")));
	return m_Config.sProjectPath[0] != _T('\0');
}

BOOL ProjectConfig::UpdateConfig(CONFIG_ID id, BOOL bUpdate)
{
	LPCTSTR		sKey		= __sKEY[id];
	LPCTSTR		sDefault	= _T("");
	int*		iTarget		= NULL;
	int			iDefault	= 0;
	CString		sConfigPath;
	TCHAR*		sTarget		= NULL;

	if(!m_Config.sProjectPath[0]) GetDefaultProjectPath();

	if(!m_Config.sProjectPath[0]) return FALSE;

	sConfigPath.Format(_T("%s\\.%s"), m_Config.sProjectPath, g_sGlobalName);
	sConfigPath	= g_pSystem->RetrieveFullPath(sConfigPath);

	switch(id) {
	case CONFIG_ID_FAMILY:
		sTarget	= m_Config.spec.sFamily;
		sDefault = _T("Virtex UltraScale+");
		break;

	case CONFIG_ID_DEVICE:
		sTarget	= m_Config.spec.sDevice;
		sDefault = _T("xcvu3p");
		break;

	case CONFIG_ID_PACKAGE:
		sTarget	= m_Config.spec.sPackage;
		sDefault = _T("ffvc1517");
		break;

	case CONFIG_ID_SPEED_GRADE:
		sTarget	= m_Config.spec.sSpeedGrade;
		sDefault = _T("-2-e");
		break;

	case CONFIG_ID_TARGET_SPEED:
		sTarget	= m_Config.spec.sTargetSpeed;
		sDefault = _T("2000");
		break;

	case CONFIG_ID_CLOCKS:
		sTarget = m_Config.spec.sClocks;
		sDefault = _T("*CLK");
		break;

	case CONFIG_ID_RESETS:
		sTarget = m_Config.spec.sResets;
		sDefault = _T("*RST");
		break;

	case CONFIG_ID_MAX_PATHS:
		iTarget	= &m_Config.iMaxPaths;
		iDefault = 1;
		break;
	}

	if(bUpdate) {	// get
		if(sTarget) {
			TCHAR sStr[1024];
			GetPrivateProfileString(g_sGlobalName, sKey, sDefault, sStr, 1024, sConfigPath);
			_tcscpy(sTarget, sStr);
		} else if(iTarget) {
			*iTarget	= GetPrivateProfileInt(g_sGlobalName, sKey, iDefault, sConfigPath);
		}
	} else {		// set
		if(sTarget) {
			WritePrivateProfileString(g_sGlobalName, sKey, sTarget, sConfigPath);
		} else if(iTarget) {
			CString str;
			str.Format(_T("%d"), *iTarget);
			WritePrivateProfileString(g_sGlobalName, sKey, (LPCTSTR)str, sConfigPath);
		}
	}

	return TRUE;
}

CString ProjectConfig::MakeDevicePartName(void)
{
	CString sPartName;
	BOOL	bSeperator	= FALSE;
	{
		// If there are more than 2 numbers in the device name, separate them from the package name with a '-' sign.
		int		iDigitCount	= 0;
		BOOL	bDigit	= 0;

		for(int i = 0; m_Config.spec.sDevice[i] != 0; i++) {
			if(_istdigit(m_Config.spec.sDevice[i])) {
				if(!bDigit) iDigitCount++;

				bDigit	= TRUE;
			} else bDigit = FALSE;
		}

		bSeperator	= iDigitCount < 2;
	}
	sPartName.Format(_T("%s%s%s%s"), (LPCTSTR)m_Config.spec.sDevice, bSeperator ? _T("-") : _T(""), (LPCTSTR)m_Config.spec.sPackage, (LPCTSTR)m_Config.spec.sSpeedGrade);
	sPartName.MakeLower();
	return sPartName;
}

BOOL ProjectConfig::UpdateConfigAll(BOOL bUpdate)
{
	for(int id = 0; id < CONFIG_ID_SIZE; id++) {
		if(!UpdateConfig((CONFIG_ID)id, bUpdate)) return FALSE;
	}

	return CheckConfigCompleteness();
}

BOOL ProjectConfig::CheckConfigCompleteness(void)
{
	if(!m_Config.spec.sFamily[0] ||
	   !m_Config.spec.sDevice[0] ||
	   !m_Config.spec.sPackage[0] ||
	   !m_Config.spec.sSpeedGrade[0] ||
	   !m_Config.spec.sClocks[0] ||
	   !m_Config.spec.sResets[0] ||
	   _ttoi(m_Config.spec.sTargetSpeed) <= 0)
		return FALSE;

	return TRUE;
}

CString ProjectConfig::RetrievePath(LPCTSTR sExtPath)
{
	CString sPath(m_Config.sProjectPath);

	if(sExtPath) {
		sPath.AppendFormat(_T("\\%s"), sExtPath);
	}

	sPath	= g_pSystem->RetrieveFullPath(sPath);
	return sPath;
}

BOOL ProjectConfig::RetrieveSourcePath(LPCTSTR sSrcFile, CString& sSrcPath)
{
	CString sProjectPath(g_pSystem->RetrieveFullPath(m_Config.sProjectPath));
	CString sPath;
	sPath.Format(_T("%s%s"), (LPCTSTR)sProjectPath, sSrcFile);

	while(sPath.Compare(sProjectPath) >= 0) {
		int iPos	= sPath.ReverseFind(_T('/'));

		if(iPos < 0) break;

		sPath[iPos]	= _T('\0');
		{
			CString sSearch;
			sSearch.Format(_T("%s/%s"), (LPCTSTR)sPath, SOURCE_LIST_FILE_NAME);

			if(IsFileExist(sSearch)) {
				sSrcPath	= g_pSystem->RetrieveFullPath(sSearch);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL ProjectConfig::MakeProjectConfigFile(SourceVector* pSource) 	// for ISE only
{
	BOOL	bRet		= TRUE;
	CString sWorkDir	= RetrievePath(WORK_DIR_NAME);

	if(!pSource) return FALSE;

	if(_taccess(sWorkDir, 0) == -1) {	// not exist
		_tmkdir(sWorkDir);
	}

	{
		// make config
		CString sPath;
		sPath.Format(_T("%s\\project_config.tcl"), sWorkDir);
		FILE* fp = _tfopen(sPath, _T("wt"));

		if(fp) {
			// configuration
			FilePrint(fp, _T("add_condition -notrace\n"));
			FilePrint(fp, _T("project set family \"%s\"\n"), (LPCTSTR)m_Config.spec.sFamily);
			FilePrint(fp, _T("project set device \"%s\"\n"), (LPCTSTR)m_Config.spec.sDevice);
			FilePrint(fp, _T("project set package \"%s\"\n"), (LPCTSTR)m_Config.spec.sPackage);
			FilePrint(fp, _T("project set speed \"%s\"\n"), (LPCTSTR)m_Config.spec.sSpeedGrade);
			// xst work dir.
			sPath.Format(_T("%s\\xst"), sWorkDir);
			sPath.Replace(_T('\\'), _T('/'));
			FilePrint(fp, _T("project set \"Work Directory\" \"%s\"\n"), (LPCTSTR)sPath);
			fclose(fp);
		}
	}

	{
		// make source list
		CString sPath, sIncPath;
		sPath.Format(_T("%s\\project_sources.tcl"), sWorkDir);
		FILE* fp = _tfopen(sPath, _T("wt"));
		sIncPath	= g_pSystem->RetrieveFullPath(_T("%PROJECT%System/HDL"));
		//sIncPath.AppendFormat(_T("|%s"), (LPCTSTR)RetrievePath());

		if(fp) {
			{
				// add source list
				CString sSourceListPath;

				if(RetrieveSourcePath(pSource->FullName(), sSourceListPath)) {
					FILE* fp_src = _tfopen(sSourceListPath, _T("rt"));
					TCHAR sLine[1024];

					while(_fgetts(sLine, 1024, fp_src)) {
						CString sSrc(sLine);
						// eliminate comment
						sSrc.Replace(_T("//"), _T(";"));
						sSrc.Replace(_T(';'), _T('\0'));
						// triming
						sSrc.TrimLeft(_T(" \t"));
						sSrc.TrimRight(_T(" \t\r\n"));

						// add source
						if(!sSrc.IsEmpty()) {
							CString sFullPath;

							if(sSrc.GetAt(0) == _T('@')) {
								// add includes
								sSrc.Delete(0, 1);
								sIncPath.AppendFormat(_T("|%s"), (LPCTSTR)RetrievePath(sSrc));
							} else if(sSrc.GetAt(0) == _T('#')) {
								// bypass string
								sSrc.Delete(0, 1);
								FilePrint(fp, _T("%s\n"), (LPCTSTR)sSrc);
							} else {
								// add source(s)
								sFullPath	= RetrievePath(sSrc);

								if(sFullPath.Find(_T('*')) != -1) {
									HANDLE				hFind;
									WIN32_FIND_DATA		FindFileData;
									hFind	= FindFirstFile(sFullPath, &FindFileData);

									if(hFind != INVALID_HANDLE_VALUE) {
										TCHAR sDirPath[MAX_PATH], * pFilePart;
										GetFullPathName(sFullPath, MAX_PATH, sDirPath, &pFilePart);

										if(pFilePart) *pFilePart = _T('\0');

										do {
											if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
												sFullPath.Format(_T("%s%s"), sDirPath, FindFileData.cFileName);
												sFullPath.Replace(_T('\\'), _T('/'));

												if(sFullPath.Find(_T(".xdc")) != -1)	// xdc (for vivado only)
													FilePrint(fp, _T("xfile add \"%s\"\n"), (LPCTSTR)sFullPath);
											}
										} while(FindNextFile(hFind, &FindFileData));

										FindClose(hFind);
									}
								} else {
									sFullPath.Replace(_T('\\'), _T('/'));

									if(sFullPath.Find(_T(".xdc")) != -1)	// xdc (for vivado only)
										FilePrint(fp, _T("xfile add \"%s\"\n"), (LPCTSTR)sFullPath);
								}
							}
						}
					}

					fclose(fp_src);
				} else {
					fclose(fp);
					return FALSE;
				}
			}
			{
				// top design
				FilePrint(fp, _T("project set \"Verilog Include Directories\" \"%s\""), (LPCTSTR)sIncPath);
			}
			fclose(fp);
		}

		// make top module name
		sPath.Format(_T("%s\\project_top_name.tcl"), sWorkDir);
		fp = _tfopen(sPath, _T("wt"));

		if(fp) {
			CString sModuleName;

			if(!pSource->GetModuleName(sModuleName)) {
				g_pSystem->LogInfo(_L(MODULE_NAME_IS_NOT_FOUND));
				fclose(fp);
				return FALSE;
			}

			g_pSystem->LogInfo(_T("Top design name : '%s'"), (LPCTSTR)sModuleName);
			FilePrint(fp, _T("project set top \"%s\"\n"), (LPCTSTR)sModuleName);
			fclose(fp);
		}
	}

	return bRet;
}

