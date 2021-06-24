#include "Common.h"

int main(int argc, const char* argv[])
{
	cstring sTestDrivePath;
	cstring	sArg;	// argument path
	cstring	sExe;	// verilator path
	cstring	sCmd;	// modified command line

	if(!sTestDrivePath.GetEnvironment("TESTDRIVE_DIR")) {
		LOGE("You must run TestDrive Profiling Master first.");
		return 1;
	}
	// make arguments
	if (argc != 1) {
		// get given arguments
		for (int i = 1; i<argc; i++) {
			sArg.AppendFormat(" %s", argv[i]);
		}
		// get extra arguments from file description
		{
			cstring	sPath;
			if (sPath.GetEnvironment("SIM_TOP_FILE")) {
				int		pos;
				// delete '"'
				sPath.Replace("\"", "", true);
				// change linux style path to windows
				sPath.Replace("/", "\\", true);

				if ((pos = sPath.rfind("\\")) > 0) {
					sPath.erase(pos, -1);
					sPath += "\\.verilator";
					TextFile	f;

					if (f.Open(sPath.c_str())) {
						cstring sLine;
						while(f.GetLine(sLine)){
							sLine.Trim(" \r\n");
							if(sLine.size())
								sArg.AppendFormat(" %s", sLine.c_str());
						}
					}
				}
			}
		}
	}
	{	// get verilator path
		sExe.Format("%sbin/msys64/mingw64/bin", sTestDrivePath.c_str());
		sExe.SetEnvironment("VERILATOR_ROOT");
		sExe += "/verilator";
	}

	// make command
	sCmd.Format("perl \"%s\" %s", sExe.c_str(), sArg.c_str());

	return system(sCmd.c_str());
}
