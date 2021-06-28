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
// Title : Process filter
// Rev.  : 6/28/2021 Mon (clonextop@gmail.com)
//================================================================================
#include "STDInterface.h"

int main(int argc, const char* argv[])
{
	while(!feof(stdin)) {
		char buf[256];
		buf[0] = 0;
		fscanf(stdin, "%s", buf);

		if(buf[0]) {
			switch((buf[0] - '0') >> 1) {
			default:
			case 0:
			case 1:	// valid
			case 2:	// ready
				printf("?black?\n");
				break;

			case 3: {	// valid & ready
				char ctemp;
				unsigned int	data[4];

				for(int i = 0; i < 4; i++) {
					int last_c	= (i + 1) * 8 + 1;
					ctemp		= buf[last_c];
					buf[last_c]	= 0;
					sscanf(&buf[1 + (8 * i)], "%x", &data[i]);
					buf[last_c]	= ctemp;
				}

				printf("?%s?%08X:%08X:%08X:%08X%s\n", (buf[0] & 1) ? "red" : "blue", data[0], data[1], data[2], data[3], (buf[0] & 1) ? "-Last" : "");
			}
			break;
			}

			fflush(stdout);
		}
	}

	return 0;
}
