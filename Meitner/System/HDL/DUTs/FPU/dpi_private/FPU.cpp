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
// Title : FPU
// Rev.  : 3/1/2021 Mon (clonextop@gmail.com)
//================================================================================
#include "dpi_common.h"

DPI_FUNCTION void FPU_32f_add(const svBitVecVal* A, const svBitVecVal* B, svBitVecVal* O)
{
	const float& fA		= *(const float*)A;
	const float& fB		= *(const float*)B;
	float& fO			= *(float*)O;
	fO					= fA + fB;
}

DPI_FUNCTION void FPU_32f_Multiply(const svBitVecVal* A, const svBitVecVal* B, svBitVecVal* O)
{
	const float& fA		= *(const float*)A;
	const float& fB		= *(const float*)B;
	float& fO			= *(float*)O;
	fO					= fA / fB;
}

DPI_FUNCTION void FPU_32f_Divide(const svBitVecVal* A, const svBitVecVal* B, svBitVecVal* O)
{
	const float& fA		= *(const float*)A;
	const float& fB		= *(const float*)B;
	float& fO			= *(float*)O;
	fO					= (fB != 0) ? fA / fB : 0;
}

DPI_FUNCTION void FPU_32f_to_int(const svBitVecVal* A, svBitVecVal* O)
{
	const float& fA		= *(const float*)A;
	int& fO				= *(int*)O;
	fO					= fA;
}

DPI_FUNCTION void FPU_32f_from_int(const svBitVecVal* A, svBitVecVal* O)
{
	const int& fA		= *(const int*)A;
	float& fO			= *(float*)O;
	fO					= fA;
}