//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 6/23/2025 Mon (clonextop@gmail.com)
//================================================================================
// Rev.  : 12/22/2020 Tue (clonextop@gmail.com)
//================================================================================
#include "XilinxSynthesis.h"
#include <io.h>

BOOL IsFileExist(LPCTSTR sFilePath)
{
	return _taccess(g_pSystem->RetrieveFullPath(sFilePath), 0) != -1;
}

static BOOL __FindXilinxTool_Vivado(TCHAR cDrive, LPTSTR sXilinxPath, LPTSTR sToolName)
{
	// Vivado existence checking
	WIN32_FIND_DATA FindFileData;
	HANDLE			hFind;
	CString			sPath;
	BOOL			bResult = TRUE;
	BOOL			bFound	= FALSE;
	{ // searching "drive:\Xilinx\Vivado\..
		sPath.Format(_T("%c:\\Xilinx\\Vivado\\*.*"), cDrive);

		if ((hFind = FindFirstFile(sPath, &FindFileData)) != INVALID_HANDLE_VALUE) {
			while (bResult) {
				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
					sPath.Format(_T("%c:\\Xilinx\\Vivado\\%s\\"), cDrive, FindFileData.cFileName);
					{
						// search environment batch file.
						CString sFile;
						sFile.Format(_T("%ssettings64.bat"), sPath);

						if (IsFileExist(sFile)) {
							_tcscpy(sXilinxPath, sPath);
							sFile.Format(_T("Vivado %s"), FindFileData.cFileName);
							_tcscpy(sToolName, sFile);
							bFound = TRUE;
							break;
						}
					}
				}

				bResult = FindNextFile(hFind, &FindFileData);
			}

			FindClose(hFind);
		}
	}
	if (!bFound) { // searching "drive:\Xilinx\20XX.X\Vivado\..
		sPath.Format(_T("%c:\\Xilinx\\*.*"), cDrive);

		if ((hFind = FindFirstFile(sPath, &FindFileData)) != INVALID_HANDLE_VALUE) {
			while (bResult) {
				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
					sPath.Format(_T("%c:\\Xilinx\\%s\\Vivado\\"), cDrive, FindFileData.cFileName);
					LOGI(_T("sPath = %s\n"), (LPCTSTR)sPath);
					{
						// search environment batch file.
						CString sFile;
						sFile.Format(_T("%ssettings64.bat"), sPath);

						if (IsFileExist(sFile)) {
							_tcscpy(sXilinxPath, sPath);
							sFile.Format(_T("Vivado %s"), FindFileData.cFileName);
							_tcscpy(sToolName, sFile);
							bFound = TRUE;
							break;
						}
					}
				}

				bResult = FindNextFile(hFind, &FindFileData);
			}

			FindClose(hFind);
		}
	}
	return bFound;
}

static BOOL FindXilinxTool(TCHAR cDrive, LPTSTR sXilinxPath, LPTSTR sToolName)
{
	if (__FindXilinxTool_Vivado(cDrive, sXilinxPath, sToolName))
		return TRUE;

	return FALSE;
}

void XilinxSynthesis::XilinxToolCheck(void)
{
	if (!m_Config.sXilinxPath[0]) {
		{
			CString sToolName;
			BOOL	bFound = FALSE;
			sToolName.GetBuffer(1024);

			// search with each drive
			for (TCHAR cDrive = _T('C'); cDrive <= _T('G') && !bFound; cDrive++) {
				bFound = FindXilinxTool(cDrive, m_Config.sXilinxPath, sToolName.Buffer());
			}

			if (!bFound)
				g_pSystem->LogWarning(_L(CAN_NOT_FIND_XILINX_PATH));
			else {
				UpdateProperty(PROPERTY_ID_INSTALL_PATH);
				g_pSystem->LogInfo(_L(FOUND_XILINX_PATH), (LPCTSTR)m_Config.sXilinxPath);
				m_XilinxDevices.Initialize();
				m_XilinxDevices.RefreshFamilyProperty(m_pProperty[PROPERTY_ID_FAMILY]);
			}
		}
	}
}
