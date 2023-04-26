local	Arg				= ArgTable("Verilog Generator for TestDrive Profiling Master. v1.00")
local	sProfilePath	= String(nil)

sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")

Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua file")
Arg:AddOptionFile	("out_path", "./output", nil, nil, "output_path", "output path")

if (Arg:DoParse() == false) then
	return
end

local	sInFilename		= Arg:GetOptionFile("in_file", 0)
local	sOutPath		= Arg:GetOptionFile("out_path", 0)

LOGI("sInFilename : " .. sInFilename)
LOGI("sOutPath : " .. sOutPath)
