#lua
do
	local system_tree = ProfileTree("System")
	local build_tree = ProfileTree("Build", system_tree)
	local use_hw_build = (lfs.attributes(System.ProjectPath .. "System\\HDL\\", "mode") == "directory")
	build_tree:AddItem("profile",	"Build All",					"common\\build_all.sp")
	build_tree:AddItem("profile",	"Build S/W",					"common\\build_sw.sp")
	
	if use_hw_build then
		build_tree:AddItem("profile",	"Build H/W",					"common\\build_hw.sp")
		build_tree:AddItem("profile",	"Bake H/W model",				"common\\bake_hw.sp")
		build_tree:AddItem("profile",	"View current DPI functions (Need to run 'build H/W')",	"common\\view_dpi_functions.sp")
	end
	build_tree:Expand()
	
	system_tree:AddItem("profile",	"MinGW Shell",					"%TESTDRIVE_DIR%bin\\msys64\\ucrt64.exe")
	if use_hw_build then
		system_tree:AddItem("screen",	"View wave-form",				"common\\view_sim_wave.bat")
	end
	system_tree:AddItem("profile",	"View system memory",			"common\\view_system_memory.sp")
end

do
	local community_tree = ProfileTree("Community")
	community_tree:AddItem("network", "TestDrive Wiki", "Community\\testdrive_wiki.sp")
	System.CallProfile("Community\\testdrive_wiki.sp")
end

-- Do user specific initialization
if lfs.attributes(System.ProjectPath .. "Profiles\\common\\initialize.sp", "mode") == "file" then
	System.CallProfile("%PROJECT%Profiles\\common\\initialize.sp")
end
