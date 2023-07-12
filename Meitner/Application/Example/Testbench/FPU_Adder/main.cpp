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
// Rev.  : 7/12/2023 Wed (clonextop@gmail.com)
//================================================================================
#include "vSim.h"
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

bool RetrieveParams(uint32_t& A, uint32_t& B)
{
	if(__param.m == 0xFFFFFFFF'FFFFFFFFULL) return false;

	__param.m++;
	A	= __param.v[0].u;
	B	= __param.v[1].u;
	return true;
}

int main(int argc, const char* argv[])
{
	atomic<bool>	bFine	= true;
	#pragma omp parallel
	{
		vSim	sim;

		if(sim.Initialize()) {
			RetrieveParams(sim.Top()->A, sim.Top()->B);
			sim.Eval();
			printf("A(%f) + B(%f) = %f\n", *(float*) & (sim.Top()->A), *(float*) & (sim.Top()->B), *(float*) & (sim.Top()->O));
		}
	}
	return 0;
}
