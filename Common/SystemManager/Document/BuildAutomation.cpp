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
// Title : System manager
// Rev.  : 3/20/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "BuildAutomation.h"
#include "Utils.h"

static LPCTSTR	__sDIRTY_SYSTEM = _T("SYSTEM_DIRTY");

BuildAutomation::BuildAutomation(ITDDocument* pDoc, DWORD dwCheckCommandStart, DWORD dwBuildCommandStart)
{
	m_pDoc					= pDoc;
	m_dwCheckCommandStart	= dwCheckCommandStart;
	m_dwBuildCommandStart	= dwBuildCommandStart;
	m_bAutoBuild			= FALSE;
	m_bStrictBuild			= TRUE;
	m_bDebug				= FALSE;
	m_bDirtySystem			= FALSE;
	m_sAStyle.GetBuffer(1024);
	m_sAStyle.Format(_T("--style=linux -T -n -k1 -L -xc -xn -xl -f -p -U -xe"));
	m_siStyle.GetBuffer(1024);
	m_siStyle.Format(_T("--style=kr -T4 -n"));
	m_sAuthorName.GetBuffer(MAX_PATH);
	CString curProjectPath(pDoc->GetSystem()->GetProjectPath());
	{
		DWORD	dwSize = 1024;
		GetUserName((LPTSTR)m_sAuthorName.GetBuffer(), &dwSize);
	}
	m_BuildList.Initialize();

	for(int i = 0; i < m_BuildList.Size(); i++) {
		BuildAutomationItem* pItem	= m_BuildList.Item(i);

		if(pItem && pItem->sExtensions.size()) {
			CString sWatchPath;

			if(pItem->sWatchPath.Find(_T('%%')) != -1)
				sWatchPath	= g_pSystem->RetrieveFullPath(pItem->sWatchPath);
			else
				sWatchPath	= curProjectPath + pItem->sWatchPath;

			m_pDoc->AddWatchDogPath(sWatchPath, m_dwCheckCommandStart + i, pItem->bSearchSubDir);
		}
	}

	if(!(m_bDirtySystem = m_pDoc->GetConfigInt(__sDIRTY_SYSTEM, TRUE))) {
		FILETIME swTIme = {0, 0};
		FILETIME hwTIme = {0, 0};
		LPCTSTR	 sExtsHW[]	= {_T("v"), _T("sv"), _T("vh"), NULL};
		LPCTSTR	 sExtsSW[]	= {_T("c"), _T("cpp"), _T("h"), _T("inl"), NULL};
		SearchLastWriteFileTime(swTIme, _T("%PROJECT%System/SubSystems/Simulation/verilator/"), sExtsSW);

		if(!*(ULONGLONG*)&swTIme)
			m_bDirtySystem = TRUE;
		else {
			SearchLastWriteFileTime(hwTIme, _T("%PROJECT%System/HDL/"), sExtsHW);

			if(!*(ULONGLONG*)&swTIme || *(ULONGLONG*)&hwTIme > *(ULONGLONG*)&swTIme)
				m_bDirtySystem = TRUE;
		}
	}

	assert((m_dwCheckCommandStart + m_BuildList.Size()) <= m_dwBuildCommandStart);
}

BuildAutomation::~BuildAutomation(void)
{
	m_pDoc->SetConfigInt(__sDIRTY_SYSTEM, m_bDirtySystem);
}

BOOL BuildAutomation::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	if(command >= m_dwCheckCommandStart && command < m_dwCheckCommandStart + m_BuildList.Size()) {
		DoCheck(command, (LPCTSTR)lParam);
	} else if(command >= m_dwBuildCommandStart && command < m_dwBuildCommandStart + m_BuildList.Size()) {
		DoBuild(command);
	} else return FALSE;

	return TRUE;
}

