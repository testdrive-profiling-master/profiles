//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 10/20/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __ARG_TABLE_H__
#define __ARG_TABLE_H__
#include "UtilFramework.h"

typedef enum {
	ARG_TYPE_NONE,
	ARG_TYPE_OPT,
	ARG_TYPE_INT,
	ARG_TYPE_DOUBLE,
	ARG_TYPE_STRING,
	ARG_TYPE_FILE,
} ARG_TYPE;

typedef struct {
	ARG_TYPE	type;
	void*		pArg;
} ARG_DATA;

class ArgTable {
public:
	ArgTable(const char* sDescription);
	virtual ~ArgTable(void);

	void Release(void);
	bool DoParse(int argc, const char** argv);
	bool DoParseDefault(void);

	static void SetDefaultArguments(int argc, const char** argv);
	static int ArgumentSize(void);
	static const char* GetArgument(int iIndex);

	void AddRemark(const char* sDataType, const char* sGlossary);
	void AddOption(const char* sName, const char* sOptShort, const char* sOptLong, const char* sGlossary);
	void AddOptionInt(const char* sName, int iDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary);
	void AddOptionDouble(const char* sName, int fDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary);
	void AddOptionString(const char* sName, const char* sDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary);
	void AddOptionFile(const char* sName, const char* sDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary);
	void AddOptionFiles(const char* sName, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary);

	bool GetOption(const char* sName);
	int GetOptionInt(const char* sName);
	double GetOptionDouble(const char* sName);
	const char* GetOptionString(const char* sName);
	const char* GetOptionFile(const char* sName, int iIndex = 0);

	void ShowHelp(void);

	void SetExecuteFileName(const char* sName);
	void SetDescription(const char* sDescription);
	void SetExtraArguments(const char* sExtraArg);
	void SetVersion(const char* sVersion);

private:
	cstring					m_sExecuteFile;
	cstring					m_sDescription;
	cstring					m_sExtraArgs;
	cstring					m_sVersion;
	vector<void*>			m_ArgList;
	map<string, ARG_DATA>	m_MapList;
	struct {
		map<string, string>		opt_file;
		map<string, string>		opt_string;
	} m_DefaultList;
};

#endif//__ARG_TABLE_H__
