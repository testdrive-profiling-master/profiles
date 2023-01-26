local	f 				= TextFile()
local	iVersionMajor	= 0
local	iVersionMinor	= 0
local	iBuildCount		= 0

-- get version number
f:Open("src\\code_gen_version.h")
while f:IsEOF() == false do
	local	s = String(f:Get(true))
	if s:CompareFront("#define") then
		s:CutFront("#define", false)
		local	def_name	= s:Tokenize(" \t\r\n").s
		local	iNumber		= tonumber(s:Tokenize(" \t\r\n").s)
		
		if def_name == "CODE_GEN_VERSION_MAJOR" then
			iVersionMajor	= iNumber
		elseif def_name == "CODE_GEN_VERSION_MINOR" then
			iVersionMinor	= iNumber
		elseif def_name == "CODE_GEN_BUILD_COUNT" then
			iBuildCount		= iNumber
		end
	end
end
f:Close()

-- save version number
f:Create("src\\code_gen_version.h")
f:Put("#define		CODE_GEN_VERSION_MAJOR		" .. tostring(iVersionMajor) .. "\n")
f:Put("#define		CODE_GEN_VERSION_MINOR		" .. tostring(iVersionMinor) .. "\n")
f:Put("#define		CODE_GEN_BUILD_COUNT		" .. tostring(iBuildCount + 1) .. "\n")
f:Close()