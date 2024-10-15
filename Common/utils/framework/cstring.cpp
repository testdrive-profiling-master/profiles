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
// Title : utility framework
// Rev.  : 10/15/2024 Tue (clonextop@gmail.com)
//================================================================================
#include "STDInterface.h"
// cstrings
#include "cstring.h"
#include <stdarg.h>
#include <vector>
#ifndef UNUSE_CSTRING_ICONV
#	include <iconv.h>
#endif

cstring::cstring(void) {}

cstring::cstring(const string &s)
{
	if (s.c_str())
		m_sStr = s.c_str();
}

cstring::cstring(const char *s)
{
	if (s)
		m_sStr = s;
}

cstring::cstring(const char *s, size_t size) : m_sStr(s, size) // specific size setup
{
}

#ifndef UNUSE_CSTRING_ICONV
cstring::cstring(const wchar_t *s) : cstring(s, wcslen(s)) {}

cstring::cstring(const wchar_t *s, size_t size)
{
	Append(s, size);
}
#endif

cstring::~cstring(void) {}

bool cstring::operator>=(const cstring &s)
{
	return Compare(s) <= 0;
}

bool cstring::operator<=(const cstring &s)
{
	return Compare(s) >= 0;
}

bool cstring::operator>(const cstring &s)
{
	return Compare(s) < 0;
}

bool cstring::operator<(const cstring &s)
{
	return Compare(s) > 0;
}

bool cstring::operator==(const char *s)
{
	if (!s)
		return false;

	return Compare(s) == 0;
}

bool cstring::operator!=(const char *s)
{
	if (!s)
		return false;

	return Compare(s) != 0;
}

cstring &cstring::operator=(const char *s)
{
	if (s)
		m_sStr = s;
	else
		m_sStr.clear();

	return *this;
}

cstring &cstring::operator+=(const char *s)
{
	if (s)
		m_sStr += s;

	return *this;
}

cstring &cstring::operator+=(char ch)
{
	m_sStr += ch;
	return *this;
}

cstring cstring::operator+(const char *s) const
{
	cstring sStr(m_sStr);

	if (s)
		sStr += s;

	return sStr;
}

char &cstring::operator[](int iIndex)
{
	return m_sStr[iIndex];
}

#ifndef UNUSE_CSTRING_ICONV
cstring::operator wstring(void) const
{
	wstring wStr;
	size_t	in_size = Length();
	if (in_size) {
		iconv_t	 it		  = iconv_open("WCHAR_T", "UTF-8");
		size_t	 out_size = in_size * 2;
		wchar_t *ws		  = new wchar_t[in_size];
		char	*in_s	  = (char *)m_sStr.c_str();

		while (in_size) {
			char  *out_s	 = (char *)ws;
			size_t left_size = out_size;
			iconv(it, &in_s, &in_size, &out_s, &left_size);
			wStr.append(ws, (out_size - left_size) >> 1);
		}
		delete[] ws;
		iconv_close(it);
	}
	return wStr;
}
#endif

int cstring::Compare(const char *s)
{
	const char *s1 = c_str();
	const char *s2 = s;

	for (;;) {
		char c1 = *s1;
		char c2 = *s2;

		if (!c1 && !c2)
			return 0;

		if (c1 != c2) {
			if (isdigit(c1) && isdigit(c2)) {
				return (atoi(s1) > atoi(s2)) ? -1 : 1;
			} else {
				return (c1 > c2) ? -1 : 1;
			}
		} else if (isdigit(c1) && isdigit(c2)) {
			int i1 = atoi(s1);
			int i2 = atoi(s2);

			if (i1 != i2)
				return i1 > i2 ? -1 : 1;
		}

		s1++;
		s2++;
	}

	return 0;
}

bool cstring::CompareFront(const char *s) const
{
	if (!s)
		return false;

	return (m_sStr.find(s) == 0);
}

bool cstring::CompareBack(const char *s) const
{
	if (!s)
		return false;

	int iPos = m_sStr.rfind(s);

	if (iPos < 0)
		return false;

	return (iPos == Length() - strlen(s));
}

bool cstring::CutFront(const char *s, bool bRecursive)
{
	if (s) {
		int iPos = bRecursive ? m_sStr.rfind(s) : m_sStr.find(s);

		if (iPos >= 0) {
			m_sStr.erase(0, iPos + strlen(s));
			return true;
		}
	}

	return false;
}

bool cstring::CutBack(const char *s, bool bRecursive)
{
	if (s) {
		int iPos = bRecursive ? m_sStr.find(s) : m_sStr.rfind(s);

		if (iPos >= 0) {
			m_sStr.erase(iPos, -1);
			return true;
		}
	}

	return false;
}

