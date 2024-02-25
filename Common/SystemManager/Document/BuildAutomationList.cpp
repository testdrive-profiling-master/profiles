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
// Title : System manager
// Rev.  : 2/26/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "BuildAutomationList.h"

extern LPCTSTR g_sConfigPath;
static LPCTSTR __sAppName = _T("Build Automation");

BuildAutomationList::BuildAutomationList(void) {}

BuildAutomationList::~BuildAutomationList(void)
{
	DeleteAll();
}

void BuildAutomationList::DeleteAll(void)
{
	for (auto &i : m_List) delete i.second;

	m_List.clear();
}

BuildAutomationItem *BuildAutomationList::Item(int index)
{
	return m_List[index];
}

int BuildAutomationList::Size(void)
{
	return m_List.size();
}

void BuildAutomationList::Initialize(void)
{
	DeleteAll();
	{
		CString sConfigPath = g_pSystem->RetrieveFullPath(g_sConfigPath);
		CString sItem;
		TCHAR	sStr[MAX_PATH];

		for (int i = 0;; i++) {
			sItem.Format(_T("Item[%d]."), i);
			GetPrivateProfileString(__sAppName, sItem + _T("title"), _T(""), sStr, MAX_PATH, sConfigPath);

			if (!*sStr)
				break;

			{
				BuildAutomationItem *pItem = new BuildAutomationItem;
				pItem->sTitle			   = sStr;
				GetPrivateProfileString(__sAppName, sItem + _T("extensions"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sExtensions.Format(_T(";%s;"), sStr);
				pItem->sExtensions.MakeLower();
				GetPrivateProfileString(__sAppName, sItem + _T("inception"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sInceptionFile = sStr;
				GetPrivateProfileString(__sAppName, sItem + _T("error_string"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sErrorString = sStr;
				pItem->iLinkItem	= GetPrivateProfileInt(__sAppName, sItem + _T("link_item"), -1, sConfigPath);
				GetPrivateProfileString(__sAppName, sItem + _T("watch_path"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sWatchPath	 = sStr;
				pItem->bSearchSubDir = GetPrivateProfileInt(__sAppName, sItem + _T("search_subdir"), 0, sConfigPath);
				GetPrivateProfileString(__sAppName, sItem + _T("work_path"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sWorkPath = *sStr ? sStr : pItem->sWatchPath;
				GetPrivateProfileString(__sAppName, sItem + _T("target"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sExecuteFile = sStr;
				GetPrivateProfileString(__sAppName, sItem + _T("argument"), _T(""), sStr, MAX_PATH, sConfigPath);
				pItem->sArgument = sStr;
				m_List[i]		 = pItem;
			}
		}
	}
}
