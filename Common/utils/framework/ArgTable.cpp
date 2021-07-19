//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 7/19/2021 Mon (clonextop@gmail.com)
//================================================================================
#include "ArgTable.h"
#include "argtable3/src/argtable3.h"

ArgTable::ArgTable(const char* sDescription)
{
	SetDescription(sDescription);
	AddOption("help", NULL, "help", "display this help and exit");
}

ArgTable::~ArgTable(void)
{
	Release();
}

void ArgTable::Release(void)
{
	if(m_ArgList.size()) {
		arg_freetable(&m_ArgList.front(), m_ArgList.size());
		m_ArgList.clear();
		m_MapList.clear();
	}
}

void ArgTable::SetExecuteFileName(const char* sName)
{
	m_sExecuteFile		= sName;
	m_sExecuteFile.CutFront("\\", true);
	m_sExecuteFile.CutFront("/", true);
}

void ArgTable::SetDescription(const char* sDescription)
{
	m_sDescription		= sDescription;
}

void ArgTable::SetExtraArguments(const char* sExtraArg)
{
	m_sExtraArgs		= sExtraArg;
}

void ArgTable::SetVersion(const char* sVersion)
{
	m_sVersion			= sVersion;
	AddOption("version", NULL, "version", "print version information and exit");
}

bool ArgTable::DoParse(int argc, const char** argv)
{
	if(!m_sExecuteFile.size()) {
		SetExecuteFileName(argv[0]);
	}

	if(m_ArgList.size() > 0) {
		m_ArgList.push_back(arg_end(20));

		if(arg_parse(argc, (char**)argv, &m_ArgList.front()) > 0) {
			if(argc == 1 || GetOption("help")) {
				goto SHOW_HELP;
			}

			arg_print_errors(stdout, (struct arg_end*)(m_ArgList[m_ArgList.size() - 1]), m_sExecuteFile.c_str());
			printf("Try '%s --help' for more information.\n", m_sExecuteFile.c_str());
			return false;
		}

		if(argc == 1 || GetOption("help")) {	// --help option
		SHOW_HELP:
			ShowHelp();
			return false;
		}

		return true;
	}

	return false;
}

static int			__default_argc	= 0;
static const char**	__default_argv;

bool ArgTable::DoParseDefault(void)
{
	return DoParse(__default_argc, __default_argv);
}

void ArgTable::SetDefaultArguments(int argc, const char** argv)
{
	__default_argc	= argc;
	__default_argv	= argv;
}

int ArgTable::ArgumentSize(void)
{
	return (__default_argc - 1);
}

const char* ArgTable::GetArgument(int iIndex)
{
	if(iIndex >= 0 &&  iIndex < (__default_argc - 1))
		return __default_argv[iIndex + 1];

	return NULL;
}

void ArgTable::AddRemark(const char* sDataType, const char* sGlossary)
{
	m_ArgList.push_back(arg_rem(sDataType, sGlossary));
}

void ArgTable::AddOption(const char* sName, const char* sOptShort, const char* sOptLong, const char* sGlossary)
{
	struct arg_lit* pArg = arg_lit0(sOptShort, sOptLong, sGlossary);
	m_MapList[sName]	= ARG_DATA{ARG_TYPE_OPT, pArg};
	m_ArgList.push_back(pArg);
}

void ArgTable::AddOptionInt(const char* sName, int iDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary)
{
	struct arg_int* pArg = arg_int0(sOptShort, sOptLong, sDataType, sGlossary);
	pArg->ival[0]		= iDefaultValue;
	pArg->hdr.maxcount	= 1;
	m_MapList[sName]	= ARG_DATA{ARG_TYPE_INT, pArg};
	m_ArgList.push_back(pArg);
}

void ArgTable::AddOptionDouble(const char* sName, int fDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary)
{
	struct arg_dbl* pArg = arg_dbl0(sOptShort, sOptLong, sDataType, sGlossary);
	pArg->dval[0]		= fDefaultValue;
	pArg->hdr.maxcount	= 1;
	m_MapList[sName]	= ARG_DATA{ARG_TYPE_DOUBLE, pArg};
	m_ArgList.push_back(pArg);
}