bool cstring::DeleteFront(const char *s)
{
	if (s) {
		int iPos = m_sStr.find(s);

		if (iPos >= 0) {
			m_sStr.erase(iPos, strlen(s));
			return true;
		}
	}

	return false;
}
bool cstring::DeleteBack(const char *s)
{
	if (s) {
		int iPos = m_sStr.rfind(s);

		if (iPos >= 0) {
			m_sStr.erase(iPos, strlen(s));
			return true;
		}
	}

	return false;
}

bool cstring::DeleteBlock(const char *sExpression, int iPos)
{
	cstring s;
	return FindVariableString(s, sExpression, iPos) >= 0;
}

// search string with bypass brackets on string
static bool __RetrieveBracketString(const char *sSearch, int &iPos, const char *sEnd)
{
	bool bString = (strcmp(sEnd, "\"") == 0);
	int	 iLen	 = strlen(sEnd);

	for (;; iPos++) {
		switch (sSearch[iPos]) {
		case '\0':
			return false;
			break;

		case '(':
			if (bString)
				break;

			iPos++;

			if (!__RetrieveBracketString(sSearch, iPos, ")"))
				return false;

			break;

		case '{':
			if (bString)
				break;

			iPos++;

			if (!__RetrieveBracketString(sSearch, iPos, "}"))
				return false;

			break;

		case '[':
			if (bString)
				break;

			iPos++;

			if (!__RetrieveBracketString(sSearch, iPos, "]"))
				return false;

			break;

		case '\"':
			if (bString)
				return true;

			if (!__RetrieveBracketString(sSearch, iPos, "\""))
				return false;

			break;

		case '\\':
			iPos++;

			if (bString) {
				if (!sSearch[iPos] == '\"') { // string in string
					iPos++;
					break;
				}
			}

			if (!sSearch[iPos])
				return false;

			break;

		default:
			if (bString)
				break;

			if (!strncmp(&sSearch[iPos], sEnd, iLen))
				return true;

			break;
		}
	}

	return false;
}

int cstring::FindVariableString(cstring &sVar, const char *sExpression, int iPos)
{
	if (sExpression) {
		cstring sStart = sExpression;
		cstring sEnd   = sExpression;
		int		iIndex;

		if ((iIndex = sStart.find("*")) > 0) {
			// seperate start/end tag
			sStart.erase(iIndex, -1);
			sEnd.erase(0, iIndex + 1);

			if (sStart.size() && sEnd.size() && (iPos = find(sStart, iPos)) >= 0) {
				erase(iPos, sStart.size()); // delete start TAG
				sVar = m_sStr.c_str() + iPos;
				// find end TAG
				int iEnd = iPos; // find(sEnd, iPos);

				if (!__RetrieveBracketString(m_sStr.c_str(), iEnd, sEnd)) {
					iEnd = -1;
				}

				if (iEnd > iPos) {
					int iSize = iEnd - iPos;
					sVar.erase(iSize, -1);
					erase(iPos, iSize + sEnd.size());
					return iPos;
				}
			}
		}
	}

	return -1;
}

int cstring::FindNameString(const char *sName, int iPos)
{
	if (sName) {
		int iLen = strlen(sName);

		if (iLen) {
		RETRY:
			iPos = find(sName, iPos);

			if (iPos >= 0) {
				char ch;

				// front check
				if (iPos > 0) {
					ch = m_sStr[iPos - 1];

					if (ch == '_' || isalpha(ch) || isdigit(ch)) {
						iPos += iLen;
						goto RETRY;
					}
				}

				// back check
				ch = m_sStr[iPos + iLen];

				if (ch == '_' || isalpha(ch) || isdigit(ch)) {
					iPos += iLen;
					goto RETRY;
				}

				return iPos;
			}
		}
	}

	return -1;
}

void cstring::MakeUpper(void)
{
	int iLen = Length();

	for (int i = 0; i < iLen; i++) {
		m_sStr[i] = toupper(m_sStr[i]);
	}
}

void cstring::MakeLower(void)
{
	int iLen = Length();

	for (int i = 0; i < iLen; i++) {
		m_sStr[i] = tolower(m_sStr[i]);
	}
}

