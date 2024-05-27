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
// Rev.  : 5/27/2024 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __UTIL_FRAMEWORK_H__
#define __UTIL_FRAMEWORK_H__

#define _USE_MATH_DEFINES

#include "STDInterface.h"
// extras
#include <algorithm>
#include <assert.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
// extended functions
#include "ArgTable.h"
#include "TestDriver.h"
#include "TextFile.h"
#include "cstring.h"

using namespace std;

#ifdef __MINGW64__
#	define lstat stat
#endif

typedef enum { LOG_MODE_INFO, LOG_MODE_WARNING, LOG_MODE_ERROR } LOG_MODE;

bool isVarChar(char ch);
void LOG(LOG_MODE id, const char *sFormat, ...);
void LOG_Suppress(bool bSuppress = true);
#define LOGI(...) LOG(LOG_MODE_INFO, __VA_ARGS__)
#define LOGW(...) LOG(LOG_MODE_WARNING, __VA_ARGS__)
#define LOGE(...) LOG(LOG_MODE_ERROR, __VA_ARGS__)
extern int g_log_warning_count;
extern int g_log_error_count;

class Reference
{
public:
	Reference(void);
	virtual ~Reference(void);

	void AddRef(void);
	void Release(void);

private:
	unsigned int m_dwReferCount;
};

template <class T> class Object : public Reference
{
public:
	Object(void)
	{
		m_pRTTI = &m_RTTI;
	}
	virtual ~Object(void) {}

	bool IsValidObject(void)
	{
		return m_pRTTI == RTTI();
	}
	static const void *RTTI(void)
	{
		return &m_RTTI;
	}

private:
	static int m_RTTI;
	void	  *m_pRTTI;
};

template <class T> int Object<T>::m_RTTI = NULL;

template <typename T> class NamedMap
{
public:
	NamedMap(void) {}
	virtual ~NamedMap(void)
	{
		ReleaseAll();
	}

	void ReleaseAll(void)
	{
		for (auto i = m_List.begin(); i != m_List.end(); i++) {
			SAFE_RELEASE(i->second);
		}

		m_List.clear();
	}

	T *Find(const char *sName)
	{
		auto i = m_List.find(sName);

		if (i == m_List.end())
			return NULL;

		return i->second;
	}

	T *operator[](const char *sName)
	{
		return Find(sName);
	}

	T *Create(const char *sName)
	{
		T *pNode = Find(sName);

		if (!pNode) {
			pNode		  = new T;
			m_List[sName] = pNode;
		}

		return pNode;
	}

	void Set(const char *sName, T *pNode)
	{
		auto i = m_List.find(sName);

		if (pNode)
			pNode->AddRef();

		if (i != m_List.end()) {
			SAFE_RELEASE(i->second);
			i->second = pNode;
		} else {
			m_List[sName] = pNode;
		}
	}

	inline map<string, T *> &List(void)
	{
		return m_List;
	}

private:
	map<string, T *> m_List;
};

template <typename T> class AutoList
{
public:
	AutoList(void)
	{
		m_pNext = m_pHead;
		m_pHead = (T *)this;
	}
	virtual ~AutoList(void)
	{
		if (m_pHead == (T *)this) {
			m_pHead = m_pNext;
		} else {
			T *pNode = m_pHead;

			while (pNode) {
				if (pNode->m_pNext == (T *)this) {
					pNode->m_pNext = m_pNext;
					break;
				}

				pNode = pNode->m_pNext;
			}
		}
	}
	static void ReleaseAll(void)
	{
		while (Head()) delete Head();
	}
	inline void Release(void)
	{
		delete this;
	}
	inline static T *Head(void)
	{
		return m_pHead;
	}
	inline T *Next(void)
	{
		return m_pNext;
	}

protected:
	static T *m_pHead;
	T		 *m_pNext;
};

template <typename T> T *AutoList<T>::m_pHead = NULL;

template <typename T> class ClassType
{
public:
	ClassType(void)
	{
		m_pClassID = &m_ClassID;
	}
	virtual ~ClassType(void) {}

	inline void *ClassID(void)
	{
		return &m_ClassID;
	}
	inline bool IsValidClass(void)
	{
		return (m_pClassID == &m_ClassID);
	}

private:
	void	  *m_pClassID;
	static int m_ClassID;
};

template <typename T> int ClassType<T>::m_ClassID = 0;

string					  GetCommonToolPath(void);

#endif //__UTIL_FRAMEWORK_H__
