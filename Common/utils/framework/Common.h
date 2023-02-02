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
// Title : utility framework
// Rev.  : 2/2/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <memory.h>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include "cstring.h"
#include "TextFile.h"
#include "ArgTable.h"
#include "TestDriver.h"

using namespace std;

#ifdef WIN32
#include <windows.h>
#else
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned int			DWORD;
#endif

// Safe memory deletion
#define SAFE_DELETE(x)			if(x){delete x;			(x) = NULL;}
#define SAFE_DELETE_ARRAY(x)	if(x){delete [] x;		(x) = NULL;}
#define SAFE_RELEASE(x)			if(x){(x)->Release();	(x) = NULL;}

#ifdef __MINGW64__
#define lstat	stat
#endif

typedef enum {
	LOG_MODE_INFO,
	LOG_MODE_WARNING,
	LOG_MODE_ERROR
} LOG_MODE;

bool isVarChar(char ch);
void LOG(LOG_MODE id, const char* sFormat, ...);
#define LOGI(...)		LOG(LOG_MODE_INFO, __VA_ARGS__)
#define LOGW(...)		LOG(LOG_MODE_WARNING, __VA_ARGS__)
#define LOGE(...)		LOG(LOG_MODE_ERROR, __VA_ARGS__)
extern int	g_log_warning_count;
extern int	g_log_error_count;

class Reference {
public:
	Reference(void);
	virtual ~Reference(void);

	void AddRef(void);
	void Release(void);

private:
	unsigned int	m_dwReferCount;
};

template <class T>
class Object : public Reference {
public:
	Object(void) {
		m_pRTTI = &m_RTTI;
	}
	virtual ~Object(void) {}

	bool IsValidObject(void)			{
		return m_pRTTI == RTTI();
	}
	static const void* RTTI(void)		{
		return &m_RTTI;
	}

private:
	static int		m_RTTI;
	void*			m_pRTTI;
};

template <class T>
int Object<T>::m_RTTI	= NULL;

template<typename T>
class NamedMap {
public:
	NamedMap(void) {}
	virtual ~NamedMap(void) {
		ReleaseAll();
	}

	void ReleaseAll(void) {
		for(auto i = m_List.begin(); i != m_List.end(); i++) {
			SAFE_RELEASE(i->second);
		}

		m_List.clear();
	}

	T* Find(const char* sName) {
		auto i = m_List.find(sName);

		if(i == m_List.end()) return NULL;

		return i->second;
	}

	T* operator[](const char* sName) {
		return Find(sName);
	}

	T* Create(const char* sName) {
		T* pNode	= Find(sName);

		if(!pNode) {
			pNode			= new T;
			m_List[sName]	= pNode;
		}

		return pNode;
	}

	void Set(const char* sName, T* pNode) {
		auto i = m_List.find(sName);

		if(pNode) pNode->AddRef();

		if(i != m_List.end()) {
			SAFE_RELEASE(i->second);
			i->second		= pNode;
		} else {
			m_List[sName]	= pNode;
		}
	}

	inline map<string, T*>& List(void)		{
		return m_List;
	}

private:
	map<string, T*>		m_List;
};

template<typename T>
class AutoList {
public:
	AutoList(void) {
		m_pNext	= m_pHead;
		m_pHead	= (T*)this;
	}
	virtual ~AutoList(void) {
		if(m_pHead == (T*)this) {
			m_pHead	= m_pNext;
		} else {
			T*	pNode	= m_pHead;

			while(pNode) {
				if(pNode->m_pNext == (T*)this) {
					pNode->m_pNext	= m_pNext;
					break;
				}

				pNode	= pNode->m_pNext;
			}
		}
	}
	static void ReleaseAll(void) {
		while(Head()) delete Head();
	}
	inline void Release(void)	{
		delete this;
	}
	inline static T* Head(void)	{
		return m_pHead;
	}
	inline T* Next(void)			{
		return m_pNext;
	}

protected:
	static T*		m_pHead;
	T*				m_pNext;
};

template<typename T>
T* AutoList<T>::m_pHead	= NULL;

template<typename T>
class ClassType {
public:
	ClassType(void) {
		m_pClassID	= &m_ClassID;
	}
	virtual ~ClassType(void) {}

	inline void* ClassID(void)			{
		return &m_ClassID;
	}
	inline bool IsValidClass(void)		{
		return (m_pClassID == &m_ClassID);
	}

private:
	void*				m_pClassID;
	static int			m_ClassID;
};

template<typename T>
int	ClassType<T>::m_ClassID		= 0;

string GetCommonToolPath(void);

#endif// __COMMON_H__
