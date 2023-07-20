local Arg = ArgTable("Version file generator.")

Arg:AddOption		("MAJOR", "m", "major", "count up major version")
Arg:AddOption		("MINOR", "n", "minor", "count up minor version")
Arg:AddOptionString	("PREFIX", "", "p", "prefix", "prefix", "Set prefix string")
Arg:AddOptionFile	("version_file", nil, nil, nil, "version_file", "version file")
--Arg:AddRemark		(nil, "default : 'version.inl'")
if (Arg:DoParse() == false) then
	return
end

-- parameters
local	bUpMajor		= Arg:GetOption("MAJOR")
local	bUpMinor		= Arg:GetOption("MINOR")
local	sPREFIX			= Arg:GetOptionString("PREFIX")
local	sFileName		= Arg:GetOptionFile("version_file", 0)

if sFileName == nil then
	sFileName	= "version.inl"
end

if #sPREFIX ~= 0 then
	sPREFIX	= sPREFIX .. "_"
end

-- 
local	f 				= TextFile()
iVersionMajor	= 0
iVersionMinor	= 0
iBuildCount		= 0

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
			elseif def_name == (sPREFIX .. "VERSION_BUILD") then
				iBuildCount		= iNumber
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
	iBuildCount		= iBuildCount + 1
end

LOGI("Current Version : " .. tostring(iVersionMajor) .. "." .. tostring(iVersionMinor) .. "(build : " .. tostring(iBuildCount) .. ")")
-- save version number
f:Create(sFileName)
f:Put("#define		" .. sPREFIX .. "VERSION_MAJOR		" .. tostring(iVersionMajor) .. "\n")
f:Put("#define		" .. sPREFIX .. "VERSION_MINOR		" .. tostring(iVersionMinor) .. "\n")
f:Put("#define		" .. sPREFIX .. "VERSION_BUILD		" .. tostring(iBuildCount) .. "\n")
f:Close()
