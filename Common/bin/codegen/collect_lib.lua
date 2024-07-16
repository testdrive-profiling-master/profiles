local	Arg		= ArgTable("Collect MinGW libraries on execution binary.")

Arg:AddOption		("forced", "f", nil, "forced to copy")
Arg:AddOptionFile	("exe_file", nil, nil, nil, "exe_file", "execution binary file")

if (Arg:DoParse() == false) then
	return
end

local	sBinFilename	= Arg:GetOptionFile("exe_file", 0)
local	sTargetPath		= String(sBinFilename)
local	sToolPath		= String()

sTargetPath:Replace("\\", "/", true)
if sTargetPath:find("/") >= 0 then
	sTargetPath:CutBack("/")
	sTargetPath	= sTargetPath.s
else
	sTargetPath	= "."
end

if sToolPath:GetEnvironment("TESTDRIVE_DIR") == false then
	LOGE("Run the testdrive first.")
	os.exit(1)
end

-- make library list
local	lib_list	= {}

function Collect_Lib(sBinPath)
	-- get ldd log
	local	sTxt		= String(exec("ldd " .. sBinPath .. " | grep ucrt64"))

	--print("sTxt : " .. sTxt.s)

	-- get reference libraries
	local	sTok		= sTxt:Tokenize("\r\n")
	while #sTok.s ~= 0 do
		sTok:CutBack("(", true)
		sTok:CutFront("=>", true)
		sTok:Trim(" \t")
		
		if sTok.s ~= 0 then
			if lib_list[sTok.s] ~= true then
				lib_list[sTok.s]	= true
				Collect_Lib(sToolPath.s .. "/bin/msys64/" .. sTok.s)
			end
		end
		
		sTok = sTxt:Tokenize("\r\n")
	end
end

LOGI("Check library dependencies...")
Collect_Lib(sBinFilename)

-- copy libraries
local	lib_count = 0
for k in pairs(lib_list) do
	local	sLibPath	= String(sToolPath.s .. "/bin/msys64/" .. k)
	sLibPath:Replace("//", "/", true)
	print(" - Copy library from : " .. sLibPath.s)
	run("cp -f \"" .. sLibPath.s .. "\" " .. sTargetPath)
	lib_count	= lib_count + 1
end

if lib_count == 0 then
	LOGW("No libraries copied. If you want ")
end

LOGI("Done!")
