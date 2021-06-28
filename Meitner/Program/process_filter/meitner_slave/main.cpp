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

typedef struct {
	union {
		struct {
			unsigned int	ready		: 1;
			unsigned int	we			: 1;
			unsigned int	en			: 1;
			unsigned int	/*dummy*/	: 0;
		};
		unsigned int	control;
	};
	unsigned int	addr;
	unsigned int	rdata;
	unsigned int	wdata;
} SLAVE;


int main(int argc, const char* argv[])
{
	SLAVE	bus_slave;

	while(!feof(stdin)) {
		char buf[256], *pTok;
		pTok	= buf;
		fscanf(stdin, "%s", buf);

		if(*pTok) {
			// read DATA
			sscanf(pTok, "%1X", &bus_slave.control);
			pTok++;

			if(bus_slave.en) {
				sscanf(pTok, "%4X", &bus_slave.addr);
				pTok += 4;
				printf("?%s?", bus_slave.ready ? (bus_slave.we ? "red" : "blue") : "gray");
				printf("%s[%d]", bus_slave.we ? "Wr" : "Rd", bus_slave.addr);

				if(bus_slave.ready) {
					if(bus_slave.we) {
						pTok += 8;
						sscanf(pTok, "%8X", &bus_slave.wdata);
						printf("<=0x%08X", bus_slave.wdata);
					} else {
						sscanf(pTok, "%8X", &bus_slave.rdata);
						printf("=>0x%08X", bus_slave.rdata);
					}
				}

				printf("\n");
			} else {
				printf("?black?\n");
			}

			fflush(stdout);
		}
	}

	return 0;
}
