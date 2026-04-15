#lua
do
	local regmap_tree	= nil -- ProfileTree("System|QEMU")
	
	local qemu_path = System.ProjectPath .. "Program/qemu/"
	local bFirst    = true

	for file in lfs.dir(qemu_path) do
		local sFile = String(file)
		if sFile:CompareFront(".") == false then
			local ini_path = qemu_path .. file .. "/qemu_testdrive.ini"
			if lfs.IsExist(ini_path) then
				if bFirst then
					regmap_tree	= ProfileTree("System|QEMU")
					bFirst = true
				end
				local sTitle = String()
				sTitle:GetEnvironment( "TITLE@QEMU@".. ini_path)
				if sTitle:IsEmpty() then
					sTitle.s = file
				end
				
				local qemu_sp_path = qemu_path .. file .."/qemu.sp"
				
				local f = TextFile()
				f:Create(qemu_sp_path)
				f:Put(
					"#lua\n" .. 
					"LOG_CLEAR()\n" ..
					"LOGI(\"" .. sTitle.s .. " is running...\")\n" ..
					"System.Execute(\"qemu_testdrive.bat\", \"\", \".\", {\"error:\", \"] Error\", {\"*I: \",0}})\n"
				)
				f:Close()
				regmap_tree:AddItem("screen", sTitle.s, qemu_sp_path)
			end
		end
	end
	
	if regmap_tree ~= nil then
		regmap_tree:Expand()
	end
end