bool cstring::Replace(const char *sSearch, const char *sReplace, bool bRecursive)
{
	bool bRet		= false;
	int	 iPos		= 0;
	int	 iSearchLen = sSearch ? strlen(sSearch) : 0;

	if (sSearch != NULL && iSearchLen != 0) {
		int iReplaceLen = sReplace ? strlen(sReplace) : 0;

		while ((iPos = m_sStr.find(sSearch, iPos)) >= 0) {
			bRet = true;
			m_sStr.erase(iPos, iSearchLen);

			if (iReplaceLen) {
				m_sStr.insert(iPos, sReplace);
				iPos += iReplaceLen;
			}

			if (!bRecursive)
				break;
		}
	}

	return bRet;
}

bool cstring::ReplaceVariable(const char *sSearch, const char *sReplace)
{
	bool bRet		= false;
	int	 iPos		= 0;
	int	 iSearchLen = sSearch ? strlen(sSearch) : 0;

	if (sSearch != NULL && iSearchLen != 0) {
		int iReplaceLen = sReplace ? strlen(sReplace) : 0;

		while ((iPos = m_sStr.find(sSearch, iPos)) >= 0) {
			bRet = true;

			// pre-fix error check
			if (iPos) {
				char ch = m_sStr[iPos - 1];

				if (isalpha(ch) || isdigit(ch) || ch == '_') {
					iPos += iSearchLen;
					continue;
				}
			}

			// post-fix error check
			if (iPos + iSearchLen < m_sStr.length()) {
				char ch = m_sStr[iPos + iSearchLen];

				if (isalpha(ch) || isdigit(ch) || ch == '_') {
					iPos += iSearchLen;
					continue;
				}
			}

			m_sStr.erase(iPos, iSearchLen);

			if (iReplaceLen) {
				m_sStr.insert(iPos, sReplace);
				iPos += iReplaceLen;
			}
		}
	}

	return bRet;
}

void cstring::TrimLeft(const char *sDelim)
{
	const char *s = c_str();
	char		ch;

	if (!sDelim)
		return;

	cstring csDelim(sDelim);

	while (Length() > 0) {
		ch = m_sStr[0];

		if (!ch || csDelim.find(ch) < 0)
			break;

		m_sStr.erase(0, 1);
	}
}
void cstring::TrimRight(const char *sDelim)
{
	const char *s = c_str();
	char		ch;

	if (!sDelim)
		return;

	cstring csDelim(sDelim);

	for (int iPos = Length() - 1; iPos >= 0; iPos--) {
		ch = s[iPos];

		if (!ch || csDelim.find(ch) < 0)
			break;

		m_sStr.erase(iPos, 1);
	}
}

void cstring::Trim(const char *sDelim)
{
	TrimLeft(sDelim);
	TrimRight(sDelim);
}

cstring cstring::Tokenize(int &iPos, const char *sDelim)
{
	cstring str;

	if (iPos >= 0 && iPos < Length()) {
		const char *s = c_str() + iPos;

		if (sDelim) { // tokenize by deliminator
			cstring __sDelim(sDelim);

			// trim deliminator
			while (*s != '\0' && (__sDelim.find(*s) >= 0)) {
				iPos++;
				s++;
			}

			// gets
			while (*s != '\0' && (__sDelim.find(*s) < 0)) {
				str += *s;
				iPos++;
				s++;
			}
		} else { // tokenize by lexer
			static cstring __sDelim(" \t\r\n");

			// pass through to first char
			while (__sDelim.find(*s) >= 0) {
				iPos++;
				s++;
			}

			{
				// tokenize
				char ch = *s;
				s++;

				if (ch) {
					if (ch == '\"') { // string
						char ch_prev = 0;

						while (ch) {
							str += ch;

							if (ch_prev != '\\' && ch == '\"')
								break;

							ch_prev = ch;
							ch		= *s;
							s++;
						}
					} else if (isalpha(ch) || isdigit(ch) || ch == '.' || ch == '_') { // variable or number
						while (isalpha(ch) || isdigit(ch) || ch == '.' || ch == '_') {
							str += ch;
							ch = *s;
							s++;
						}
					} else { // and etc... (single charactor)
						str += ch;
					}

					iPos += str.size();
				}
			}
		}
	}

	if (!str.size())
		iPos = -1;

	return str;
}

