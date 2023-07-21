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
// Title : Testbench
// Rev.  : 7/21/2023 Fri (clonextop@gmail.com)
//================================================================================
#include "vSim.h"
#include "TestDriver.inl"
#include <chrono>
#include <time.h>		// for rand/srand

static std::mutex		__fp32_test_mutex;

const SIM_VALUE	g_FP_test_value_list[] = {
	{.f = 1.f},		// one
	{.u = 0x3F800001},	// one + alhpa(1)
	{.u = 0x3F800002},	// one + alhpa(2)
	{.u = 0x3F800003},	// one + alhpa(3)
	{.f = 0.1f},
	{.f = 0.25f},
	{.f = 0.000139f},
	// zero
	{.u = 0x00000000},
	// denormal numbers
	{.u = 0x00000001},	// min
	{.u = 0x00000002},
	{.u = 0x00000003},
	{.u = 0x00000131},
	{.u = 0x00200000},	// middle
	{.u = 0x007FFFFD},
	{.u = 0x007FFFFE},
	{.u = 0x007FFFFF},	// max
	// infinity
	{.u = 0x7F800000}
};

bool RetrieveFP32_TestParam(uint32_t& A)
{
	static bool bFirst	= true;
	static int	iIndex	= 0;
	static int	iMax	= 0;

	if(bFirst) {
		iIndex	= 0;
		iMax	= (sizeof(g_FP_test_value_list) / sizeof(SIM_VALUE)) * 2;
		bFirst	= false;
	}

	if(iIndex >= iMax) return false;

	if(iIndex < (iMax / 2)) {
		A	= g_FP_test_value_list[iIndex].u;
	} else {
		A	= -g_FP_test_value_list[iIndex - (iMax / 2)].u;
	}

	iIndex++;
	return true;
}

bool RetrieveFP32_TestParam(uint32_t& A, uint32_t& B)
{
	static bool bFirst		= true;
	static int	iIndex_A	= 0;
	static int	iIndex_B	= 0;
	static int	iMax		= 0;

	if(bFirst) {
		iIndex_A	= 0;
		iIndex_B	= 0;
		iMax		= (sizeof(g_FP_test_value_list) / sizeof(SIM_VALUE)) * 2;
		bFirst		= false;
	}

	if(iIndex_B >= iMax) return false;

	if(iIndex_A < (iMax / 2)) {
		A	= g_FP_test_value_list[iIndex_A].u;
	} else {
		A	= -g_FP_test_value_list[iIndex_A - (iMax / 2)].u;
	}

	if(iIndex_B < (iMax / 2)) {
		B	= g_FP_test_value_list[iIndex_B].u;
	} else {
		B	= -g_FP_test_value_list[iIndex_B - (iMax / 2)].u;
	}

	iIndex_A++;

	if(iIndex_A >= iMax) {
		iIndex_A	= 0;
		iIndex_B++;
	}

	return true;
}

volatile uint64_t	g_vSim_Count	= 0;
volatile bool		g_vSim_End		= false;

bool RetrieveFP32_Param(uint32_t& A)
{
	static union {
		volatile uint32_t	m;
		volatile SIM_VALUE	v;
	} p = {
		.m = 0
	};
	{
		std::lock_guard<std::mutex> guard(__fp32_test_mutex);

		if(g_vSim_End) return false;

		g_vSim_Count	= ((uint64_t)p.m) << 32;
		auto		end_time	= std::chrono::high_resolution_clock::now();
		A		= p.v.u;

		if(p.m == 0xFFFFFFFF) g_vSim_End = true;

		// bypassing nan
		if(!isnormal(p.v.f) && p.v.fp.mantissa < 0x7EFFFF) {
			p.v.u	+= (1 << 16);
		} else {
			p.m++;
		}
	}
	return true;
}

