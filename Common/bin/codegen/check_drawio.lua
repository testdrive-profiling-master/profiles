-- check installation of draw.io
local	sProfilePath		= String()
sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")
sProfilePath:Append("common/bin/")

-- version update check
local bUpdateInstall = false
do
	local sVersion = String(exec("wget --max-redirect=0 https://github.com/jgraph/drawio-desktop/releases/latest 2>&1 | grep Location:"))
	if sVersion:CompareFront("Location:") then
		sVersion:CutBack("[", true)
		sVersion:CutFront("/", true)
		sVersion:Trim(" \t\r\n")
		
		local sPrevVersion = String()
		sPrevVersion:GetEnvironment("VERSION@DRAWIO")
		
		if sPrevVersion.s ~= sVersion.s then
			LOGI("Update new version of Draw.io(" .. sVersion.s .. ")...")
			sVersion:SetEnvironment("VERSION@DRAWIO")
			--lfs.rmdir(sProfilePath.s .. "drawio/")
			os.execute("rm -rf \"" .. sProfilePath.s .. "drawio/\"")
			bUpdateInstall = true
		end
	end
end

-- program existance check
if lfs.IsExist(sProfilePath.s .. "drawio/draw.io.exe") == false then
	if bUpdateInstall == false then
		LOGI("Can't found draw.io, now installing draw.io...")
	end

	local sCurDir = lfs.currentdir()
	lfs.chdir(sProfilePath.s)
	exec("wget https://github.com/jgraph/drawio-desktop/releases/latest -O github.download.drawio.html")

	local f = TextFile()
	if f:Open("github.download.drawio.html") then
		local s = String(f:GetAll())
		f:Close()
		exec("rm -f github.download.drawio.html")
		-- get download link
		s:CutBack("-windows.zip\"", true)
		s:CutFront("href=\"", true)
		s:Append("-windows.zip")
		
		LOGI("Downloading draw.io... Please wait...")
		if s:CompareFront("https://") then
			exec("wget " .. s.s .. " -O \"" .. sProfilePath.s .. "drawio.zip\"")
		else
			LOGE("Unexpected download link.")
			os.exit(1)
		end
		
		LOGI("Extracting draw.io binaries...")
		exec("unzip \"" .. sProfilePath.s .. "drawio.zip\" -d \"" .. sProfilePath.s .. "drawio\"")
		exec("rm -f \"" .. sProfilePath.s .. "drawio.zip\"")
	else
		LOGE("Can't access to github repo!")
		os.exit(1)
	end
	lfs.chdir(sCurDir)
	LOGI("Done!")
end