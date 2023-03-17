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
bool		g_bUseBakedModel	= false;

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

uint64_t MakeHash(const char* s)
{
	uint64_t hash = 2166136261;

	for (; *s; s++)
		hash = 16777619 * (hash ^ (*s));

	return hash ^ (hash >> 32);
}

bool MakeTargetName(cstring& sTargetName) {
	cstring		sTopFile;
	cstring		sWaveMode;
	cstring		sDefinition;

	sTopFile.GetEnvironment("SIM_TOP_FILE");
	if(sTopFile.IsEmpty()) return false;
	sWaveMode.GetEnvironment("SIM_WAVE_MODE");
	if(sWaveMode.IsEmpty()) return false;
	sDefinition.GetEnvironment("SIM_DEFINITION");
	if(sDefinition.IsEmpty()) return false;

	cstring sTotal;
	sTotal.Format("TopFile : %s, WaveModel : %s, Definition : %s", sTopFile.c_str(), sWaveMode.c_str(), sDefinition.c_str());
	uint64_t	ulHashCode	= MakeHash(sTotal);

	sTopFile.CutFront("\\", true);
	sTopFile.CutFront("/", true);
	sTopFile.CutBack(".", true);
	sTopFile.Replace(" ", "_", true);

	sTargetName.Format("%s_%c_%016llx", sTopFile.c_str(), sWaveMode[0], ulHashCode);

	return true;
}

int BakeModel(const char* sName)
{
	cstring sTargetName;
	if(sName) {
		sTargetName		= sName;
	} else if(!MakeTargetName(sTargetName)){
		LOGE("Invalid operation.");
		return 1;
	}

	cstring	sLatestMdir	= GetEnvString("LATEST_MDIR");
	cstring sTargetPath	= "./.bake/";

	if(sLatestMdir.IsEmpty()) {
		LOGE("No recent build result.");
		return 1;
	}

	{
		// target is a valid directory?
		auto target_path	= fs::path(sTargetPath.c_str());

		// not exist? then create
		if(!fs::exists(target_path)) {
			fs::create_directories(target_path);
			{	// add nosearch directive
				cstring	sNosearchPath(sTargetPath.c_str());
				sNosearchPath	+= "/.TestDrive.nosearch";
				target_path	= fs::path(sNosearchPath.c_str());

				if(!fs::exists(target_path)) {
					ofstream ofs(target_path);
					ofs.close();
				}
			}
		}

		if(!fs::exists(target_path) || !fs::is_directory(target_path)) {
			LOGE("bake path is not a directory. : '%s'\n", target_path.string().c_str());
			return 1;
		}
	}

	{
		// remove all simulation results.
		cstring sCmd;
		sLatestMdir.Replace("\\", "/", true);
		sCmd.Format("rm -f %s%s.tar.gz >nul 2>&1", sTargetPath.c_str(), sTargetName.c_str());
		system(sCmd);
		sCmd.Format("tar cvfz %s%s.tar.gz -C %s *.cpp *.h *.mk *.dat >nul 2>&1", sTargetPath.c_str(), sTargetName.c_str(), sLatestMdir.c_str());
		system(sCmd);

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

	{	// check baked model or not
		cstring	sBakedModel;
		if(sBakedModel.GetEnvironment("SIM_BAKED_MODEL")) {
			g_bUseBakedModel	= atoi(sBakedModel.c_str());
		}
	}


	if(argc >= 2 && !strcmp(argv[1], "bake")) {	// bake copy
		if(argc > 3) {
			LOGE("Invalid arguments...");
			return 1;
		}
		if(g_bUseBakedModel) {
			LOGE("Already baked model is activated.");
			return 1;
		}
		return BakeModel((argc == 3) ? argv[2] : NULL);
	}

	{
		// check pre-compiled repo.
		cstring	sDst;
		cstring sBakePath;
		{
			for(int i = 1; i < argc; i++) {
				cstring sArg = argv[i];

				if(sBakePath.IsEmpty() && !sArg.CompareFront("-")) {
					sBakePath	= argv[i];
					sBakePath.Replace("\\", "/", true);
					sBakePath.CutBack("/");
					sBakePath	+= "/.bake/";
				}

				if(sArg == "-Mdir" && (i + 1) < argc) {
					i++;
					sDst	= argv[i];
					sDst.Trim(" \"");
					// update latest target path
					SetEnvString("LATEST_MDIR", fs::absolute(fs::path(sDst.c_str())).string().c_str());
				}
			}

			if(g_bUseBakedModel){	// source .bake path check
				cstring sBakeName;

				if(sBakePath.IsEmpty()) {
					LOGE("Can't find repo. path.", sBakePath.c_str());
					return 1;
				}

				if(!MakeTargetName(sBakeName))
					return 1;

				sBakePath	+= sBakeName;
				sBakePath	+= ".tar.gz";
				auto repo_path	= fs::path(sBakePath.c_str());

				if(!fs::exists(repo_path) || !fs::is_regular_file(repo_path)) {
					LOGE("Can't access source repo. path : '%s'\n", sBakePath.c_str());
					return 1;
				}

				if(sDst.IsEmpty()) sDst = "./";
				else {
					sDst	+= '/';
				}

				// extract
				{
					auto src_path = fs::absolute(fs::path(sBakePath.c_str()));
					cstring sCmd;
					LOGI("Extract from baked model '%s'", sBakeName.c_str());
					sCmd.Format("tar xvf %s -C %s >nul 2>&1", sBakePath.c_str(), sDst.c_str());
					system(sCmd);
				}
				return 0;
			}
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
