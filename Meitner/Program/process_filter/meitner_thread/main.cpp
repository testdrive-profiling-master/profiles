//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 10/4/2019 Fri (clonextop@gmail.com)
//================================================================================
#include <stdio.h>
#include <tchar.h>

// signal structure
typedef struct {
	union {
		unsigned int	all;
		struct {
			unsigned int	pc		: 30;
			unsigned int	op		: 1;
			unsigned int	en		: 1;
		};
	};
} REG;

int _tmain(int argc, _TCHAR* argv[])
{
	REG	reg;

	while(!feof(stdin)) {
		char buf[32];
		buf[0] = 0;
		fscanf(stdin, "%s", buf);

		if(buf[0]) {
			sscanf(buf, "%x", &reg.all);

			if(reg.en) {
				if(reg.op)
					printf("?blue?Run PC(0x%08X)\n", (reg.pc << 2));
				else
					printf("?red?Stop\n");
			} else {
				printf(" \n");
			}

			fflush(stdout);
		}
	}

	return 0;
}
