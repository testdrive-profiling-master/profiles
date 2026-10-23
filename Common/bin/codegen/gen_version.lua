local Arg = ArgTable("Version file generator.")

Arg:AddOption		("MAJOR", "m", "major", "count up major version")
Arg:AddOption		("MINOR", "n", "minor", "count up minor version")
Arg:AddOption		("GIT_TAG", "t", "git_tag", "follow latest git tag version")
Arg:AddRemark		(nil, "MAJOR/MINOR option will be ignored.")
Arg:AddOptionString	("PREFIX", "", "p", "prefix", "prefix", "Set prefix string")
Arg:AddOptionFile	("version_file", nil, nil, nil, "version_file", "version file")

if (Arg:DoParse() == false) then
	return
end

-- parameters
local	bUpMajor		= Arg:GetOption("MAJOR")
local	bUpMinor		= Arg:GetOption("MINOR")
local	bGitTag			= Arg:GetOption("GIT_TAG")
local	sPREFIX			= Arg:GetOptionString("PREFIX")
local	sFileName		= Arg:GetOptionFile("version_file", 0)

if sFileName == nil then
	sFileName	= "version.inl"
end

if #sPREFIX ~= 0 then
	 sPREFIX	= String(sPREFIX)
	 sPREFIX:MakeUpper()
	sPREFIX		= sPREFIX.s .. "_"
end

-- 
local	f 		= TextFile()
iVersionMajor	= 0
iVersionMinor	= 1
iVersionPatch	= 0
sDATE			= os.date("%d %B %Y")

local sVersionPatch = "VERSION_PATCH"

-- get version number
if f:Open(sFileName) then
	while f:IsEOF() == false do
		local	s = String(f:Get(true))
		if s:CompareFront("#define") then
			s:CutFront("#define", false)
			local	def_name	= s:Tokenize(" \t\r\n").s
			local	iNumber		= tonumber(s:Tokenize(" \t\r\n").s)
			
			if def_name == (sPREFIX .. "VERSION_MAJOR") then
				iVersionMajor	= iNumber
			elseif def_name == (sPREFIX .. "VERSION_MINOR") then
				iVersionMinor	= iNumber
			elseif def_name == (sPREFIX .. "VERSION_PATCH") then
				iVersionPatch	= iNumber
			elseif def_name == (sPREFIX .. "VERSION_BUILD") then
				iVersionPatch	= iNumber
				sVersionPatch	= "VERSION_BUILD"
			end
		end
	end
	f:Close()
else
	LOGI("Start Initial.")
end

if bUpMajor then
	iVersionMajor	= iVersionMajor + 1
	iVersionMinor	= 0
elseif bUpMinor then
	iVersionMinor	= iVersionMinor + 1
else
	iVersionPatch	= iVersionPatch + 1
end

if bGitTag then
	local sTag = String(exec("git log -1 --format=%cd --date=format:\"%d %B %Y\""))
	sTag:Trim(" \t\r\n")
	sDATE = sTag.s
	sTag.s = exec("git describe --tags --abbrev=0")
	sTag:Trim(" \t\r\n")
	
	if sTag:CompareFront("fatal:") == false then
		sTag:MakeLower()
		local sDelim	= " abcdefghijklmnopqrstuvwxyz#@()[]{}./\\\t\r\n"
		iVersionMajor	= tonumber(sTag:Tokenize(sDelim).s)
		iVersionMinor	= tonumber(sTag:Tokenize(sDelim).s)
		iVersionPatch	= tonumber(sTag:Tokenize(sDelim).s)
	end
	LOGI("Git TAG(" .. sTag.s .. ") applied : v" .. tostring(iVersionMajor) .. "." .. tostring(iVersionMinor) .. "." .. tostring(iVersionPatch))
else
	LOGI("Version generated : v" .. tostring(iVersionMajor) .. "." .. tostring(iVersionMinor) .. "." .. tostring(iVersionPatch))
end

-- save version number
f:Create(sFileName)
f:Put("#define		" .. sPREFIX .. "VERSION_MAJOR		" .. tostring(iVersionMajor) .. "\n")
f:Put("#define		" .. sPREFIX .. "VERSION_MINOR		" .. tostring(iVersionMinor) .. "\n")
f:Put("#define		" .. sPREFIX .. sVersionPatch .. "		" .. tostring(iVersionPatch) .. "\n")
f:Put("#define		" .. sPREFIX .. "VERSION_DATE		\"" .. sDATE .. "\"\n")
f:Close()
