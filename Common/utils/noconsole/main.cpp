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
// Title : noconsole project
// Rev.  : 10/23/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <shellapi.h>

int main(int argc, const char *argv[])
{
	if (argc == 1)
		return 0;
	cstring				sCmd;
	STARTUPINFO			stInfo;
	PROCESS_INFORMATION prInfo;

	// make command from all arguments
	for (int i = 1; i < argc; i++) {
		cstring sArg(argv[i]);
		if (sArg.find(" ") > 0) {
			sArg.insert(0, "\"");
			sArg.Append('\"');
		}
		if (i != 1)
			sCmd.Append(' ');
		sCmd.Append(sArg);
	}

	ZeroMemory(&stInfo, sizeof(stInfo));
	stInfo.cb		   = sizeof(stInfo);
	stInfo.dwFlags	   = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOW;
	BOOL bResult =
		CreateProcess(NULL, (char *)sCmd.c_str(), NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &stInfo, &prInfo);
	return bResult ? 0 : 1;
}