void ArgTable::AddOptionString(const char* sName, const char* sDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary)
{
	bool	bOption	= (sOptShort != NULL) || (sOptLong != NULL) || (sDefaultValue != NULL);
	struct arg_str* pArg = bOption ? arg_str0(sOptShort, sOptLong, sDataType, sGlossary) : arg_str1(sOptShort, sOptLong, sDataType, sGlossary);

	if(sDefaultValue) {
		static list<string>	__cache;
		__cache.push_back(sDefaultValue);
		pArg->sval[0]		= (__cache.back()).c_str();
	} else {
		pArg->sval[0]		= NULL;
	}

	pArg->hdr.maxcount	= 1;
	m_MapList[sName]	= ARG_DATA{ARG_TYPE_STRING, pArg};
	m_ArgList.push_back(pArg);
}

void ArgTable::AddOptionFile(const char* sName, const char* sDefaultValue, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary)
{
	bool	bOption	= (sOptShort != NULL) || (sOptLong != NULL) || (sDefaultValue != NULL);
	struct arg_file* pArg = bOption ? arg_file0(sOptShort, sOptLong, sDataType, sGlossary) : arg_file1(sOptShort, sOptLong, sDataType, sGlossary);

	if(sDefaultValue) {
		static list<string>	__cache;
		__cache.push_back(sDefaultValue);
		pArg->filename[0]		= (__cache.back()).c_str();
	} else {
		pArg->filename[0]		= NULL;
	}

	pArg->hdr.maxcount	= 1;
	m_MapList[sName]	= ARG_DATA{ARG_TYPE_FILE, pArg};
	m_ArgList.push_back(pArg);
}

void ArgTable::AddOptionFiles(const char* sName, const char* sOptShort, const char* sOptLong, const char* sDataType, const char* sGlossary)
{
	bool	bOption	= (sOptShort != NULL) || (sOptLong != NULL);
	struct arg_file* pArg = arg_filen(sOptShort, sOptLong, sDataType, bOption ? 0 : 1, ArgumentSize(), sGlossary);
	m_MapList[sName]	= ARG_DATA{ARG_TYPE_FILE, pArg};
	m_ArgList.push_back(pArg);
}

void ArgTable::ShowHelp(void)
{
	if(m_sDescription.size()) {
		printf("\n%s\n", m_sDescription.c_str());
	}

	printf("Usage: %s ", m_sExecuteFile.c_str());
	arg_print_syntaxv(stdout, &m_ArgList.front(), NULL);
	printf(" %s\n\n", m_sExtraArgs.c_str());
	arg_print_glossary_gnu(stdout, &m_ArgList.front());
}

bool ArgTable::GetOption(const char* sName)
{
	ARG_DATA	arg_data	= m_MapList[sName];

	if(arg_data.type == ARG_TYPE_OPT) {
		struct arg_lit* pArg = (struct arg_lit*)arg_data.pArg;
		return (pArg->count > 0);
	}

	return false;
}

int ArgTable::GetOptionInt(const char* sName)
{
	ARG_DATA	arg_data	= m_MapList[sName];

	if(arg_data.type == ARG_TYPE_INT) {
		return ((struct arg_int*)arg_data.pArg)->ival[0];
	}

	return -1;
}

double ArgTable::GetOptionDouble(const char* sName)
{
	ARG_DATA	arg_data	= m_MapList[sName];

	if(arg_data.type == ARG_TYPE_DOUBLE) {
		return ((struct arg_dbl*)arg_data.pArg)->dval[0];
	}

	return (double) -1;
}

const char* ArgTable::GetOptionString(const char* sName)
{
	ARG_DATA	arg_data	= m_MapList[sName];

	if(arg_data.type == ARG_TYPE_STRING) {
		return ((struct arg_str*)arg_data.pArg)->sval[0];
	}

	return NULL;
}


const char* ArgTable::GetOptionFile(const char* sName, int iIndex)
{
	ARG_DATA	arg_data	= m_MapList[sName];

	if(arg_data.type == ARG_TYPE_FILE) {
		struct arg_file* pArg = (struct arg_file*)arg_data.pArg;

		if(iIndex < pArg->count) {
			return pArg->filename[iIndex];
		}
	}

	return NULL;
}
