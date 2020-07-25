//================================================================================
// MIT License
// 
// Copyright (c) 2013 ~ 2017. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 10/2/2017 Mon (clonextop@gmail.com)
//================================================================================
#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
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

// 		if(buf[0]){
// 			char ctemp;
// 			unsigned int	control, addr, data;
// 			// read REQ/VALID
// 			ctemp = buf[1];buf[1]=0;
// 			sscanf(buf, "%x", &control);
//
// 			if(control == 3){
// 				unsigned int	addr, data;
// 				// read ADDR
// 				buf[1] = ctemp;
// 				ctemp = buf[3];buf[3]=0;
// 				sscanf(&buf[1], "%x", &addr);
// 				// read DATA
// 				buf[3] = ctemp;
// 				sscanf(&buf[3], "%x", &data);
//
// 				printf("?dark blue?r[%d]:0x%08X\n", addr, data);
// 			}else{
// 				printf("?black?\n");
// 			}
// 			fflush(stdout);
// 		}
	}

	return 0;
}
