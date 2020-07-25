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
// Title : Common profiles
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __LOCALE_STRING_H__
#define __LOCALE_STRING_H__
#include "TestDrive.h"

#define LOCALE_USE	LocaleString __locale__;

#define LOCALE_DEFINE(size, ...) \
	class LocaleString{ \
	private: \
		DWORD		m_dwLangSize; \
		DWORD*		m_pLocaleList; \
		DWORD		m_dwLang; \
	public: \
		LocaleString(void) { \
			static DWORD lang[size] = {__VA_ARGS__}; \
			m_dwLangSize	= size; \
			m_dwLang		= 0; \
			m_pLocaleList	= lang; \
		} \
		~LocaleString(void)	{} \
		void Initialize(DWORD dwLang){ \
			for(m_dwLang = 0;m_dwLang<m_dwLangSize;m_dwLang++) \
				if(m_pLocaleList[m_dwLang] == dwLang) return; \
			m_dwLang--; \
		} \
		inline LPCTSTR Get(LPCTSTR* str_list){ \
			return str_list[m_dwLang]; \
		}

#define LOCALE_TEXT(name, ...) \
	LPCTSTR* str__##name(void){ \
		static LPCTSTR name[] = {__VA_ARGS__}; \
		return name; \
	}

#define LOCALE_END \
	}; \
	extern LocaleString __locale__;

// setup locale string
#define LOCALE_INITIALIZE(TDSystem) __locale__.Initialize(TDSystem->GetLocale())

// usage
#define _L(name)	__locale__.Get(__locale__.str__##name())

// create document
#define REGISTER_LOCALED_DOCUMENT(DocType) \
	LOCALE_USE \
	ITDSystem*	g_pSystem	= NULL; \
	__declspec(dllexport) ITDImplDocument* __cdecl RegisterDocument(ITDDocument* pDoc){ \
		g_pSystem	= pDoc->GetSystem();\
		LOCALE_INITIALIZE(g_pSystem); \
		if(g_pSystem->GetVersion() != TESTDRIVE_VERSION) return NULL; \
		pDoc->DocumentTitle(_L(DOCUMENT_TITLE)); \
		return new DocType(pDoc);\
	}

#define REGISTER_LOCALED_DOCUMENT_EX(DocType) \
	LOCALE_USE \
	ITDSystem*	g_pSystem	= NULL; \
	static BOOL __DocumentCheck(ITDDocument* pDoc);\
	__declspec(dllexport) ITDImplDocument* __cdecl RegisterDocument(ITDDocument* pDoc){ \
		g_pSystem	= pDoc->GetSystem();\
		LOCALE_INITIALIZE(g_pSystem);\
		if(g_pSystem->GetVersion() != TESTDRIVE_VERSION) return NULL;\
		if(!__DocumentCheck(pDoc)) return NULL; \
		return new DocType(pDoc);\
	} \
	static BOOL __DocumentCheck(ITDDocument* pDoc)

#endif//__LOCALE_STRING_H__