bool RetrieveFP32_Param(uint32_t& A, uint32_t& B)
{
	static union {
		volatile uint64_t	m;
		volatile SIM_VALUE	v[2];
	} p = {
		.m = 0
	};
	{
		std::lock_guard<std::mutex> guard(__fp32_test_mutex);

		if(g_vSim_End) return false;

		g_vSim_Count	= p.m;
		A		= p.v[0].u;
		B		= p.v[1].u;

		if(p.m == 0xFFFFFFFF'FFFFFFFFULL) g_vSim_End = true;

		// bypassing nan
		if(!isnormal(p.v[1].f) && p.v[1].fp.mantissa < 0x7EFFFF) {	// for fast overcome
			p.v[1].u += 1 << 16;
		} else if(!isnormal(p.v[0].f) && p.v[0].fp.mantissa < 0x7EFFFF) {
			p.v[0].u += 1 << 16;
		} else {
			p.m++;
		}
	}
	return true;
}

bool CheckFP32_Result(FP32_GOLDEN_1 golden_func, uint32_t A, uint32_t O)
{
	float	golden	= golden_func(*(float*)&A);
	float	result	= *(float*)&O;

	if(isnan(result) && isnan(golden)) return true;

	if(isinf(result) && isinf(golden) && !(((*(uint32_t*)&golden) ^ O) & 0x80000000)) return true;

	if(O != *(uint32_t*)&golden) {
		std::lock_guard<std::mutex> guard(__fp32_test_mutex);
		SIM_VALUE	As, Os, Gs;
		As.u = A;
		Os.u = O;
		Gs.u = *(uint32_t*)&golden;
		printf("\n*E: Result not matched with golden data\n");
		printf("    A      = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f\n",
			   As.u, As.fp.sign, As.fp.exponent, As.fp.mantissa, As.f);
		printf("    RESULT = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f <- NOT MATCHED!\n",
			   Os.u, Os.fp.sign, Os.fp.exponent, Os.fp.mantissa, Os.f);
		printf("    GOLDEN = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f\n",
			   Gs.u, Gs.fp.sign, Gs.fp.exponent, Gs.fp.mantissa, Gs.f);
		fflush(stdout);
		return false;
	}

	return true;
}

bool CheckFP32_Result(FP32_GOLDEN_2 golden_func, uint32_t A, uint32_t B, uint32_t O)
{
	float	golden	= golden_func(*(float*)&A, *(float*)&B);
	float	result	= *(float*)&O;

	if(isnan(result) && isnan(golden)) return true;

	if(isinf(result) && isinf(golden) && !(((*(uint32_t*)&golden) ^ O) & 0x80000000)) return true;

	if(O != *(uint32_t*)&golden) {
		std::lock_guard<std::mutex> guard(__fp32_test_mutex);
		SIM_VALUE	As, Bs, Os, Gs;
		As.u = A;
		Bs.u = B;
		Os.u = O;
		Gs.u = *(uint32_t*)&golden;
		printf("\n*E: Result not matched with golden data\n");
		printf("    A      = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f\n",
			   As.u, As.fp.sign, As.fp.exponent, As.fp.mantissa, As.f);
		printf("    B      = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f\n",
			   Bs.u, Bs.fp.sign, Bs.fp.exponent, Bs.fp.mantissa, Bs.f);
		printf("    RESULT = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f <- NOT MATCHED!\n",
			   Os.u, Os.fp.sign, Os.fp.exponent, Os.fp.mantissa, Os.f);
		printf("    GOLDEN = 0x%08X [s(%d),e(0x%02X),m(0x%06X)] : %f\n",
			   Gs.u, Gs.fp.sign, Gs.fp.exponent, Gs.fp.mantissa, Gs.f);
		fflush(stdout);
		return false;
	}

	return true;
}

vSim::vSim(void)
{
	m_pContext	= NULL;
	m_pSimTop	= NULL;
}

vSim::~vSim(void)
{
	Release();
}

void vSim::Release(void)
{
	if(m_pSimTop) {
		m_pSimTop->final();
	}

	SAFE_DELETE(m_pSimTop);
	SAFE_DELETE(m_pContext);
}

bool vSim::Initialize(void)
{
	srand(time(NULL));			// randomize seed
	m_pContext		= new VerilatedContext;
	m_pContext->randReset(2);	// randomize all bits
	m_pSimTop		= new SimTop(m_pContext);
	return true;
}

