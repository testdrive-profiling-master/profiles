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
// Title : TestDrive installer Release Build
// Rev.  : 7/22/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <unistd.h>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <string>
#include <map>
#include <boost/filesystem.hpp>

static string __exec(const char *cmd)
{
	cstring sCmd(cmd);
	char	buffer[4096];
	string	result = "";
	sCmd += " 2>&1"; // redirect catch stderr
	FILE *pipe = popen(sCmd.c_str(), "r");

	if (!pipe)
		throw runtime_error("popen() failed!");

	try {
		while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}

	pclose(pipe);
	return result;
}

bool IsFileExists(const char *path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return false;

	if (info.st_mode & S_IFMT)
		return true;

	return false;
}

map<string, int> g_LibrariesMap;

int				 main(int argc, const char *argv[])
{
	// check existence of installer
	if (access("release/TestDrive_Installer.exe", F_OK)) {
		printf("*E: No Target file...\n");
		exit(1);
	}

	// clear previous library dependencies
	__exec("rm -f release/*.dll");
	// collect new library dependencies
	printf("*I: Copy MinGW libraries...\n");
	system("codegen collect_lib release/TestDrive_Installer.exe");

	// add extra files
	for (const auto &file : boost::filesystem::directory_iterator(boost::filesystem::path("release"))) {
		g_LibrariesMap[file.path().filename().string().c_str()] = 1;
	}

	// insert file list of SED description
	{
		TextFile f;

		if (!f.Open("TestDriveInstaller.sed.default")) {
			printf("*E: No installer SED.\n");
			exit(1);
		}

		cstring sSED;
		f.GetAll(sSED);
		f.Close();

		if (!f.Create("TestDriveInstaller.sed")) {
			printf("*E: Can't create installer SED.\n");
			exit(1);
		}

		// fill file description
		{
			cstring sFileList;
			cstring sSourceFiles;
			int		index = 0;
			for (auto &i : g_LibrariesMap) {
				sFileList.AppendFormat("FILE%d=\"%s\"\n", index, i.first.c_str());
				sSourceFiles.AppendFormat("%%FILE%d%%==\n", index);
				index++;
			}
			sFileList.Trim("\n");
			sSourceFiles.Trim("\n");
			sSED.Replace("@FILE_LIST@", sFileList);
			sSED.Replace("@SOURCE_LIST@", sSourceFiles);
		}

		f.Puts(sSED);
		f.Close();
	}

	return 0;
}
