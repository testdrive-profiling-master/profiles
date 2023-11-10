sProfilePath = String("")
if sProfilePath:GetEnvironment("TESTDRIVE_PROFILE") == false then
	LOGE("You must run the TestDrive once.")
	os.exit(1)
end
sProfilePath	= sProfilePath.s


local Arg = ArgTable("Template project generator with TestDrive Profiling Master.")

Arg:AddOptionString		("type", nil, nil, nil, "type", "Template project type")
Arg:AddRemark			(nil, "'c', 'c++', 'cpp'        : C++ project")
Arg:AddRemark			(nil, "'v', 'verilog'           : verilog project")
Arg:AddRemark			(nil, "'v_bare', 'verilog_bare' : bared verilog project")
Arg:AddRemark			(nil, "'v_gen', 'verigen'       : verigen project")
Arg:AddOptionString		("project_name", nil, nil, nil, "project_name", "Project name")

	
if (Arg:DoParse() == false) then
	print("\n  ex) create_project cpp test")
	print("")
	print("  ('Verilog' and 'Verigen' H/W projects are required at least")
	print("   one TestDrive project run!)")
	return
end

-- arguments retrieves
sType				= String(Arg:GetOptionString("type", 0))
sType:MakeLower()
sType				= sType.s
sProjectName		= String(Arg:GetOptionString("project_name", 0))
sProjectName:Replace(" ", "_", true)
sProjectName:Trim("_")
sProjectName		= sProjectName.s


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


if (sType == "c" or sType == "c++" or sType == "cpp") then
	MakeDir(sProjectName)
	LOGI("Create C++ project : '" .. sProjectName .. "'")
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_cpp/.\" " .. sProjectName .. "/")
	os.execute("sed \"s/Test/" .. sProjectName .. "/\" -i ./" .. sProjectName .. "/.project")
	os.execute("sed \"s/Test/" .. sProjectName .. "/\" -i ./" .. sProjectName .. "/Makefile")
	os.execute("sed \"s/Test/" .. sProjectName .. " project/\" -i ./" .. sProjectName .. "/.inception")
elseif (sType == "v" or sType == "verilog") then
	local sProjectPath	= MakeDirForVerilog(sProjectName)
	LOGI("Create Verilog project : '" .. sProjectName .. "'(" .. sProjectPath .. ")")
	
	os.execute("cp -rf \"" .. sProfilePath .. "Common/bin/project_template_verilog/.\" \"" .. sProjectPath .. "/\"")
	os.execute("mv \"" .. sProjectPath .. "/dpi_private/.src.cpp\" \"" .. sProjectPath .. "/dpi_private/" .. sProjectName .. ".cpp\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. "/\" -i \"" .. sProjectPath .. "/.project\"")
	os.execute("sed \"s/HDL/" .. sProjectName .. " project/\" -i \"" .. sProjectPath .. "/.inception\"")
	
	os.execute("explorer " .. sProjectPath)
elseif (sType == "v_bare" or sType == "verilog_bare") then
	local sProjectPath	= MakeDirForVerilog(sProjectName)
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

	local sProjectPath	= MakeDirForVerilog(sProjectName)
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
end