void BuildAutomation::DoCheck(DWORD command, LPCTSTR sFileName)
{
	DWORD id	= command - m_dwCheckCommandStart;
	BuildAutomationItem*	pItem	= m_BuildList.Item(id);
	{
		// bypass 'nosearch' directive
		CString	sNosearchPath(sFileName);
		int iPos	= sNosearchPath.find_last_of(_T('\\'));

		if(iPos > 0) {
			sNosearchPath.Delete(iPos + 1, -1);
			sNosearchPath	+= _T(".TestDrive.nosearch");

			if(IsFileExisted(sNosearchPath)) return;
		}
	}

	if(id < m_BuildList.Size() && CheckBuild(sFileName, pItem)) {
		if(m_bAutoBuild && !pItem->sExecuteFile.IsEmpty())
			m_pDoc->KillTimer(id + m_dwBuildCommandStart);

		{
			CString sName(sFileName);

			if(m_sAStyle.GetAt(0) && m_sAStyle.GetAt(0) != ' ' &&
			   (sName.Find(_T(".h"), sName.GetLength() - 2) > 0 ||
				sName.Find(_T(".c"), sName.GetLength() - 2) > 0 ||
				sName.Find(_T(".cpp"), sName.GetLength() - 4) > 0 ||
				sName.Find(_T(".java"), sName.GetLength() - 5) > 0 ||
				sName.Find(_T(".inl"), sName.GetLength() - 4) > 0)) {
				CString sArg;
				CString sAstylePath	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%bin\\msys64\\ucrt64\\bin\\AStyle.exe"));
				sArg.Format(_T("%s \"%s\""), m_sAStyle, sFileName);
				g_pSystem->ExecuteFile(sAstylePath, sArg, TRUE, NULL, NULL, NULL);
			} else if(m_siStyle.GetAt(0) && m_siStyle.GetAt(0) != ' ' &&
					  (sName.Find(_T(".v"), sName.GetAllocLength() - 2) > 0 ||
					   sName.Find(_T(".sv"), sName.GetAllocLength() - 3) > 0)) {
				CString sArg;
				CString sIstylePath	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_PROFILE%Common\\bin\\iStyle.exe"));
				sArg.Format(_T("%s \"%s\""), m_siStyle, sFileName);
				g_pSystem->ExecuteFile(sIstylePath, sArg, TRUE, NULL, NULL, NULL);
				SetDirtySystem();
			} else if(sName.Find(_T(".vh"), sName.GetAllocLength() - 3) > 0) {
				SetDirtySystem();
			} else if(sName.Find(_T(".do"), sName.GetAllocLength() - 3) > 0) {
				CString sArg;
				CString sProjectPath	= g_pSystem->RetrieveFullPath(_T("%PROJECT%"));
				CString sProgramPath	= sProjectPath + _T("Program\\");
				sProjectPath.Replace(_T("\\"), _T("\\\\\\"));
				sProgramPath.Replace(_T("\\"), _T("\\\\\\"));
				sArg.Format(_T("-i 's;%s;;' %s"), sProgramPath.c_str(), sFileName);
				g_pSystem->ExecuteFile(_T("sed"), sArg, TRUE, NULL, NULL, NULL);
				sArg.Format(_T("-i 's;%s;..\\\\\\;' %s"), sProjectPath.c_str(), sFileName);
				g_pSystem->ExecuteFile(_T("sed"), sArg, TRUE, NULL, NULL, NULL);
				return;
			}
		}

		if(!pItem->sInceptionFile.IsEmpty())
			g_pSystem->InsertCodeInception((LPCTSTR)sFileName, pItem->sInceptionFile, pItem->sTitle, m_sAuthorName);

		if(m_bAutoBuild && !pItem->sExecuteFile.IsEmpty()) {
			m_sTempFilePath	= sFileName;
			m_pDoc->SetTimer(id + m_dwBuildCommandStart, 700);		// delayed build for chained source change
		}
	}
}

BOOL SearchMakefile(LPCTSTR sRoot, CString& sFilePath)
{
	BOOL	bRet	= FALSE;
	{
		// eliminate file part
		int iPos	= sFilePath.ReverseFind(_T('\\'));
		sFilePath.Delete(iPos, -1);
		sFilePath	= g_pSystem->RetrieveFullPath(sFilePath);
	}
	{
		CString sRootPath	= g_pSystem->RetrieveFullPath(sRoot);
		sRootPath.TrimRight(_T('\\'));

		while(sRootPath.Compare(sFilePath) <= 0) {
			CString sMakefilePath;
			sMakefilePath.Format(_T("%s\\makefile"), sFilePath);

			if(_taccess(sMakefilePath, 0) != -1) {
				bRet	= TRUE;
				break;
			}

			{
				int iPos	= sFilePath.ReverseFind(_T('\\'));

				if(iPos > 0) sFilePath.Delete(iPos, -1);
				else break;
			}
		}
	}
	return bRet;
}

int DoMake(BOOL bClean, LPCTSTR sArg, LPCTSTR sWorkPath)
{
	g_pSystem->LogInfo(_T("Make %sbuild : %s\\make %s"), bClean ? _T("clean & ") : _T(""), sWorkPath, sArg);

	// cleaning
	if(bClean)
		g_pSystem->ExecuteFile(_T("mingw32-make"), _T("clean"), TRUE, NULL, sWorkPath, NULL);
	else
		g_pSystem->ExecuteFile(_T("mingw32-make"), _T("dep"), TRUE, NULL, sWorkPath, NULL);

	// build
	return g_pSystem->ExecuteFile(_T("mingw32-make"), sArg, TRUE, NULL, sWorkPath,
								  _T("error:"), -1,
								  _T("] Error"), -1,
								  _T("*E: "), -1,
								  _T("*W: "), 0,
								  _T("undefined reference to "), -1,
								  NULL);
}

