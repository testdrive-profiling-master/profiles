local	Arg		= ArgTable("Collect MinGW libraries on execution binary.")

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

-- set libraries path
local	sLibPath	= String(sToolPath.s .. "/bin/msys64")
sLibPath:Replace("//", "/", true)

-- make library list
local	lib_list	= {}

function IsMinGWLibrary(sPath)
	return lfs.IsExist(sLibPath.s .. sPath)
end

local	lib_count	= 0
function Collect_Lib(sBinPath)
	-- get ldd log
	local	sTxt		= String(exec("ldd " .. sBinPath))

	-- get reference libraries
	local	sName		= sTxt:Tokenize("\n")
	while #sName.s ~= 0 do
		local sPath		= String(sName.s)
		sName:CutBack("=>", true)
		sName:Trim(" \t")
		sPath:CutFront("=>", true)
		sPath:CutBack("(", true)
		sPath:Trim(" \t")

		if sName.s ~= 0 then
			if lib_list[sName.s] ~= true then
				if IsMinGWLibrary(sPath.s) then
					lib_list[sName.s]	= true
					print(" - Copy library from : " .. sName.s)
					run("cp -f \"" .. sLibPath.s .. sPath.s .. "\" " .. sTargetPath)
					lib_count	= lib_count + 1
					Collect_Lib(sLibPath.s .. sPath.s)
				end
			end
		end
		
		sName = sTxt:Tokenize("\r\n")
	end
end

LOGI("Check library dependencies...")
Collect_Lib(sBinFilename)

if lib_count == 0 then
	LOGI("No more MinGW library is required for this.")
end

LOGI("Done!")
