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
// Rev.  : 7/13/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <filesystem>



bool CheckDepency(const char* sDepFileName)
{
	TextFile	f;
	LOGI("@3- %s\n", sDepFileName);

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
				} else break;	// not a dependency file.
			}

			sLine.Trim(" \r\n");
			bool bContinue	= sLine.CompareBack("\\");

			if(bContinue) sLine.Trim(" \\");

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

			if(!bContinue) break;	// no more dependency list
		}

		return true;
	}

	return false;
}

int main(int argc, const char* argv[])
{
	int			arg_size;
	ArgTable	arg_table("TestDrive's dependency preparation check.");
	arg_table.SetDefaultArguments(argc, argv);
	arg_table.AddOption("suppress", "s", NULL, "Suppress the log output.");
	arg_table.AddOptionFiles("dep_files", NULL, NULL, "dep_files", "dependency files...");
	arg_table.AddRemark(NULL, "file extension must be .d");

	if(!arg_table.DoParse(argc, argv))
		return 0;

	LOG_Suppress(arg_table.GetOption("suppress"));

	for(int i = 0;; i++) {
		const char* sFileName = arg_table.GetOptionFile("dep_files", i);

		if(!sFileName) break;

		cstring sDepFileName(sFileName);
		cstring cExt(sDepFileName);
		cExt.CutFront(".", true);
		cExt.MakeLower();

		if(cExt != "d") {
			LOGW("Unknown file extension : %s", sDepFileName.c_str());
			continue;
		}

		{
			cstring	sObjectFile	= sDepFileName;
			sObjectFile.DeleteBack(".d");
			sObjectFile	+= ".o";

			if(!access(sObjectFile, F_OK)) {	// object file is existed?
				if(!CheckDepency(sDepFileName)) {
					LOGW("Dependency(%s) is broken, it will be deleted.", sDepFileName.c_str());
					remove(sDepFileName.c_str());	// delete dependency file
					remove(sObjectFile.c_str());	// delete object file
				}
			}
		}
	}

	return 0;
}
