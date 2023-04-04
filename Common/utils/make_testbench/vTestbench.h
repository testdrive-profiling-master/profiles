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
#ifndef _V_TESTBENCH_H__
#define _V_TESTBENCH_H__
#include "UtilFramework.h"
#include "v2l_debug.h"
#include "lxt_write.h"

typedef enum {
	TB_VAR_TYPE_NONE,
	TB_VAR_TYPE_PARAMETER,
	TB_VAR_TYPE_IN,
	TB_VAR_TYPE_OUT,
	TB_VAR_TYPE_INOUT,
} TB_VAR_TYPE;

class vTestbenchVar {
public:
	vTestbenchVar(lt_symbol* pSymbol);
	virtual ~vTestbenchVar(void);

	static bool Initialize(const char* sFileName);
	static bool Finalize(void);
	static vTestbenchVar* Find(const char* sName);
	void Push(lxttime_t timeval, const char* sValue);
	void PushClock(lxttime_t timeval, lxttime_t delta, int iTimes, bool bMulti = false);
	void Apply(cstring& sOut);

	inline vTestbenchVar* Next(void)		{
		return m_pNext;
	}
	inline lt_symbol* Symbol(void)			{
		return m_pSymbol;
	}
	inline string& Name(void)				{
		return m_sName;
	}
	inline string& LastValue(void)			{
		return m_sValue;
	}
	inline void SetVarType(TB_VAR_TYPE var_type)	{
		m_VarType = var_type;
	}
	inline TB_VAR_TYPE VarType(void)		{
		return m_VarType;
	}

protected:
	vTestbenchVar*			m_pNext;
	struct lt_symbol*		m_pSymbol;
	string					m_sName;
	TextFile				m_File;
	lxttime_t				m_PrevTime;
	TB_VAR_TYPE				m_VarType;
	string					m_sValue;
};

extern string					g_Timescale;
extern list<vTestbenchVar*>		g_TestbenchVarList;
extern string					g_sModuleName;
extern string					g_InstancePath;
extern string					g_TemplatePath;

void AddTestbenchSignal(lt_symbol* pSymbol);
void Testbench_PushValue(lt_symbol* pSymbol, lxttime_t timeval, const char* sValue);
void Testbench_PushClock(lt_symbol* pSymbol, lxttime_t timeval, lxttime_t clk_delta, int clk_numtrans, bool bMulti = false);

#endif//_V_TESTBENCH_H__
