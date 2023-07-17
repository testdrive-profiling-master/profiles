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
// Rev.  : 7/17/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "vSim.h"
#include <math.h>
#include <atomic>
#include <mutex>

using namespace std;

typedef union {
	uint64_t	m;
	union {
		int32_t		i;
		uint32_t	u;
		float		f;
	} v[2];
} PARAMs;

static PARAMs	__param	= {0};
std::mutex		__param_mutex;

bool RetrieveParams(uint32_t& A, uint32_t& B)
{
	bool	bLog;
	{
		std::lock_guard<std::mutex> guard(__param_mutex);

		if(__param.m == 0xFFFFFFFF'FFFFFFFFULL) return false;

		bLog	= (__param.m & 0xFFFFFF) == 0;
		A		= __param.v[0].u;
		B		= __param.v[1].u;

		// bypassing nan
		if(!isnormal(__param.v[1].f)) {
			__param.v[1].u	+= (1 << 21);
		} else if(!isnormal(__param.v[1].f)) {
			__param.v[0].u	+= (1 << 21);
		} else {
			__param.m++;
		}
	}

	if(bLog) {
		double	fRatio	= (double)(__param.m >> 24) / 0xFFFFFFFFFFULL;
		printf("\r %f%%[%d] completed.", fRatio * 100, __param.m);
		fflush(stdout);
	}

	return true;
}

bool CheckResult(vSim& sim)
{
	float	golden	= (*(float*) & (sim.Top()->A)) * (*(float*) & (sim.Top()->B));
	float	result	= *(float*) & (sim.Top()->O);

	if(result != golden) {
		std::lock_guard<std::mutex> guard(__param_mutex);
		printf("\n");
		fflush(stdout);
		printf("*E: A(%f[0x%08X]) * B(%f[0x%08X]) = %f[0x%08X] (!= %f[0x%08X] golden)",
			   *(float*) & (sim.Top()->A), sim.Top()->A,
			   *(float*) & (sim.Top()->B), sim.Top()->B,
			   *(float*) & (sim.Top()->O), sim.Top()->O,
			   golden, *(uint32_t*)&golden);
		return false;
	}

	return true;
}

int main(int argc, const char* argv[])
{
	atomic<bool>	bFine	= true;
	printf("- Check FPU32 Multiplier\n\n");
	#pragma omp parallel
	{
		vSim	sim;

		if(sim.Initialize()) for(;;) {
				if(!RetrieveParams(sim.Top()->A, sim.Top()->B)) {
					// last
					break;
				}

				sim.Eval();

				if(!CheckResult(sim)) break;
			}
	}
	printf("\n");
	return 0;
}