typedef struct {
	BOOL		bClean;
	LPCTSTR		sArg;
} MAKE_DESC;

BOOL DoMakefileBuild(LPCTSTR sPath, LPVOID pData)
{
	MAKE_DESC*	pDesc	= (MAKE_DESC*)pData;
	{
		CString	sWorkPath(sPath);
		int iPos	= sWorkPath.ReverseFind(_T('\\'));

		if(iPos > 0) sWorkPath.Delete(iPos, -1);

		if(DoMake(pDesc->bClean, pDesc->sArg, sWorkPath) < 0)
			return FALSE;
	}
	return TRUE;
}

void BuildAutomation::DoBuild(DWORD command)
{
	BOOL	bCleanBuild		= FALSE;
	DWORD	id				= command - m_dwBuildCommandStart;
	m_pDoc->KillTimer(command);

	if(GetKeyState(VK_LCONTROL) < 0) {
		g_pSystem->LogInfo(_L(BUILD_CANCELED_BY_USER));
		return;
	}

	CString	sFilePath	= m_sTempFilePath;	// latest changed file path
	m_sTempFilePath.Empty();
	g_pSystem->ClearLog();
RETRY_BUILD:

	if(id < m_BuildList.Size()) {
		BuildAutomationItem*	pItem	= m_BuildList.Item(id);
		CString sProjectPath(g_pSystem->GetProjectPath());
		CString sRunPath;
		LPCTSTR sExecuteFile	= pItem->sExecuteFile;
		LPCTSTR sTitle			= pItem->sTitle;
		LPCTSTR sArgument		= pItem->sArgument;
		int		iRet	= 0;
		sProjectPath	+= pItem->sWorkPath;
		sRunPath		= sProjectPath;
		sRunPath		+= sExecuteFile;
		g_pSystem->LogInfo(_L(START_BUILD), sTitle);

		if(sRunPath.Find(_T(".profile"), sRunPath.GetLength() - 8) > 0 ||
		   sRunPath.Find(_T(".sp"), sRunPath.GetLength() - 3) > 0) {
			// run profile
			if(g_pSystem->RunProfile(sRunPath, TRUE)) iRet = 1;
		} else if(sRunPath.Find(_T(".sv"), sRunPath.GetLength() - 3) > 0 ||
				  sRunPath.Find(_T(".v"), sRunPath.GetLength() - 2) > 0) {
			if(!m_bDirtySystem) {
				if((pItem->iLinkItem != id) && (pItem->iLinkItem >= 0)) {
					id	= pItem->iLinkItem;
					goto RETRY_BUILD;
				}

				goto BUILD_IS_DONE;
			}

			{
				HANDLE				hFind;
				WIN32_FIND_DATA		FindFileData;

				// delete previous builded c++ source & object files
				if((hFind = FindFirstFile(g_pSystem->RetrieveFullPath(_T("%PROJECT%/System/SubSystems/Simulation/verilator/*.*")), &FindFileData)) != INVALID_HANDLE_VALUE) {
					do {
						if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
							CString		FileName;
							FileName.Format(_T("%s%s"), g_pSystem->RetrieveFullPath(_T("%PROJECT%/System/SubSystems/Simulation/verilator/")), FindFileData.cFileName);
							_tremove((LPCTSTR)FileName);
						}
					} while(FindNextFile(hFind, &FindFileData));

					FindClose(hFind);
				}

				if(m_bDirtySystem) {
					bCleanBuild		= TRUE;
					m_bDirtySystem	= FALSE;
				}
			}

			{
				// run verilator
				CString sArgs(sArgument);
				TCHAR	sTopFile[1024 * 32];
				TCHAR	sDefines[1024 * 32];
				GetEnvironmentVariable(_T("SIM_TOP_FILE"), sTopFile, 1024 * 32);
				GetEnvironmentVariable(_T("SIM_DEFINITION"), sDefines, 1024 * 32);
				sArgs.AppendFormat(_T(" %s"), sDefines);

				if(g_pSystem->VerilatorCompile(sTopFile, sProjectPath, sArgs)) {
					if((pItem->iLinkItem != id) && (pItem->iLinkItem >= 0)) {
						id	= pItem->iLinkItem;
						goto RETRY_BUILD;
					}
				} else iRet	= -1;
			}
		} else if(sRunPath.Find(_T(".sln"), sRunPath.GetLength() - 4) > 0) {
			// build project
			CString vs_build, sArg;
			vs_build	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_PROFILE%Common\\common\\vsenv.bat"));
			sArg.Format(_T("\"%s\" /Build \"%s\""), sRunPath, sArgument ? sArgument :
						(m_bDebug ? _T("Debug|Win32") : _T("Release|Win32")));
			iRet = g_pSystem->ExecuteFile(vs_build, sArg, TRUE, NULL, sProjectPath,
										  _T(": warning LNK"), 0,
										  _T(": warning "), m_bStrictBuild ? -1 : 0,
										  _T("): error "), -1,
										  _T(": fatal error "), -1,
										  NULL);
		} else if(sRunPath.Find(_T("make"), sRunPath.GetLength() - 4) > 0) {
			CString	sArg;

			if(m_bDebug) sArg.Append(_T("DEBUG=1 "));

			if(sArgument) sArg.Append(sArgument);

			if(!sFilePath.empty()) {	// if a path is existed, search current & sub directories' "makefile" and do build
				CString sWorkPath(sFilePath);

				if(SearchMakefile(sProjectPath, sWorkPath)) {
					// Find sub-path and make build
					iRet	= DoMake(bCleanBuild, sArg, sWorkPath);
				} else {
					// find all sub-path and make all
					MAKE_DESC	desc;
					desc.bClean	= bCleanBuild;
					desc.sArg	= sArg;

					if(!g_pSystem->SearchSubPathFile(sProjectPath, _T("makefile"), DoMakefileBuild, (LPVOID)&desc))
						iRet	= -1;
				}
			} else {
				iRet	= DoMake(bCleanBuild, sArg, sProjectPath);
			}
		} else {
			// run execution file
			CString	sArg;
			sArg.Format(sArgument, (LPCTSTR)sFilePath);
			LPCTSTR sErrorString	= pItem->sErrorString;
			iRet = g_pSystem->ExecuteFile(sRunPath, (LPCTSTR)sArg, TRUE, NULL, sProjectPath,
										  sErrorString, -1,
										  NULL);
		}

	BUILD_IS_DONE:

		if(iRet < 0)	g_pSystem->LogError(_L(START_FAILD), sTitle);
		else		g_pSystem->LogInfo(_L(START_SUCCEED), sTitle);
	}
}

