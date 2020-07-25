profile.tree "System\\Build" {
	branch("Driver"){
		branch("Windows (WDM)"){
			profile ("Build Auto (Debug)",			"windows\\src\\build_auto_chk.sp");
			profile ("Build All (Debug)",			"windows\\src\\build_all_chk.sp");
			profile ("Build All (Release)",			"windows\\src\\build_all_fre.sp");
			report ("DebugView",					"%TESTDRIVE_PROFILE%Common\\WDK\\tools\\DebugView\\Dbgview.exe");
			report ("Device management",			"windows\\src\\devmanagement.bat");
			report ("Open project",					"windows\\vs_project\\driver.sln");
			expand();
		}
	}
}
