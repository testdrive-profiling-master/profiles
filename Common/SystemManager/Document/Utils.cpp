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
// Title : System manager
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "Utils.h"

void SearchLastWriteFileTime(FILETIME& FileTime, LPCTSTR sPath, LPCTSTR* sExts)
{
	HANDLE				hFind;
	WIN32_FIND_DATA		FindFileData;
	{
		// check no search path
		CString sNoSearch;
		sNoSearch.Format(_T("%s/.TestDrive.nosearch"), sPath);

		if(_taccess(sNoSearch, 0) != -1) return;
	}
	{
		// search all files
		CString str;
		str.Format(_T("%s/*.*"), g_pSystem->RetrieveFullPath(sPath));
		hFind	= FindFirstFile(str, &FindFileData);
	}

	if(hFind != INVALID_HANDLE_VALUE) {
		do {
			if(*FindFileData.cFileName != _T('.')) {
				if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					// directories
					CString sNewPath;
					sNewPath.Format(_T("%s/%s"), sPath, FindFileData.cFileName);
					SearchLastWriteFileTime(FileTime, sNewPath, sExts);	// recursive search
				} else {
					// files
					if(*(ULONGLONG*)&FindFileData.ftLastWriteTime > *(ULONGLONG*)&FileTime) {
						CString sExt(FindFileData.cFileName);
						{
							// make path to ext. only
							int iPos	= sExt.ReverseFind(_T('.'));

							if(iPos > 0) sExt.Delete(0, iPos + 1);
						}

						// ext. list check
						for(int i = 0; sExts[i]; i++) {
							if(!sExt.Compare(sExts[i])) {
								FileTime	= FindFileData.ftLastWriteTime;
								break;
							}
						}
					}
				}
			}
		} while(FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}
}

BOOL IsFileExisted(LPCTSTR sPath)
{
	return (_taccess(g_pSystem->RetrieveFullPath(sPath), 0) != -1);
}

CString GetTodayString(int* wday)
{
	time_t cur;
	struct tm* ptm;
	char	sDate[MAX_PATH];
	cur = time(NULL);
	ptm = localtime(&cur);
	strftime(sDate, MAX_PATH, "%#m/%#d/%Y", ptm);

	if(wday) *wday = ptm->tm_wday;

	return sDate;
}