static LPCTSTR __sConvensionCheckFailed = _T("Code convention check is failed!\n");

BOOL BuildAutomation::CheckBuild(LPCTSTR sFilePath, BuildAutomationItem* pItem)
{
	CString	sExt;
	CString sFile(sFilePath);
	BOOL	bPass = FALSE;

	// none-check bypass
	if(pItem->sExtensions.IsEmpty()) return TRUE;

	{
		// get file name
		int iPos	= sFile.ReverseFind(_T('\\'));

		if(iPos > 0) {
			sFile.Delete(0, iPos + 1);
		}

		// get file extension
		iPos	= sFile.ReverseFind(_T('.'));

		if(iPos >= 0) {
			sExt = &(sFile.GetBuffer()[iPos + 1]);
		} else return FALSE;

		sExt.MakeLower();
	}
	{
		// existence check with file extension list.
		CString sExtFind;
		sExtFind.Format(_T(";%s;"), (LPCTSTR)sExt);

		if(pItem->sExtensions.Find(sExtFind) == -1) return FALSE;
	}

	// do cpp-check.
	if(!sExt.Compare(_T("c")) || !sExt.Compare(_T("cpp"))) {
		if(CppCheck(sFilePath))
			bPass = TRUE;
		else {
			if(m_bStrictBuild) g_pSystem->LogError(__sConvensionCheckFailed);
			else {
				g_pSystem->LogWarning(__sConvensionCheckFailed);
				bPass = TRUE;
			}
		}
	} else {
		bPass = TRUE;
	}

	return bPass;
}

BOOL BuildAutomation::CppCheck(LPCTSTR lpszPath)
{
	CString sIncCommonPath, sIncTestDrivePath;
	CString sArg;
	sIncCommonPath		= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_PROFILE%Common/include"));
	sIncTestDrivePath	= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%include"));
	sArg.Format(_T("-I %s -I %s -U_WIN32_WCE -U_MFC_VER -U_MSC_VER -U__BORLANDC__ -U__sgi -U__SGI_STL_PORT --platform=win64 -DWIN32 --suppress=preprocessorErrorDirective --suppress=unknownMacro --inline-suppr --force %s"), (LPCTSTR)sIncCommonPath, (LPCTSTR)sIncTestDrivePath, lpszPath);

	if(g_pSystem->ExecuteFile(_T("%TESTDRIVE_DIR%bin\\msys64\\ucrt64\\bin\\CppCheck.exe"), sArg.c_str(), TRUE, NULL, g_pSystem->GetProjectPath(),
							  _T(": error:"), -1,
							  _T(": information:"), 2,
							  NULL) < 0) {
		return FALSE;
	}

	return TRUE;
}