static bool __SearchBracket(const char *sSearch, int &iPos, const char *sEnd = NULL)
{
	bool bString = sEnd ? (strcmp(sEnd, "\"") == 0) : false;
	int	 iLen	 = sEnd ? strlen(sEnd) : 0;

	for (;; iPos++) {
		switch (sSearch[iPos]) {
		case '\0':
			return false;

		case '(':
			if (bString)
				break;

			iPos++;
			return __SearchBracket(sSearch, iPos, ")");

		case '{':
			if (bString)
				break;

			iPos++;
			return __SearchBracket(sSearch, iPos, "}");

		case '[':
			if (bString)
				break;

			iPos++;
			return __SearchBracket(sSearch, iPos, "]");

		case '\"':
			if (bString)
				return true;

			iPos++;
			return __SearchBracket(sSearch, iPos, "\"");

		case '\\':
			if (bString) {
				iPos++;

				if (!sSearch[iPos])
					return false;
			}

			break;

		default:
			if (sEnd && !bString && !strncmp(&sSearch[iPos], sEnd, iLen))
				return true;

			break;
		}
	}

	return false;
}

int cstring::SearchBraket(int iPos)
{
	if (iPos >= 0 && iPos < Length()) {
		if (__SearchBracket(c_str(), iPos)) {
			return iPos;
		}
	}

	return -1;
}

int cstring::Length(void) const
{
	return strlen(c_str());
}

int cstring::RetrieveTag(const char **sTagList, int iTagSize)
{
	for (int i = 0; iTagSize ? (i < iTagSize) : (sTagList[i] != NULL); i++) {
		if (m_sStr == sTagList[i])
			return i;
	}

	return -1;
}

void cstring::Format(const char *sFormat, ...)
{
	if (sFormat) {
		int		iLen = 0;
		va_list vaArgs;
		va_start(vaArgs, sFormat);
		{
			// get size only
			va_list vaCopy;
			va_copy(vaCopy, vaArgs);
			iLen = std::vsnprintf(NULL, 0, sFormat, vaCopy);
			va_end(vaCopy);
		}
		{
			char			 *pBuff = new char[iLen + 1];
			std::vector<char> zc(iLen + 1);
			std::vsnprintf(pBuff, iLen + 1, sFormat, vaArgs);
			va_end(vaArgs);
			{
				// write to string
				m_sStr = pBuff;
			}
			delete[] pBuff;
		}
	}
}

void cstring::AppendFormat(const char *sFormat, ...)
{
	if (sFormat) {
		int		iLen = 0;
		va_list vaArgs;
		va_start(vaArgs, sFormat);
		{
			// get size only
			va_list vaCopy;
			va_copy(vaCopy, vaArgs);
			iLen = std::vsnprintf(NULL, 0, sFormat, vaCopy);
			va_end(vaCopy);
		}
		{
			char			 *pBuff = new char[iLen + 1];
			std::vector<char> zc(iLen + 1);
			std::vsnprintf(pBuff, iLen + 1, sFormat, vaArgs);
			va_end(vaArgs);
			{
				// write to string
				m_sStr += pBuff;
			}
			delete[] pBuff;
		}
	}
}

void cstring::Set(const char *sStr)
{
	if (sStr)
		m_sStr = sStr;
	else
		m_sStr.clear();
}

void cstring::Append(const char *sStr)
{
	if (sStr)
		m_sStr += sStr;
}

void cstring::Append(char ch)
{
	if (ch)
		m_sStr += ch;
}

#ifndef UNUSE_CSTRING_ICONV
void cstring::Append(const wchar_t *sStr)
{
	Append(sStr, wcslen(sStr));
}

void cstring::Append(const wchar_t *sStr, size_t size)
{
	if (sStr && size) {
		iconv_t it	 = iconv_open("UTF-8", "WCHAR_T");
		char   *in_s = (char *)sStr;
		char	dst_s[4096];
		size *= 2;

		while (size) {
			char  *out_s	= dst_s;
			size_t out_size = 4096;
			iconv(it, &in_s, &size, &out_s, &out_size);
			m_sStr.append(dst_s, 4096 - out_size);
		}

		iconv_close(it);
	}
}
#endif

int cstring::CheckFileExtension(const char **sExtList)
{
	if (sExtList) {
		cstring sExt = m_sStr;

		if (sExt.CutFront(".", true) && sExt.size() > 0) {
			static const char *sDelim = " ,;";
			sExt.MakeLower();

			for (int i = 0; sExtList[i]; i++) {
				int		iPos = 0;
				cstring sExtCmp(sExtList[i]);

				while (iPos >= 0) {
					cstring sExt2 = sExtCmp.Tokenize(iPos, sDelim);

					if (iPos >= 0 && sExt == sExt2) {
						return i;
					}
				}
			}
		}
	}

	return -1;
}

static const char *__sEnvDelim = "@";
#ifdef WIN32
#	include <windows.h>
#	include <tchar.h>
static const char *__sEnvFileName = "testdrive.ini";

