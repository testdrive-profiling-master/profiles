profile.tree "System" {
	branch("Build"){
		profile ("Build All",					"common\\build_all.sp");
		profile ("Build S/W",					"common\\build_sw.sp");
		profile ("Build H/W",					"common\\build_hw.sp");
		profile ("Bake H/W model",				"common\\bake_hw.sp");
		profile ("View current DPI functions (Need to run 'build H/W')",	"common\\view_dpi_functions.sp");
		expand();
	}
	profile ("MinGW Shell",				"%TESTDRIVE_DIR%bin\\msys64\\ucrt64.exe");
	screen ("View wave-form",			"common\\view_sim_wave.bat");
	profile ("View system memory",		"common\\view_system_memory.sp");
}

profile.tree "Community" {
	network ("TestDrive Wiki",			"Community\\testdrive_wiki.sp");
}

profile.call		"Community\\testdrive_wiki.sp"

if.file "%PROJECT%Profiles\\common\\initialize.sp"
profile.call		"%PROJECT%Profiles\\common\\initialize.sp"
if.end
