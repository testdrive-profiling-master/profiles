-- default definitions
RunScript("defines_PROJECT.lua")

-- IP design
RunScript("design_hierarchy.lua")

-- add eclipse project link
module:add_document("img/simulation_top.png", "../.eclipse.bat")

----------------------------------------------------------------
-- add all code details
----------------------------------------------------------------
for entry in lfs.dir(".") do
	local	s	= String(entry)
	if s:CompareBack(".sv") then
		module.apply_code(entry)
	end
end

----------------------------------------------------------------
-- make design
----------------------------------------------------------------
PROJECT.top_wrapper:make_code()

do	-- Post to TestDrive's 'Design map' document
	local	sWorkPath	= lfs.currentdir()		-- ./verigen_src
	local	sTopPath	= String(sWorkPath)		-- .
	sTopPath:CutBack("\\")
	sTopPath			= sTopPath.s
	local	sOutputPath	= sTopPath	.. "\\rtl"
	local	sDesignFile	= "PROJECT_wrapper_hierarchy.svg"
	
	PostToDocument("DesignMap", 0, sWorkPath .. ";" .. sOutputPath .. ";" .. sDesignFile .. ";")
end
