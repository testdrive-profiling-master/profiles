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
// Rev.  : 7/19/2023 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __V_SIM_H__
#define __V_SIM_H__
#include "STDInterface.h"
#include "TestDriver.h"
#include "SimTop.h"
#include <math.h>
#include <mutex>

#define	LOGI(...)	{printf(__VA_ARGS__);}

typedef union {
	int32_t		i;
	uint32_t	u;
	float		f;
} SIM_VALUE;

void EnableReferenceTest(bool bEnable = true);

bool RetrieveFP32_TestParam(uint32_t& A);
bool RetrieveFP32_TestParam(uint32_t& A, uint32_t& B);
bool RetrieveFP32_Param(uint32_t& A);
bool RetrieveFP32_Param(uint32_t& A, uint32_t& B);

typedef float (*FP32_GOLDEN_1)(float A);
typedef float (*FP32_GOLDEN_2)(float A, float B);
bool CheckFP32_Result(FP32_GOLDEN_1 golden_func, uint32_t A, uint32_t O);
bool CheckFP32_Result(FP32_GOLDEN_2 golden_func, uint32_t A, uint32_t B, uint32_t O);

class vSim {
public:
	vSim(void);
	~vSim(void);

	bool Initialize(void);
	void Release(void);
	inline void Eval(void) {
		m_pSimTop->eval();
	}
	inline SimTop* Top(void) {
		return m_pSimTop;
	}

private:
	VerilatedContext*	m_pContext;
	SimTop*				m_pSimTop;
};

#define	DO_FP32_TEST_QUICK(golden_func, out, ...) \
	printf("- Check FPU32 quick test sequence.\n\n");\
	{\
		vSim	sim;\
		if(sim.Initialize()) for(;;) {\
			if(!RetrieveFP32_TestParam(__VA_ARGS__)) break;\
			sim.Eval();\
			if(!CheckFP32_Result(golden_func, __VA_ARGS__, out)) {\
				printf("\n");\
				return 1;\
			}\
		}\
	}

#define DO_FP32_TEST_FULL(golden_func, out, ...) \
		printf("- Check FPU32 full test sequence.\n\n");\
		_Pragma("omp parallel")\
		{\
			vSim	sim;\
			if(sim.Initialize()) for(;;) {\
				if(!RetrieveFP32_Param(__VA_ARGS__)) break;\
				sim.Eval();\
				if(!CheckFP32_Result(golden_func, __VA_ARGS__, out)) break;\
			}\
		}\
		printf("\n");
#endif//__V_SIM_H__