static bool		   __GetEnvString(cstring sKey, cstring &sAppName, cstring &sKeyName, cstring &sEnvPath)
{
	if (!sEnvPath.GetEnvironment(_T("TESTDRIVE_DIR")))
		return false;

	sEnvPath.Append(__sEnvFileName);
	int iPos = 0;
	sKeyName = sKey.Tokenize(iPos, __sEnvDelim);
	sAppName = sKey.Tokenize(iPos, __sEnvDelim);

	if (sKeyName.IsEmpty() || sAppName.IsEmpty())
		return false;

	return true;
}
#endif

bool cstring::GetEnvironment(const char *sKey)
{
	if (sKey) {
		if (strstr(sKey, __sEnvDelim)) {
#ifdef WIN32
			bool bRet = true;
			// in testdrive.ini
			m_sStr.clear();
			cstring sAppName, sKeyName, sEnvPath;

			if (!__GetEnvString(sKey, sAppName, sKeyName, sEnvPath))
				return false;

			{
				char *sData = new char[1024 * 64];
				*sData		= 0;

				if (GetPrivateProfileString(sAppName, sKeyName, NULL, sData, 1024 * 64, sEnvPath))
					m_sStr = sData;
				else
					bRet = false;

				delete[] sData;
			}
			return bRet;
#else
			return false;
#endif
		} else {
			char *sEnv = getenv(sKey);

			if (sEnv) {
				m_sStr = sEnv;
				return true;
			} else {
				m_sStr.clear();
			}
		}
	}

	return false;
}

#ifdef WIN32
static void setenv(const char *sKey, const char *sData, int replace)
{
	cstring sEnv;
	sEnv.Format("%s=%s", sKey, sData);
	putenv(sEnv);
}
#endif

void cstring::SetEnvironment(const char *sKey)
{
	if (sKey) {
		if (strstr(sKey, __sEnvDelim)) {
#ifdef WIN32
			// in testdrive.ini
			cstring sAppName, sKeyName, sEnvPath;

			if (!__GetEnvString(sKey, sAppName, sKeyName, sEnvPath))
				return;

			WritePrivateProfileString(sAppName, sKeyName, m_sStr.c_str(), sEnvPath);
#endif
		} else {
			setenv(sKey, m_sStr.c_str(), 1);
		}
	}
}

static bool is_utf8(const char *string)
{
	if (!string)
		return true;

	const unsigned char *bytes = (const unsigned char *)string;
	int					 num;

	while (*bytes != 0x00) {
		if ((*bytes & 0x80) == 0x00) {
			// U+0000 to U+007F
			num = 1;
		} else if ((*bytes & 0xE0) == 0xC0) {
			// U+0080 to U+07FF
			num = 2;
		} else if ((*bytes & 0xF0) == 0xE0) {
			// U+0800 to U+FFFF
			num = 3;
		} else if ((*bytes & 0xF8) == 0xF0) {
			// U+10000 to U+10FFFF
			num = 4;
		} else
			return false;

		bytes += 1;

		for (int i = 1; i < num; ++i) {
			if ((*bytes & 0xC0) != 0x80)
				return false;

			bytes += 1;
		}
	}

	return true;
}

bool cstring::ChangeCharset(const char *szSrcCharset, const char *szDstCharset)
{
#ifndef UNUSE_CSTRING_ICONV
	iconv_t it = iconv_open(szDstCharset, szSrcCharset);

	if (it != (iconv_t)(-1)) {
		size_t nSrcStrLen = Length();
		size_t nDstStrLen = nSrcStrLen * 2 + 16;
		char  *sInput	  = (char *)m_sStr.c_str();
		char  *sOut		  = new char[nDstStrLen];
		string sResult;

		while (nSrcStrLen) {
			char  *sOutput	 = sOut;
			size_t sOut_size = nDstStrLen;
			iconv(it, &sInput, &nSrcStrLen, &sOutput, &sOut_size);
			sResult.append(sOut, nDstStrLen - sOut_size);
		}
		m_sStr = sResult;

		iconv_close(it);
		delete[] sOut;
		return true;
	}
#endif
	return false;
}

bool cstring::ChangeCharsetToUTF8(void)
{
#ifndef UNUSE_CSTRING_ICONV
	if (is_utf8(m_sStr.c_str()))
		return true;

	return ChangeCharset("EUC-KR", "UTF-8//IGNORE");
#else
	return false;
#endif
}

bool cstring::ChangeCharsetToANSI(void)
{
#ifndef UNUSE_CSTRING_ICONV
	if (!is_utf8(m_sStr.c_str()))
		return true;

	return ChangeCharset("UTF-8", "EUC-KR//IGNORE");
#else
	return false;
#endif
}
