-- build constraint file

do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. module.__top .. "_constraint.xdc") == false then
		__ERROR("Can't create constraint file.")
	end
	
	f:Put("###############################################################################\n")
	f:Put("# clock & reset constraint\n")
	f:Put("###############################################################################\n")

	-- global reset
	f:Put("# resets\n")
	do
		local	use_default_rst = false
		for clk_name, clk in key_pairs(clock.__list) do
			if clk.__rst ~= nil then
				f:Put("set_false_path -from [get_ports " .. clk.__rst .. "]\n")
			end
		end
		f:Put("set_false_path -from [get_ports nRST]\n")
	end
	
	-- private clock
	f:Put("\n# clocks\n")
	do
		for name, clk in key_pairs(clock.__list) do
			f:Put("create_clock -name " .. name .. " -period " .. string.format("%f", 1000/clk.__speed) .. " [get_ports " .. name .. "]\n")
			if clk.__private_reset then
				f:Put("set_false_path -from [get_ports " .. name .. "_nRST]\n")
			end
		end
		
		-- false path with clock domain
		for name_a, clk_a in key_pairs(clock.__list) do
			for name_b, clk_b in key_pairs(clock.__list) do
				if clk_a ~= clk_b then
					f:Put("set_false_path -from [all_registers -clock " .. name_a .. "] -to [all_registers -clock " .. name_b .. "]\n")
				end
			end
		end
	end
end
