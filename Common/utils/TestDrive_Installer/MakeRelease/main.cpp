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
// Title : TestDrive installer Release Build
// Rev.  : 1/31/2023 Tue (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include <unistd.h>
#include <sstream>
#include <string>
#include <map>

static string __exec(const char* cmd)
{
	cstring	sCmd(cmd);
	char buffer[4096];
	string result = "";
	sCmd	+= " 2>&1";		// redirect catch stderr
	FILE* pipe = popen(sCmd.c_str(), "r");

	if(!pipe) throw runtime_error("popen() failed!");

	try {
		while(fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}
	} catch(...) {
		pclose(pipe);
		throw;
	}

	pclose(pipe);
	return result;
}

map<string, int>	g_LibrariesMap;

int main(int argc, const char* argv[])
{
	cstring sToolPath	= getenv("TESTDRIVE_DIR");

	// get tool path
	if(sToolPath.IsEmpty()) {
		printf("*E: No TestDrive...\n");
		exit(1);
	}

	// check existence of installer
	if(access("release/TestDrive_Installer.exe", F_OK)) {
		printf("*E: No Target file...\n");
		exit(1);
	}

	// clear previous library dependencies
	__exec("rm -f release/*.dll");
	// collect new library dependencies
	cstring	sOut = __exec("ldd release/TestDrive_Installer.exe | grep ucrt64");
	{
		int iTokenPos = 0;

		while(iTokenPos >= 0) {
			cstring	sTok = sOut.Tokenize(iTokenPos, "\r\n");
			sTok.CutBack("=>");
			sTok.Trim(" \t");

			if(!sTok.Length()) continue;

			g_LibrariesMap[sTok.c_str()]	= 1;
		}
	}
	sToolPath	+= "bin/msys64/ucrt64/bin/";

	// copy libraries
	for(auto& i : g_LibrariesMap) {
		cstring sCmd;
		printf("Updating '%s'...\n", i.first.c_str());
		sCmd.Format("cp -r \"%s%s\" release", sToolPath.c_str(), i.first.c_str());
		__exec(sCmd.c_str());
	}

	return 0;
}
