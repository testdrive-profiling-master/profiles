sProfilePath = String("")
if sProfilePath:GetEnvironment("TESTDRIVE_PROFILE") == false then
	LOGE("You must run the TestDrive once.")
	os.exit(1)
end
sProfilePath	= sProfilePath.s


local Arg = ArgTable("Template project generator with TestDrive Profiling Master.")

Arg:AddOptionString		("type", nil, nil, nil, "type", "Template project type")
Arg:AddRemark			(nil, "'td', 'testdrive'        : TestDrive project")
Arg:AddRemark			(nil, "'c', 'c++', 'cpp'        : C++(Pure) project")
Arg:AddRemark			(nil, "'util'                   : C++(Util) project")
Arg:AddRemark			(nil, "'v', 'verilog'           : verilog project")
Arg:AddRemark			(nil, "'v_bare', 'verilog_bare' : bared verilog project")
Arg:AddRemark			(nil, "'v_gen', 'verigen'       : verigen project")
Arg:AddOptionString		("project_name", nil, nil, nil, "project_name", "Project name")

	
if (Arg:DoParse() == false) then
	print("\n  ex) create_project cpp test")
	print("")
	print("  * To create a H/W(verilog, verigen...) project,")
	print("    you must run the TestDrive project at least once.")
	return
end

-- arguments retrieves
sType				= String(Arg:GetOptionString("type", 0))
sType:MakeLower()
sType				= sType.s
sProjectName		= String(Arg:GetOptionString("project_name", 0))
sProjectName:Replace(" ", "_", true)
sProjectName:Trim("_.#@$%^&*()~|\\")
sProjectName		= sProjectName.s

if #sProjectName == 0 then
	LOGE("Invalid project name.")
end


sProjectType		= sType

if (sProjectType == "c")     	then sProjectType = "cpp"			end
if (sProjectType == "c++")   	then sProjectType = "cpp"			end
if (sProjectType == "v")     	then sProjectType = "verilog"		end
if (sProjectType == "v_bare")   then sProjectType = "verilog_bare"	end


function MakeDir(folder_name)
	local attr = lfs.attributes(folder_name)
	
	if attr ~= nil then
		LOGE("Can't create project, '" .. folder_name .. "' " .. attr.mode .. " is already existed. Please specify a different project name.")
		os.exit(1)
	end
	
	if lfs.mkdir(folder_name) == false then
		LOGE("Can't create project directory '" .. folder_name .. "'")
		os.exit(1)
	end
	
	return folder_name .. "/"
end

function MakeDirForTestDrive(folder_name)
	local folder_path = sProfilePath .. folder_name

	local attr = lfs.attributes(folder_path)
	
	if attr ~= nil then
		LOGE("Can't create project, '" .. folder_name .. "'(" .. folder_path .. ") " .. attr.mode .. " is already existed. Please specify a different project name.")
		os.exit(1)
	end
	
	if lfs.mkdir(folder_path) == false then
		LOGE("Can't create project directory '" .. folder_name .. "'(" .. folder_path .. ")")
		os.exit(1)
	end
	
	return folder_path
end

function MakeDirForVerilog(folder_name)
	local folder_path = String("")
	if folder_path:GetEnvironment("RECENT_PROJECT@TESTDRIVE") == false then
		LOGE("You must execute your 'TestDrive' project at least once.")
		os.exit(1)
	end
	folder_path	= folder_path.s .. "System\\HDL\\DUTs\\" .. folder_name

	local attr = lfs.attributes(folder_path)
	
	if attr ~= nil then
		LOGE("Can't create project, '" .. folder_name .. "'(" .. folder_path .. ") " .. attr.mode .. " is already existed. Please specify a different project name.")
		os.exit(1)
	end
	
	if lfs.mkdir(folder_path) == false then
		LOGE("Can't create project directory '" .. folder_name .. "'(" .. folder_path .. ")")
		os.exit(1)
	end
	
	return folder_path
end


local sProjectPath = ""

if (sType == "td" or sType == "testdrive") then
	local sProjectNameUp	= String(sProjectName)
	sProjectPath			= MakeDirForTestDrive(sProjectName)	-- make folder
	
	local	sYear = String(nil)
	sYear:FormatDate("%Y", 0)	-- get year
	
	sProjectNameUp:MakeUpper()
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_testdrive/.\" " .. sProjectPath .. "/")
	
	os.execute("mv \"" .. sProjectPath .. "/Application/include/SystemConfigPROJECT.h\" \"" .. sProjectPath .. "/Application/include/SystemConfig" .. sProjectNameUp.s .. ".h\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/project.profile\"")
	os.execute("sed \"s/PROJECT/" .. sProjectNameUp.s .. "/g\" -i \"" .. sProjectPath .. "/Application/include/SystemConfig" .. sProjectNameUp.s .. ".h\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/Profiles/code_inception.txt\"")
	os.execute("sed \"s/INITIAL_YEAR/" .. sYear.s .. "/g\" -i \"" .. sProjectPath .. "/Profiles/code_inception.txt\"")
	
	-- open folder
	print("\nRun 'project.profile' to open your project.")
	sProjectPath	= String(sProjectPath)
	sProjectPath:Replace("/", "\\", true)
	os.execute("explorer " .. sProjectPath.s)
	os.execute(sProjectPath.s .."/Profiles/code_inception.txt")
	os.exit(0)
