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
// Title : make testbench from simulation waveform
// Rev.  : 4/4/2023 Tue (clonextop@gmail.com)
//================================================================================
#include "vTestbench.h"

string						g_Timescale;
list<vTestbenchVar*>		g_TestbenchVarList;
string						g_sModuleName		= "unnamed_module";
string						g_InstancePath;
string						g_TemplatePath;
string						g_sOutputFileName;
map<string, int>			g_ForInteratorList;
static bool					__bParameterExist	= false;

#define	INT_LIMIT			2147483647

vTestbenchVar::vTestbenchVar(lt_symbol* pSymbol)
{
	m_pSymbol			= pSymbol;
	m_PrevTime			= 0;
	m_VarType			= TB_VAR_TYPE_NONE;
	g_TestbenchVarList.push_back(this);

	if(pSymbol->vartype == V_PARAMETER) __bParameterExist = true;

	{
		m_sName	= pSymbol->name;
		int iPos = m_sName.find('[');

		if(iPos > 0) m_sName.erase(iPos, -1);

		iPos = m_sName.rfind('.');

		if(iPos > 0) m_sName.erase(0, iPos + 1);
	}

	while(pSymbol->aliased_to) pSymbol = pSymbol->aliased_to;

	m_pNext				= pSymbol->pTestbench;
	pSymbol->pTestbench	= this;
	{
		string sFileName(".vTestbench.");
		sFileName	+= m_sName;
		sFileName	+= ".inl";

		if(!m_File.Create(sFileName.c_str())) {
			printf("Can't create partial testbench file : %s\n", sFileName.c_str());
			exit(1);
		}
	}
}

vTestbenchVar* vTestbenchVar::Find(const char* sName)
{
	if(sName)
		for(list<vTestbenchVar*>::iterator i = g_TestbenchVarList.begin(); i != g_TestbenchVarList.end(); i++) {
			if((*i)->Name() == sName)
				return *i;
		}

	return NULL;
}

void vTestbenchVar::Push(lxttime_t timeval, const char* sValue)
{
	if(VarType() == TB_VAR_TYPE_NONE) return;

	if(m_PrevTime != timeval) {
		lxttime_t	rest_time	= timeval - m_PrevTime;

		while(rest_time > INT_LIMIT) {
			m_File.Write("#%d /* to prevent 32bit overflow */;\n", INT_LIMIT);
			rest_time	-= INT_LIMIT;
		}

		if(rest_time) m_File.Write("#"TTFormat" ", rest_time);

		m_PrevTime	= timeval;
	}

	{
		char sTag[128];
		sprintf(sTag, "%d'b", m_pSymbol->len);
		m_sValue	= sTag;
		m_sValue	+= sValue;
	}

	m_File.Write("%s = %s;\n", m_sName.c_str(), m_sValue.c_str());
}

void vTestbenchVar::PushClock(lxttime_t timeval, lxttime_t delta, int iTimes, bool bMulti)
{
	cstring sIter;
	g_ForInteratorList[m_sName]	= 0;
	m_File.Write("for(__i_%s=0;__i_%s<%d;__i_%s=__i_%s+1) begin\n\t#"TTFormat" ", m_sName.c_str(), m_sName.c_str(), iTimes, m_sName.c_str(), m_sName.c_str(), delta);

	if(bMulti)
		m_File.Write("%s = %s + 1'b1;\nend\n", m_sName.c_str(), m_sName.c_str());
	else
		m_File.Write("%s = ~%s;\nend\n", m_sName.c_str(), m_sName.c_str());

	{
		// expected timing
		lxttime_t	rest_time = timeval - (m_PrevTime + (delta * iTimes));

		while(rest_time > INT_LIMIT) {
			m_File.Write("#%d /* to prevent 32bit overflow */;\n", INT_LIMIT);
			rest_time	-= INT_LIMIT;
		}

		if(rest_time) m_File.Write("#"TTFormat ";\n", rest_time);
	}
	m_PrevTime	= timeval;
}

