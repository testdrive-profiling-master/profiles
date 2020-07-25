//================================================================================
// Copyright (c) 2013 ~ 2018. HyungKi Jeong(clonextop@gmail.com)
// 
// MIT License
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// Title : Process filter
// Rev.  : 6/8/2018 Fri (clonextop@gmail.com)
//================================================================================
#include <stdio.h>
#include <tchar.h>

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


int _tmain(int argc, _TCHAR* argv[])
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
