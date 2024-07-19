//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Process filter
// Rev.  : 7/19/2024 Fri (clonextop@gmail.com)
//================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

typedef struct {
	unsigned int data[4];
	union {
		unsigned int m;
		struct {
			unsigned int branch_op	   : 2;
			unsigned int /*reserved*/  : 2;
			unsigned int branch_en	   : 1;
			unsigned int intterrupt_on : 1;
			unsigned int run_count_on  : 1;
			unsigned int icache_clear  : 1;
			unsigned int gcache_clear  : 1;
			unsigned int lcache_clear  : 1;
			unsigned int /*reserved*/  : 0;
		};
	} op;
	union {
		unsigned int m;
		struct {
			unsigned int ready		  : 1;
			unsigned int en			  : 1;
			unsigned int /*reserved*/ : 0;
		};
	} ctrl;
} SIGNAL_DATA;

int _tmain(int argc, _TCHAR *argv[])
{
	SIGNAL_DATA s;

	while (!feof(stdin)) {
		char buf[256];
		fscanf(stdin, "%s", buf);
		sscanf(buf, "%01x%08x%08x%08x%08x%08x", &s.ctrl.m, &s.op.m, &s.data[3], &s.data[2], &s.data[1], &s.data[0]);

		if (s.ctrl.en) {
			if (s.ctrl.ready)
				printf("?red?");
			else
				printf("?dim gray?");

			bool bFirst = true;

			if (s.op.branch_en) {
				if (s.op.branch_op >= 2)
					printf("Invalid branch");
				else
					printf("Run%s 0x%X_%08X", (s.op.branch_op == 1) ? ".all" : "", s.data[1], s.data[0]);

				bFirst = false;
			}

			if (s.op.intterrupt_on) {
				if (!bFirst)
					printf(", ");
				else
					bFirst = false;

				printf("INTR");
			}

			if (s.op.run_count_on) {
				if (!bFirst)
					printf(", ");
				else
					bFirst = false;

				printf("RUNCOUNT");
			}

			if (s.op.icache_clear) {
				if (!bFirst)
					printf(", ");
				else
					bFirst = false;

				printf("I$");
			}

			if (s.op.gcache_clear) {
				if (!bFirst)
					printf(", ");
				else
					bFirst = false;

				printf("G$");
			}

			if (s.op.lcache_clear) {
				if (!bFirst)
					printf(", ");
				else
					bFirst = false;

				printf("L$");
			}

			printf("\n");
		} else {
			printf("?black?\n");
		}

		fflush(stdout);
	}

	return 0;
}
