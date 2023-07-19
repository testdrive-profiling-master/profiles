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
#include "vSim.h"

uint32_t fadd_reference(uint32_t a, uint32_t b)
{
	//Taking care of zero cases
	if(a == 0) {
		return b;
	}

	if(b == 0) {
		return a;
	}

	//Extracting information from a and b
	unsigned int a_sign = (a & 0x80000000) >> 31;
	unsigned int a_enc_exp = (a & 0x7f800000) >> 23;
	unsigned int a_mantissa = (a & 0x7fffff);
	unsigned int b_sign = (b & 0x80000000) >> 31;
	unsigned int b_enc_exp = (b & 0x7f800000) >> 23;
	unsigned int b_mantissa = (b & 0x7fffff);
	unsigned int a_significand = (a_enc_exp >= 1) ? (a_mantissa | (1 << 23)) : a_mantissa;
	unsigned int b_significand = (b_enc_exp >= 1) ? (b_mantissa | (1 << 23)) : b_mantissa;
	//Initially shifting a and b 7 bits left to increase precison for rounding
	unsigned int a_shift_significand = (a_significand << 7);
	unsigned int b_shift_significand = (b_significand << 7);
	//Taking care of denormal numbers
	unsigned int a_exp = ((a_enc_exp == 0) ? 1 : a_enc_exp);
	unsigned int b_exp = ((b_enc_exp == 0) ? 1 : b_enc_exp);
	unsigned int ans_exp;
	unsigned int ans_significand;
	unsigned int ans_sign;
	bool ans_denormal = false;

	/* Special Cases */

	//Case when a is NaN
	if(a_exp == 255 && a_mantissa != 0) {
		return 0x7fffffff;
	}

	//Case when b is NaN
	if(b_exp == 255 && b_mantissa != 0) {
		return 0x7fffffff;
	}

	//Case when Infinity - Infinity
	if(a_exp == 255 && a_mantissa == 0 && b_exp == 255 && b_mantissa == 0 && a_sign != b_sign) {
		return 0x7fffffff;
	}

	//Case when a is Infinity
	if(a_exp == 255 && a_mantissa == 0) {
		return a;
	}

	//Case when b is Infinty
	if(b_exp == 255 && b_mantissa == 0) {
		return b;
	}

	/* Making Exponent Same */

	if(a_exp >= b_exp) {
		unsigned int shift = a_exp - b_exp;
		b_shift_significand = (b_shift_significand >> ((shift > 31) ? 31 : shift));
		ans_exp = a_exp;
	} else {
		unsigned int shift = b_exp - a_exp;
		a_shift_significand = (a_shift_significand >> ((shift > 31) ? 31 : shift));
		ans_exp = b_exp;
	}

	/* Adding Significands */
	if(a_sign == b_sign) {
		ans_significand = a_shift_significand + b_shift_significand;
		ans_sign = a_sign;
	} else {
		if(a_shift_significand > b_shift_significand) {
			ans_sign = a_sign;
			ans_significand = a_shift_significand - b_shift_significand;
		} else if(a_shift_significand < b_shift_significand) {
			ans_sign = b_sign;
			ans_significand = b_shift_significand - a_shift_significand;
		} else if((a_shift_significand == b_shift_significand)) {
			ans_sign = 0;
			ans_significand = a_shift_significand - b_shift_significand;
		}
	}

	LOGI("ans_significand = %X\n", ans_significand);
	/* Normalization */
	int i;

	for(i = 31; i > 0 && ((ans_significand >> i) == 0); i--) {;}

	if(i > 23) {
		//Rounding
		unsigned int residual = ((ans_significand & (1 << (i - 23 - 1))) >> (i - 23 - 1));
		unsigned int sticky = 0;

		for(int j = 0; j < i - 23 - 1; j++) {
			sticky = sticky | ((ans_significand & (1 << j)) >> j);
		}

		LOGI("sticky(%d), residual(%d)\n", sticky, residual);

		if((int(ans_exp) + (i - 23) - 7) > 0 && (int(ans_exp) + (i - 23) - 7) < 255) {
			ans_significand = (ans_significand >> (i - 23));
			ans_exp = ans_exp + (i - 23) - 7;

			if(residual == 1 && sticky == 1) {
				ans_significand += 1;
			} else if((ans_significand & 1) == 1 && residual == 1 && sticky == 0) {
				ans_significand += 1;
			}

			if((ans_significand >> 24) == 1) {
				ans_significand = (ans_significand >> 1);
				ans_exp += 1;
			}
		}
		//Denormal number
		else if(int(ans_exp) + (i - 23) - 7 <= 0) {
			ans_denormal = true;
			ans_significand = ans_significand >> 7;
			ans_significand = ans_significand << (ans_exp - 1);
			ans_exp = 0;
		}
		//Overflow
		else if(int(ans_exp) + (i - 23) - 7 >= 255) {
			ans_significand = (1 << 23);
			ans_exp = 255;
		}
	} else if(i <= 23 && i != 0) {
		if((int(ans_exp) - (23 - i) - 7) > 0 && (int(ans_exp) - (23 - i) - 7) < 255) {
			ans_significand = (ans_significand << (23 - i));
			ans_exp = ans_exp - (23 - i) - 7;
		}
		//Denormal Number
		else if(int(ans_exp) - (23 - i) - 7 <= 0) {
			ans_denormal = true;
			ans_significand = ans_significand >> 7;
			ans_significand = ans_significand << (ans_exp - 1);
			ans_exp = 0;
		}
		//Overflow
		else if((int(ans_exp) - (23 - i) - 7) >= 255) {
			ans_significand = (1 << 23);
			ans_exp = 255;
		}
	}
	//When answer is zero
	else if(i == 0 && ans_exp < 255) {
		ans_exp = 0;
	}

	LOGI("final ans_exp = %X\n", ans_exp);
	LOGI("final ans_significand = %X\n", ans_significand);
	/* Constructing floating point number from sign, exponent and significand */
	uint32_t ans = (ans_sign << 31) | (ans_exp << 23) | (ans_significand & (0x7FFFFF));
	return ans;
}

static bool	__bReferenceMode	= false;
float fadd_golden(float A, float B)
{
	if(__bReferenceMode) {
		uint32_t out = fadd_reference(*(uint32_t*)&A, *(uint32_t*)&B);
		return *(float*)&out;
	}

	return A + B;
}

int main(int argc, const char* argv[])
{
	if(GetAsyncKeyState(VK_LCONTROL) < 0 && GetAsyncKeyState(VK_LSHIFT) < 0) {
		__bReferenceMode	= true;
		printf("***************************************\n");
		printf("***** REFERENCE MODE IS ACTIVATED *****\n");
		printf("***************************************\n\n");
	}

	DO_FP32_TEST_QUICK(fadd_golden, sim.Top()->O, sim.Top()->A, sim.Top()->B)
	DO_FP32_TEST_FULL(fadd_golden, sim.Top()->O, sim.Top()->A, sim.Top()->B)
	return 0;
}
