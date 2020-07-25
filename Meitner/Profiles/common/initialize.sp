profile.tree "System" {
	branch("Build"){
		profile ("Build All",					"build_all.sp");
		profile ("Build S/W",					"build_sw.sp");
		profile ("Build H/W",					"build_hw.sp");
		profile ("View current DPI functions (Need to run 'build H/W')",	"view_dpi_functions.sp");
		expand();
	}
	
	screen ("View wave-form",				"view_sim_wave.bat");
	profile ("View system memory",			"view_system_memory.sp");
}
