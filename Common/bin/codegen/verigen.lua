local	Arg				= ArgTable("Verilog Generator for TestDrive Profiling Master. v1.00")
local	sProfilePath	= String(nil)

sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")

Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua file")
Arg:AddOptionFile	("out_path", "./output", nil, nil, "output_path", "output path")
Arg:AddRemark(nil, "default : ./output")

if (Arg:DoParse() == false) then
	return
end

local	sInFilename				= Arg:GetOptionFile("in_file", 0)
local	sOutPath				= Arg:GetOptionFile("out_path", 0)

-- 출력 폴더 생성
if lfs.attributes(sOutPath, "mode") ~= "directory" then
	lfs.mkdir(sOutPath)
	if lfs.attributes(sOutPath, "mode") ~= "directory" then
		LOGE("Output path can't access : '" .. sOutPath .. "'")
		os.exit(1)
	end
end

-- top, interface, config 선언
top			= {}
interface	= {}
config		= {}

interface.Create = function(name, decl)
end

top.CreateModule = function(name)
end

top.Parameter = function(decl)
end

-- 사용자 소스 실행
if RunScript(sInFilename) == false then
	return
end

-- top design 선언 여부 판단
do
	if top.name == nil then
		LOGE("Top design name is not declared.")
		os.exit(1)
	end
	
	local name	= String(top.name)
	name:MakeLower()
	name:Replace(" ", "_", true)
	name:Replace(".", "_", true)
	name:Replace("__", "_", true)
	name:Trim("_")
	top.name		= name.s
	name:MakeUpper()
	top.name_ucase	= name.s
	
	LOGI("Top design : " .. top.name)
end

-- configuration 생성
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. top.name_ucase .. "_config.vh") == false then
		LOGE("Can't create configuration file.")
		os.exit(1)
	end
	f:Put(	"`ifndef __" .. top.name_ucase .. "_CONFIG_VH__\n"..
			"`define __" .. top.name_ucase .. "_CONFIG_VH__\n\n")
			
	f:Put(	"`endif//__" .. top.name_ucase .. "_DEFINES_VH__\n")
end

-- defines 생성
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. top.name_ucase .. "_defines.vh") == false then
		LOGE("Can't create configuration file.")
		os.exit(1)
	end

	f:Put(	"`ifndef __" .. top.name_ucase .. "_DEFINES_VH__\n"..
			"`define __" .. top.name_ucase .. "_DEFINES_VH__\n"..
			"`include \"testdrive_system.vh\"		// default system defines\n"..
			"`include \"" .. top.name_ucase .. "_config.vh\"		// current system configurations\n\n")
			
	f:Put(	"`endif//__" .. top.name_ucase .. "_DEFINES_VH__\n")
end

-- top design 생성
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. top.name .. ".sv") == false then
		LOGE("Can't create top design file.")
		os.exit(1)
	end
	f:Put(	"`include \"" .. top.name_ucase .. "_defines.vh\"\n\n")
	
	f:Put(	"module " .. top.name)
	
	if top.params ~= nil then
		f:Put(	"#(\n")
		f:Put(	") ")
	end
	
	-- ports
	f:Put(	" (\n")
	f:Put(	");\n\n")
	f:Put(	"endmodule\n")
end

-- constraint 파일 생성
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. top.name .. "_constraint.xdc") == false then
		LOGE("Can't create constraint file.")
		os.exit(1)
	end
end