elseif (sType == "c" or sType == "c++" or sType == "cpp") then
	sProjectPath	= MakeDir(sProjectName)
	LOGI("Create C++ project : '" .. sProjectName .. "'")
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_cpp/.\" " .. sProjectName .. "/")
	os.execute("sed \"s/Test/" .. sProjectName .. "/\" -i ./" .. sProjectName .. "/.project")
	os.execute("sed \"s/Test/" .. sProjectName .. "/\" -i ./" .. sProjectName .. "/Makefile")
	os.execute("sed \"s/Test/" .. sProjectName .. " project/\" -i ./" .. sProjectName .. "/.inception")
	
	os.execute("explorer " .. sProjectName)
elseif (sType == "util") then
	sProjectPath	= MakeDir(sProjectName)
	LOGI("Create C++(Util) project : '" .. sProjectName .. "'")
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_cpp_util/.\" " .. sProjectName .. "/")
	os.execute("sed \"s/Test/" .. sProjectName .. "/\" -i ./" .. sProjectName .. "/.project")
	os.execute("sed \"s/Test/" .. sProjectName .. "/\" -i ./" .. sProjectName .. "/Makefile")
	os.execute("sed \"s/Test/" .. sProjectName .. " project/\" -i ./" .. sProjectName .. "/.inception")
	
	os.execute("explorer " .. sProjectName)
elseif (sType == "v" or sType == "verilog") then
	sProjectPath	= MakeDirForVerilog(sProjectName)
	LOGI("Create Verilog project : '" .. sProjectName .. "'(" .. sProjectPath .. ")")
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_verilog/.\" \"" .. sProjectPath .. "/\"")
	os.execute("mv \"" .. sProjectPath .. "/dpi_private/.src.cpp\" \"" .. sProjectPath .. "/dpi_private/" .. sProjectName .. ".cpp\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. "/\" -i \"" .. sProjectPath .. "/.project\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. " project/\" -i \"" .. sProjectPath .. "/.inception\"")
	
	os.execute("explorer " .. sProjectPath)
elseif (sType == "v_bare" or sType == "verilog_bare") then
	sProjectPath	= MakeDirForVerilog(sProjectName)
	LOGI("Create Bared-Verilog project : '" .. sProjectName .. "'(" .. sProjectPath .. ")")

	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_verilog_bare/.\" \"" .. sProjectPath .. "/\"")
	os.execute("mv \"" .. sProjectPath .. "/dpi_private/.src.cpp\" \"" .. sProjectPath .. "/dpi_private/" .. sProjectName .. ".cpp\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. "/\" -i \"" .. sProjectPath .. "/.project\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. " project/\" -i \"" .. sProjectPath .. "/.inception\"")

	os.execute("explorer " .. sProjectPath)
elseif (sType == "v_gen" or sType == "verigen") then
	if sProjectName == "common" then
		LOGE("Invalid project name for verigen : " .. sProjectName)
		os.exit(1)
	end

	sProjectPath	= MakeDirForVerilog(sProjectName)
	LOGI("Create Verigen project : '" .. sProjectName .. "'(" .. sProjectPath .. ")")
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_verigen/.\" \"" .. sProjectPath .. "/\"")
	os.execute("mv \"" .. sProjectPath .. "/dpi_private/.src.cpp\" \"" .. sProjectPath .. "/dpi_private/" .. sProjectName .. ".cpp\"")
	os.execute("mv \"" .. sProjectPath .. "/verigen_src/defines_PROJECT.lua\" \"" .. sProjectPath .. "/verigen_src/defines_" .. sProjectName .. ".lua\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. "/\" -i \"" .. sProjectPath .. "/.project\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. " project/\" -i \"" .. sProjectPath .. "/.inception\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/.verilator\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/verigen_src/main.lua\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/verigen_src/design_hierarchy.lua\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/verigen_src/Makefile\"")
	os.execute("sed \"s/PROJECT/" .. sProjectName .. "/g\" -i \"" .. sProjectPath .. "/verigen_src/defines_" .. sProjectName .. ".lua\"")
	
	os.execute("explorer " .. sProjectPath)
else
	LOGE("Invalid project type : '" .. sType .. "'. Please refer 'help' with \"create_project --help\" command.")
	os.exit(1)
end

do	-- delete previous eclipse workspace to avoid conflict
	lfs.chdir(sProjectPath)
	sProjectPath	= lfs.currentdir()
	local sWorkspacePath = String(sProjectPath)
	
	while(sWorkspacePath:find("\\", 0) > 0) do
		sWorkspacePath:CutBack("\\")
		
		if lfs.attributes(sWorkspacePath.s .. "/.workspace/") ~= nil then
			local sFindPath = String(sProjectPath)
			sFindPath:erase(0, sWorkspacePath:Length())
			sWorkspacePath	= sWorkspacePath.s .. "/.workspace" .. sFindPath.s

			if lfs.attributes(sWorkspacePath) ~= nil then
				os.execute("rm -rf \"" .. sWorkspacePath .. "\"")
			end
			break
		end
	end
end

print("\nRun '.eclipse' to open your project's IDE environment.")