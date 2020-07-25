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
#include "TopDesignList.h"

TopDesignList::TopDesignList(void)
{
}

TopDesignList::~TopDesignList(void)
{
	DeleteAll();
}

void TopDesignList::DeleteAll(void)
{
	for(TopDesignMap::iterator i = m_List.begin(); i != m_List.end(); i++) {
		delete i->second;
	}

	m_List.clear();
}

TopDesignItem* TopDesignList::FindDesign(LPCTSTR sTopPath)
{
	for(int i = 0; i < Size(); i++) {
		TopDesignItem*	pItem	= Item(i);

		if(!pItem->sTopPath.Compare(sTopPath)) return pItem;
	}

	return NULL;
}

TopDesignItem* TopDesignList::AddTopDesign(LPCTSTR sTopPath)
{
	TopDesignItem* pItem	= FindDesign(sTopPath);

	if(!pItem) {
		// add new
		pItem				= new TopDesignItem;
		pItem->sTopPath		= sTopPath;
		m_List[Size()]		= pItem;
	}

	return pItem;
}

TopDesignItem* TopDesignList::Item(int index)
{
	return m_List[index];
}

int	TopDesignList::Size(void)
{
	return m_List.size();
}

static BOOL __SearchTopDesign(LPCTSTR sPath, TopDesignList* pList)
{
	BOOL	bRet	= TRUE;
	FILE*	fp		= _tfopen(sPath, _T("rt"));
	int		iLine	= 0;
	CString	sCurDir(sPath);
	{
		// get relative path for current design
		sCurDir.Replace(g_pSystem->RetrieveFullPath(_T("%PROJECT%System/HDL/")), _T(""));
		sCurDir.Replace(_T('\\'), _T('/'));
		int iPos = sCurDir.ReverseFind(_T('/'));
		sCurDir[iPos + 1]	= _T('\0');
	}

	if(fp) {
		TCHAR*			sLine	= new TCHAR[1024 * 64];
		TopDesignItem*	pTop	= NULL;

		while(_fgetts(sLine, 1024 * 64, fp)) {
			CString sMsg(sLine);
			iLine++;
			// 林籍 力芭
			sMsg.Replace(_T("//"), _T("\0"));
			sMsg.Replace(_T("#"), _T("\0"));
			// 内靛 力芭
			sMsg.TrimLeft(_T("\t "));
			sMsg.TrimRight(_T("\r\n\t "));
			{
				CString	sID, sCode;
				int iPos		= 0;
				LPCTSTR sDelim	= _T("\t =:");
				sID		= sMsg.Tokenize(sDelim, iPos);

				if(iPos >= 0) {
					sCode	= &(sMsg[iPos]);
					sCode.TrimLeft(sDelim);

					if(!sID.Compare(_T("TOP_DESIGN"))) {
						if(!sCode[0]) {
							g_pSystem->LogError(_L(CONTENTS_IS_EMPTY), (LPCTSTR)sMsg, sPath, iLine);
							bRet	= FALSE;
							goto EXIT;
						}

						CString sTopPath;
						sTopPath.Format(_T("%s%s"), (LPCTSTR)sCurDir, (LPCTSTR)sCode);
						pTop	= pList->AddTopDesign(sTopPath);
					} else if(!sID.Compare(_T("OPTION"))) {
						if(!pTop) {
							g_pSystem->LogError(_L(TOPDESIGN_FIRST), (LPCTSTR)sMsg, sPath, iLine);
							bRet	= FALSE;
							goto EXIT;
						}

						if(sCode[0]) {
							pTop->OptionList.push_back(sCode);
						}
					} else {
						g_pSystem->LogError(_L(UNRECOGNIZED_CODE), (LPCTSTR)sID, sPath, iLine);
						bRet	= FALSE;
						goto EXIT;
					}
				}
			}
		}

	EXIT:
		fclose(fp);
		delete [] sLine;
	}

	return bRet;
}

BOOL TopDesignList::Initialize(void)
{
	DeleteAll();
	return g_pSystem->SearchSubPathFile(_T("%PROJECT%System/HDL"), _T("top.list"), (SEARCH_FILE_FUNCTION)__SearchTopDesign, (LPVOID)this);
}