void vTestbenchVar::Apply(cstring& sOut)
{
	switch(VarType()) {
	case TB_VAR_TYPE_PARAMETER:
		sOut.AppendFormat("localparam %s = %s;\n", Name().c_str(), LastValue().c_str());
		return;

	case TB_VAR_TYPE_IN:
		break;

	case TB_VAR_TYPE_OUT:
	case TB_VAR_TYPE_INOUT:
		sOut.AppendFormat("wire\t");

		if(m_pSymbol->len > 1) {
			sOut.AppendFormat("[%d:%d]", m_pSymbol->msb, m_pSymbol->lsb);
		}

		sOut.AppendFormat("\t%s;\n", m_sName.c_str());
		return;

	default:
		return;
	}

	if(m_File.IsOpen()) {
		string sFileName	= m_File.FileName();
		m_File.Close();
		sOut.AppendFormat("reg\t");

		if(m_pSymbol->len > 1) {
			sOut.AppendFormat("[%d:%d]", m_pSymbol->msb, m_pSymbol->lsb);
		}

		if(g_ForInteratorList.count(m_sName.c_str())) {
			g_ForInteratorList[m_sName.c_str()]	= 1;
		}

		sOut.AppendFormat("\t%s;\ninitial begin\t// %s\n", m_sName.c_str(), m_sName.c_str());

		if(m_File.Open(sFileName.c_str())) {
			cstring sLine;

			while(m_File.GetLine(sLine)) {
				sOut.AppendFormat("\t%s", sLine.c_str());
			}

			m_File.Close();
		} else {
			LOGE("Can't open file : %s", sFileName.c_str());
			exit(1);
		}

		sOut.AppendFormat("end\n\n");
		remove(sFileName.c_str());
	}
}

vTestbenchVar::~vTestbenchVar(void)
{
	if(m_File.IsOpen()) {
		string sFileName	= m_File.FileName();
		m_File.Close();
		remove(sFileName.c_str());
	}
}

void AddTestbenchSignal(lt_symbol* pSymbol)
{
	string sName	= pSymbol->name;

	switch(pSymbol->vartype) {
	case V_PARAMETER:
	case V_WIRE:
	case V_REG:
	case V_PORT:
	case V_IN:
	case V_OUT:
	case V_INOUT:
	case V_SUPPLY0:
	case V_SUPPLY1:
		break;

	default:
		return;
	}

	if(strstr(sName.c_str(), g_InstancePath.c_str()) == sName.c_str() && !strstr(sName.c_str() + strlen(g_InstancePath.c_str()) + 1, ".")) {
		new vTestbenchVar(pSymbol);
	}
}

bool vTestbenchVar::Initialize(const char* sFileName)
{
	g_ForInteratorList.clear();
	g_sOutputFileName	= sFileName;
	TextFile f;
	bool	bRet	= f.Create(sFileName);	// check for writable
	return bRet;
}

bool vTestbenchVar::Finalize(void)
{
	bool	bNoPort	= true;
	cstring	sOut;
	sOut.AppendFormat("%s\n\n", g_Timescale.c_str());
	sOut.AppendFormat("module Testbench_top ();$(for_iterator)\n\n");

	// release all objects
	for(auto& i : g_TestbenchVarList) {
		vTestbenchVar* pVar = i;
		i->Apply(sOut);

		if(i->m_VarType != TB_VAR_TYPE_NONE) {
			bNoPort	= false;
		}
	}

	{
		// simulation life cycle
		extern TimeType max_time;
		lxttime_t	rest_time = max_time;
		sOut.AppendFormat("//simulation life cycle.\ninitial begin\n");

		while(rest_time > INT_LIMIT) {
			sOut.AppendFormat("\t#%d /* to prevent 32bit overflow */;\n", INT_LIMIT);
			rest_time	-= INT_LIMIT;
		}

		sOut.AppendFormat("\t");

		if(rest_time) sOut.AppendFormat("#"TTFormat" ", rest_time);

		sOut.AppendFormat("$finish;\nend\n\n");
	}

	{
		// print out module instance
		{
			// module instance
			string inst_name(g_InstancePath);
			int iPos = inst_name.rfind('.');

			if(iPos >= 0) inst_name.erase(0, iPos + 1);

			sOut.AppendFormat("%s %s ", g_sModuleName.c_str(), inst_name.c_str());
		}
		{
			// parameters
			cstring s;
			int iItemCount = 0;

			for(auto& i : g_TestbenchVarList) {
				if(i->VarType() == TB_VAR_TYPE_PARAMETER) {
					if(iItemCount) s.Append(",");

					s.AppendFormat("\n\t.%-24s(%-24s)", i->Name().c_str(), i->Name().c_str());
					iItemCount++;
				}
			}

			if(iItemCount)
				sOut.AppendFormat("#(%s\n) ", s.c_str());
		}
		{
			// ports
			int iItemCount = 0;
			sOut.AppendFormat("(");

			// signals
			for(auto& i : g_TestbenchVarList) {
				if(bNoPort) {
					if(iItemCount)sOut.AppendFormat(",");

					sOut.AppendFormat("\n\t.%-24s(                        )", i->Name().c_str());
					iItemCount++;
				} else {
					if(i->VarType() != TB_VAR_TYPE_PARAMETER && i->VarType() != TB_VAR_TYPE_NONE) {
						if(iItemCount)sOut.AppendFormat(",");

						sOut.AppendFormat("\n\t.%-24s(%-24s)", i->Name().c_str(), i->Name().c_str());
						iItemCount++;
					}
				}
			}

			sOut.AppendFormat("\n);\n");
		}
	}

	sOut.AppendFormat("\nendmodule\n");
	{
		// release all objects
		for(auto& i : g_TestbenchVarList) {
			delete i;
		}

		g_TestbenchVarList.clear();
	}

	if(g_ForInteratorList.size()) {	// setup for 'i' iterators
		int iCount = 0;
		cstring sIter("\nint ");

		for(auto& i : g_ForInteratorList) {
			if(i.second) {
				if(iCount) sIter.Append(", ");

				sIter.AppendFormat("__i_%s", i.first.c_str());
				iCount++;
			}
		}

		if(iCount) {
			sIter.Append(";");
			sOut.Replace("$(for_iterator)", sIter);
		} else goto NO_FOR_ITERATOR;
	} else {
	NO_FOR_ITERATOR:
		sOut.Replace("$(for_iterator)", "");
	}

	{
		// save to file
		TextFile f;
		f.Create(g_sOutputFileName.c_str());
		f.Puts(sOut);
	}

	return true;
}

