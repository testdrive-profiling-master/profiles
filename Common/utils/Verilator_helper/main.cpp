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
// Title : Verilator helper
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

cstring		g_sTestDrivePath;

static string GetEnvString(const char* sKey)
{
	char	sStr[1024 * 64];
	GetPrivateProfileString("VERILATOR", sKey, "", sStr, 1024 * 64, g_sTestDrivePath + "/verilator_helper.ini");
	return sStr;
}

static void SetEnvString(const char* sKey, const char* sStr)
{
	WritePrivateProfileString("VERILATOR", sKey, sStr, g_sTestDrivePath + "/verilator_helper.ini");
}

int RepoCopy(const char* sTargetPath)
{
	if(!sTargetPath) {
		printf("* Verilator's extra function *\n" \
			   "  : Copy from latest build results to 'target' path.\n\n" \
			   "  Usage : verilator repo TARGET_PATH\n\n");
	}

	cstring	sLatestMdir	= GetEnvString("LATEST_MDIR");

	if(sLatestMdir.IsEmpty()) {
		LOGE("No recent build result.");
		return 1;
	}

	{
		// target is a valid directory?
		auto target_path	= fs::path(sTargetPath);

		if(!fs::exists(target_path) || !fs::is_directory(target_path)) {
			LOGE("Target path is not exist or not a directory. : '%s'\n", target_path);
			return 1;
		}

		// add nosearch directive
		{
			cstring	sNosearchPath(sTargetPath);
			sNosearchPath	+= "/.TestDrive.nosearch";
			target_path	= fs::path(sNosearchPath.c_str());

			if(!fs::exists(target_path)) {
				ofstream ofs(target_path);
				ofs.close();
			}
		}
	}

	{
		// remove all simulation results.
		cstring sCmd;
		sCmd.Format("rm -f %s/SimTop.* >nul 2>&1", sTargetPath);
		system(sCmd);
		sCmd.Format("rm -f %s/SimTop_* >nul 2>&1", sTargetPath);
		// copy new simulation results.
		system(sCmd);
		sCmd.Format("cp -f %s/SimTop*.cpp %s >nul 2>&1", sLatestMdir.c_str(), sTargetPath);
		system(sCmd);
		sCmd.Format("cp -f %s/SimTop*.h %s >nul 2>&1", sLatestMdir.c_str(), sTargetPath);
		system(sCmd);
		sCmd.Format("cp -f %s/SimTop*.mk %s >nul 2>&1", sLatestMdir.c_str(), sTargetPath);
		system(sCmd);
		sCmd.Format("cp -f %s/SimTop*.dat %s >nul 2>&1", sLatestMdir.c_str(), sTargetPath);
		LOGI("Done!");
	}

	return 0;
}

int main(int argc, const char* argv[])
{
	cstring	sArg;	// argument path
	cstring	sExe;	// verilator path
	cstring	sCmd;	// modified command line

	if(!g_sTestDrivePath.GetEnvironment("TESTDRIVE_DIR")) {
		LOGE("You must run TestDrive Profiling Master first.");
		return 1;
	}

	if(argc >= 2 && !strcmp(argv[1], "repo")) {	// repo copy
		return RepoCopy((argc == 3) ? argv[2] : NULL);
	}

	{
		// check pre-compiled repo.
		bool	bPreCompiled	= false;
		cstring	sDst;
		cstring sRepo;
		{
			cstring	sSrc;

			for(int i = 1; i < argc; i++) {
				cstring sArg = argv[i];

				if(sRepo.IsEmpty() && !sArg.CompareFront("-")) {
					sRepo	= argv[i];
					sRepo.Replace("\\", "/", true);
					sRepo.CutBack("/");
					sRepo	+= "/repo/";
				}

				if(sArg.CompareFront("-Drepo=")) {
					bPreCompiled	= true;
					sSrc	= sArg;
					sSrc.CutFront("-Drepo=");
					sSrc.Trim(" \"");
				} else if(sArg == "-Mdir" && (i + 1) < argc) {
					i++;
					sDst	= argv[i];
					sDst.Trim(" \"");
					// update latest target path
					SetEnvString("LATEST_MDIR", fs::absolute(fs::path(sDst.c_str())).string().c_str());
				}
			}

			if(bPreCompiled) {	// source repo. path check
				if(sRepo.IsEmpty()) {
					LOGE("Can't find repo. path.", sRepo.c_str());
					return 1;
				}

				sRepo	+= sSrc;
				auto repo_path	= fs::path(sRepo.c_str());

				if(!fs::exists(repo_path) || !fs::is_directory(repo_path)) {
					LOGE("Can't access source repo. path : '%s'\n", sRepo.c_str());
					return 1;
				}

				if(sDst.IsEmpty()) sDst = "./";
				else {
					sDst	+= '/';
				}
			}
		}

		if(bPreCompiled) {
			auto src_path = fs::absolute(fs::path(sRepo.c_str()));
			cstring sCmd;
			sCmd.Format("cp -f -v %s/* %s", sRepo.c_str(), sDst.c_str());
			system(sCmd);
			return 0;
		}
	}

	// make arguments
	if(argc != 1) {
		// get given arguments
		for(int i = 1; i < argc; i++) {
			cstring	s(argv[i]);

			if(s.find(' ') >= 0) {
				s.insert(0, "\"");
				s	+= "\"";
			}

			sArg.AppendFormat(" %s", s.c_str());
		}

		// get extra arguments from file description
		{
			cstring	sPath;

			if(sPath.GetEnvironment("SIM_TOP_FILE")) {
				int		pos;
				// delete '"'
				sPath.Replace("\"", "", true);
				// change linux style path to windows
				sPath.Replace("/", "\\", true);

				if((pos = sPath.rfind("\\")) > 0) {
					sPath.erase(pos, -1);
					sPath += "\\.verilator";
					TextFile	f;

					if(f.Open(sPath.c_str())) {
						cstring sLine;

						while(f.GetLine(sLine)) {
							sLine.Trim(" \r\n");

							if(sLine.size())
								sArg.AppendFormat(" %s", sLine.c_str());
						}
					}
				}
			}
		}
		// default arguments for Testdrive
		sArg	+= " -I$(TESTDRIVE_PROFILE)Common/System/SystemSim/HDL";
	}

	{
		// get verilator path
		sExe.Format("%sbin/msys64/ucrt64/bin", g_sTestDrivePath.c_str());
		sExe.SetEnvironment("VERILATOR_ROOT");
		sExe += "/verilator";
	}

	// make command
	sCmd.Format("perl \"%s\" %s", sExe.c_str(), sArg.c_str());
	return system(sCmd.c_str());
}
