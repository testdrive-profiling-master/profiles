local	sProjectPath	= String("")
sProjectPath:GetEnvironment("PROJECT")

-- prevent running by user
if sProjectPath:Length() == 0 then
	LOGE("This program can run under TestDrive execution.")
	os.exit(1)
end

local	Arg				= ArgTable("GTKWave .do file fix. v1.0")

Arg:AddOptionFile	("target", nil, nil, nil, "do_file", "target .do file")

if (Arg:DoParse() == false) then
	os.exit(0)
end
-- get target .do file name
local	sDoFileName		= Arg:GetOptionFile("target", 0)


local f = TextFile()

if f:Open(sDoFileName) then
	-- read
	local sCode	= String(f:GetAll())
	local iPrevCodeSize	= #sCode.s
	f:Close()

	-- modify
	sCode:Replace(sProjectPath.s, "..\\", true)

	-- write back
	if #sCode.s ~= iPrevCodeSize then
		if f:Create(sDoFileName) then
			f:Put(sCode.s)
			f:Close()
		end
	end
end