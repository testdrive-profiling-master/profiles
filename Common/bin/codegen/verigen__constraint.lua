-- build constraint file

do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. module.__top.name .. "_constraint.xdc") == false then
		LOGE("Can't create constraint file.")
		os.exit(1)
	end
	
	f:Put("###############################################################################\n")
	f:Put("# clock & reset constraint\n")
	f:Put("###############################################################################\n")

	-- global reset
	f:Put(	"# global reset\n"..
			"set_false_path -from [get_ports nRST]\n")
	
	-- private clock
	do
		for name, clk in pairs(clock.__list) do
			f:Put("create_clock -name " .. name .. " -period " .. string.format("%f", 1000/clk.__speed) .. " [get_ports " .. name .. "]\n")
			if clk.__private_reset then
				f:Put("set_false_path -from [get_ports " .. name .. "_nRST]\n")
			end
		end
		
		-- false path with clock domain
		for name_a, clk_a in pairs(clock.__list) do
			for name_b, clk_b in pairs(clock.__list) do
				if clk_a ~= clk_b then
					f:Put("set_false_path -from [all_registers -clock " .. name_a .. "] -to [all_registers -clock " .. name_b .. "]\n")
				end
			end
		end
	end
end