void __FirstCheck(void)
{
	static bool __bFirst	= true;

	if(__bFirst) {
		if(!g_TestbenchVarList.size()) {
			LOGE("There is not a instance path : '%s'", g_InstancePath.c_str());
			exit(1);
		}

		{
			TextFile TemplateFile;
			cstring sLine;

			if(!TemplateFile.Open(g_TemplatePath.c_str())) {
				LOGE("Can't open template file : %s", g_TemplatePath.c_str());
				exit(1);
			}

			while(TemplateFile.GetLine(sLine)) {
				const char* sDelim	= " \t\r\n;,";
				char sTemp[8192];
				char* sTok;
				strcpy(sTemp, sLine.c_str());
				sTok	= strstr(sTemp, "//");

				if(sTok) *sTok = NULL;

				sTok	= strtok(sTemp, sDelim);

				if(sTok) {
					if(!strcmp(sTok, "module")) {
						g_sModuleName	= strtok(NULL, sDelim);
					} else if(!strcmp(sTok, "parameter")) {
						sTok	= strtok(NULL, sDelim);
						vTestbenchVar* pVar = vTestbenchVar::Find(sTok);

						if(!pVar) {
							printf("Can't find parameter '%s'\n", sTok);
							exit(1);
						}

						pVar->SetVarType(TB_VAR_TYPE_PARAMETER);
					} else {
						TB_VAR_TYPE	var_type = TB_VAR_TYPE_NONE;

						if(!strcmp(sTok, "input")) {
							var_type	= TB_VAR_TYPE_IN;
						} else if(!strcmp(sTok, "output")) {
							var_type	= TB_VAR_TYPE_OUT;
						} else if(!strcmp(sTok, "inout")) {
							var_type	= TB_VAR_TYPE_INOUT;
						}

						if(var_type != TB_VAR_TYPE_NONE) {
							{
								// copy from original
								string sMod;
								sMod = strtok(NULL, "");
								int iPos = sMod.rfind(']');

								if(iPos >= 0)sMod.erase(0, iPos + 1);

								strcpy(sTemp, sMod.c_str());
								sTok	= strtok(sTemp, sDelim);
							}

							while(sTok) {
								if(strcmp(sTok, "wire") && strcmp(sTok, "reg")) {
									vTestbenchVar* pVar = vTestbenchVar::Find(sTok);

									if(!pVar) {
										LOGE("Can't find input port '%s'", sTok);
										exit(1);
									}

									pVar->SetVarType(var_type);
								}

								sTok	= strtok(NULL, sDelim);
							}
						}
					}
				}
			}

			TemplateFile.Close();
		}

		__bFirst	= false;
	}
}

void Testbench_PushValue(lt_symbol* pSymbol, lxttime_t timeval, const char* sValue)
{
	__FirstCheck();

	while(pSymbol->aliased_to) pSymbol = pSymbol->aliased_to;

	if(pSymbol->pTestbench) {
		vTestbenchVar*	pVar	= pSymbol->pTestbench;

		while(pVar) {
			pVar->Push(timeval, sValue);
			pVar = pVar->Next();
		}
	}
}

void Testbench_PushClock(lt_symbol* pSymbol, lxttime_t timeval, lxttime_t clk_delta, int clk_numtrans, bool bMulti)
{
	__FirstCheck();

	while(pSymbol->aliased_to) pSymbol = pSymbol->aliased_to;

	if(pSymbol->pTestbench) {
		vTestbenchVar*	pVar	= pSymbol->pTestbench;

		while(pVar) {
			pVar->PushClock(timeval, clk_delta, clk_numtrans, bMulti);
			pVar = pVar->Next();
		}
	}
}
