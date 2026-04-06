//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Run as Administrator
// Rev.  : 4/5/2026 Sun (CloneX)
//================================================================================
#include "UACElevate.h"
#include "UtilFramework.h"

int main(int argc, const char *argv[])
{
	if (argc == 1) {
		printf("Run as Administrator 1.0 (" __DATE__ ", Q&A : clonextop@gmail.com)\n\n");
		printf("Usage : run_as_admin ...\n");
		return 0;
	}

	cstring cmd, cur_path(".");
	// get current full path
	cur_path.MakeFullPath();
	if (cur_path.find(' ') >= 0) {
		cur_path.insert(0, "\"");
		cur_path += "\"";
	}

	cmd.Format("/C cd /d %s&&", cur_path.c_str());

	// append all command
	for (int i = 1; i < argc; i++) {
		cstring s = argv[i];
		s.Replace("\"", "\\\"", true);
		if (s.find(' ') >= 0) {
			s.insert(0, "\"");
			s += "\"";
		}
		if (i != 1)
			cmd += " ";
		cmd += s;
	}

	// check for no-wait
	cstring no_wait;
	no_wait.GetEnvironment("RUN_AS_ADMIN_NOWAIT");
	bool bWait = (no_wait != "TRUE");
	no_wait.clear();
	no_wait.SetEnvironment("RUN_AS_ADMIN_NOWAIT");

	// run with admin. privileges
	return ShellExecWithElevation("cmd.exe", cmd, ".", bWait) ? 0 : 1;
}
