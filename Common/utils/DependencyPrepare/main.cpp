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
// Title : Dependency prepare
// Rev.  : 2/2/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include <filesystem>

bool CheckDepency(const char* sDepFileName)
{
	TextFile	f;

	if(f.Open(sDepFileName)) {
		bool	bFirstLine	= true;
		cstring sLine;
		auto		dep_time	= filesystem::last_write_time(sDepFileName);

		while(f.GetLine(sLine)) {
			if(bFirstLine) {
				int iPos = sLine.find(".o:");

				if(iPos > 0) {
					sLine.erase(0, iPos + 3);
					bFirstLine		= false;
				}
			}

			sLine.Trim(" \\\r\n");
			{
				// dependency file existence check
				for(int iPos = 0;;) {
					cstring sFileName	= sLine.Tokenize(iPos, " ");

					if(iPos < 0) break;

					// access check, if can't access this file, dependency is broken.
					if(access(sFileName.c_str(), F_OK)) {
						return false;
					}

					// check dependency file time
					auto		cmp_time	= filesystem::last_write_time(sFileName.c_str());

					if(cmp_time > dep_time) {
						return false;
					}
				}
			}
		}
	}

	return true;
}

int main(int argc, const char* argv[])
{
	int			arg_size;
	ArgTable	arg_table("TestDrive's dependency preparation check.");
	arg_table.AddOption("silence", "s", "silence", "Force to no logging.");
	arg_table.AddOptionFile("dep_file", NULL, NULL, NULL, "dep_file", "dependency file.");
	arg_table.AddRemark(NULL, "file extension must be .d");

	if(!arg_table.DoParse(argc, argv))
		return 0;

	bool bSilence	= arg_table.GetOption("silence");
	cstring sDepFileName(arg_table.GetOptionFile("dep_file"));
	cstring cExt(sDepFileName);
	cExt.CutFront(".", true);
	cExt.MakeLower();

	if(cExt != "d") {
		if(!bSilence) LOGE("Unknown file extension : %s", sDepFileName.c_str());

		return 0;
	}

	{
		if(!CheckDepency(sDepFileName)) {
			cstring sCC(arg_table.GetOptionString("CC"));
			cstring sCXX(arg_table.GetOptionString("CXX"));

			if(!bSilence) LOGI("Dependency is broken, it will be deleted. : %s", sDepFileName.c_str());

			remove(sDepFileName.c_str());
		}
	}

	return 0;
}
