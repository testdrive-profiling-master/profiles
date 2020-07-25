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
#include "SubSystemList.h"

LPCTSTR g_sConfigPath			= _T("%PROJECT%Profiles/Config.ini");
static LPCTSTR		__sAppName	= _T("System");

SubSystemList::SubSystemList(void)
{
}

SubSystemList::~SubSystemList(void)
{
	DeleteAll();
}

void SubSystemList::DeleteAll(void)
{
	for(SubSystemMap::iterator i = m_List.begin(); i != m_List.end(); i++) {
		delete i->second;
	}

	m_List.clear();
}

void SubSystemList::Initialize(void)
{
	DeleteAll();
	{
		CString	sConfigPath	= g_pSystem->RetrieveFullPath(g_sConfigPath);
		CString sKey;
		TCHAR	sStr[MAX_PATH];

		for(int i = 0;; i++) {
			sKey.Format(_T("SubSystem[%d]"), i);
			GetPrivateProfileString(__sAppName, sKey, _T(""), sStr, MAX_PATH, sConfigPath);

			if(!*sStr) break;

			{
				CString	sFile	= sStr;
				CString sName	= sStr;
				int	iPos		= sFile.Find(_T('@'));

				if(iPos > 0) {
					sName	= &(sStr[iPos + 1]);
					sStr[iPos]	= '\0';
					sFile	= sStr;
				} else {
					sName	= sFile;
				}

				Insert(i, sFile, sName);
			}
		}
	}
}

SubSystemItem* SubSystemList::Find(LPCTSTR sName)
{
	for(SubSystemMap::iterator i = m_List.begin(); i != m_List.end(); i++) {
		if(!(i->second->sName.Compare(sName)))
			return i->second;
	}

	return NULL;
}

SubSystemItem* SubSystemList::Item(int index)
{
	return m_List[index];
}

int	SubSystemList::Size(void)
{
	return m_List.size();
}

void SubSystemList::Insert(int i, LPCTSTR sFile, LPCTSTR sName)
{
	SubSystemItem* pItem	= new SubSystemItem;
	pItem->sFile	= sFile;
	pItem->sName	= sName ? sName : sFile;
	m_List[i]		= pItem;
}
