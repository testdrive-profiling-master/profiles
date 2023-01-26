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
// Title : TestDrive codegen project
// Rev.  : 1/26/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __CODE_BASE_H__
#define __CODE_BASE_H__
#include "Common.h"
#include "TextFile.h"

typedef enum {
	CLASS_STRING,
	CLASS_NUMBER,
} CLASS;

//----------------------------------------------------------------------------------
// VL primitives
//----------------------------------------------------------------------------------
class VL;
class VL_String;

typedef bool (*ENUMERATE_VL_FUNCTION)(VL* pChild, void* pPrivate);

class VL : public AutoList<VL> {
public:
	VL(CLASS Class);
	virtual ~VL(void);

	string Data(void);
	virtual string PrivateData(void)			{
		return "";
	}

	void Clear(void);
	VL* AddChild(VL* pCode);
	VL* AddChildFront(VL* pCode);
	VL* PopChild(void);
	bool RemoveChild(VL* pCode);
	VL* FindChild(const char* sName);
	void ShowAll(void);	// for debug only
	void EnumerateChild(ENUMERATE_VL_FUNCTION func, void* pPrivate = NULL);
	inline int ChildSize(void)					{
		return m_ChildList.size();
	}
	string& Name(void)							{
		return m_sName;
	}
	const char* GetName(void) const				{
		return m_sName.c_str();
	}
	void SetName(const char* sName)				{
		m_sName = sName;
	}
	virtual const char* GetData(void) const = 0;
	virtual void SetData(const char* sData)	= 0;
	inline CLASS Class(void)					{
		return m_Class;
	}

protected:
	string			m_sName;
	list<VL*>		m_ChildList;

private:
	CLASS			m_Class;
};

//----------------------------------------------------------------------------------
// Code
//----------------------------------------------------------------------------------
class VL_String : public VL {
public:
	VL_String(const char* sName, const char* sData = NULL);
	virtual ~VL_String(void);

	static VL_String* Alias(VL* pNode);
	static VL_String* Create(const char* sName, const char* sData);
	static VL_String* CreateFromTable(const char* sFileName, const char* sChildID);
	static VL_String* CreateFromXLSX(const char* sFileName, const char* sSheetName, const char* sChildID);
	static bool CreateToXLSX(const char* sFileName, VL* pVL);
	static VL_String* CreateFileList(const char* sPath, int iDepth, bool bOnlyFile = false, const char* sExtList = NULL);
	virtual string PrivateData(void);

	virtual const char* GetData(void) const	{
		return m_sData.c_str();
	}
	virtual void SetData(const char* sData) {
		m_sData	= sData;
	}

	string 		m_sData;
};

//----------------------------------------------------------------------------------
// Integer
//----------------------------------------------------------------------------------
class VL_Number : public VL {
public:
	VL_Number(const char* sName, double fData = 0);
	virtual ~VL_Number(void);

	static VL_Number* Alias(VL* pNode);
	static VL_Number* Create(const char* sName, double fData);
	virtual string PrivateData(void);

	void SetFormat(const char* sFormat);

	virtual const char* GetData(void) const	{
		return NULL;
	}
	virtual double GetNumber(void)			{
		return m_fData;
	}
	virtual void SetData(const char* sData) {
		m_fData	= atof(sData);
	}

	string		m_sFormat;
	double		m_fData;
};

#endif//__CODE_BASE_H__
