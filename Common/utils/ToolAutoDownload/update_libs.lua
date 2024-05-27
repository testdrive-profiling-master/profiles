local	log			= String(exec("ldd ToolAutoDownload.exe"))
local	sTok		= log:Tokenize("\r\n")
local	f			= TextFile()
local	sToolPath	= String("")

sToolPath:GetEnvironment("TESTDRIVE_DIR")
sToolPath	= sToolPath.s .. "bin/"

local	sMsysPath	= sToolPath .. "msys64/ucrt64/bin/"

if f:Create("update_libs_private.bat") then
	f:Put("@echo off\n")

	while sTok:IsEmpty() == false do
		if sTok:find("/ucrt64/bin/", 0) > 0 then
			-- get to filename
			sTok:CutBack("=>", true)
			sTok:Trim(" \t")
			
			f:Put("cp -f \"" .. sMsysPath .. sTok.s .. "\" \"" .. sToolPath .. "\"\n")
		end
		sTok = log:Tokenize("\r\n")
	end
	
	f:Put("cp -f ToolAutoDownload.exe \"" .. sToolPath .. "\"\n")
	
	f:Close()
else
	LOGE("Can't create update_lib.bat")
	os.exit(1)
end