local	Arg		= ArgTable("Collect MinGW libraries on execution binary.")

Arg:AddOptionFile	("exe_file", nil, nil, nil, "exe_file", "execution binary file")

if (Arg:DoParse() == false) then
	return
end

local	sBinFilename	= Arg:GetOptionFile("exe_file", 0)
local	sTargetPath		= String(sBinFilename)
local	sToolPath		= String()
local	sProfilePath	= String()

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

if sProfilePath:GetEnvironment("TESTDRIVE_PROFILE") == false then
	LOGE("Run your testdrive project once.")
	os.exit(1)
end

-- set libraries path
local	sLibPath	= String(sToolPath.s .. "/bin/msys64")
sLibPath:Replace("//", "/", true)
sLibPath = sLibPath.s

-- set common profiling bin path
local	sCommonPath	= String(sProfilePath.s .. "/common/bin/")
sCommonPath:Replace("//", "/", true)
sCommonPath = sCommonPath.s

-- make library list
local	lib_list	= {}

function IsExternalLibrary(sPath, sSearchPath)
	local sFileName = String(sPath)
	local sFilePath = String()
	
	sFileName:CutFront("/", true)
	sFileName:CutFront("\\", true)
	sFileName = sFileName.s
	
	sSearchPath.s = sLibPath .. sPath
	if lfs.IsExist(sSearchPath.s) then
		return true
	end
	
	sSearchPath.s = sCommonPath .. sFileName
	if lfs.IsExist(sSearchPath.s) then
		return true
	end
	
	return false
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
				if lfs.IsExist(sTargetPath .. "/" .. sName.s) == false then	-- not in current path
					local sSearchPath = String()
					
					if IsExternalLibrary(sPath.s, sSearchPath) then
						lib_list[sName.s]	= true
						print(" - Copy library from : " .. sSearchPath.s)
						run("cp -f \"" .. sSearchPath.s .. "\" " .. sTargetPath)
						lib_count	= lib_count + 1
						Collect_Lib(sLibPath .. sPath.s)
					end
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
