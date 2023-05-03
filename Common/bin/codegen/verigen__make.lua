local	__constraint_list	= {}

-- build constraint file
function module:make_definition()
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. module.name .. "_defines.vh") == false then
		__ERROR("Can't create configuration file.")
	end

	f:Put(	"`ifndef __" .. module.name:upper() .. "_DEFINES_VH__\n"..
			"`define __" .. module.name:upper() .. "_DEFINES_VH__\n"..
			"`include \"testdrive_system.vh\"		// default system defines\n"..
			"`include \"" .. module.name .. "_config.vh\"		// current system configurations\n\n")
	interface.__make_code(f)
	f:Put(	"`endif//__" .. module.name:upper() .. "_DEFINES_VH__\n")
end

function set_constraint(name, constraint)
	if __constraint_list[name] == nil then
		__constraint_list[name]		= constraint
	end
end

function module:make_constraint()
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. self.name .. "_constraint.xdc") == false then
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
			if clk.__active then
				if clk.__rst ~= nil then
					f:Put("set_false_path -from [get_ports " .. clk.__rst .. "]\n")
				else
					use_default_rst	= true
				end
			end
		end
		if use_default_rst then
			f:Put("set_false_path -from [get_ports nRST]\n")
		end
	end
	
	-- private clock
	f:Put("\n# clocks\n")
	do
		for name, clk in key_pairs(clock.__list) do
			if clk.__active then
				f:Put("create_clock -name " .. name .. " -period " .. string.format("%f", 1000/clk.__speed) .. " [get_ports " .. name .. "]\n")
				if clk.__private_reset then
					f:Put("set_false_path -from [get_ports " .. name .. "_nRST]\n")
				end
			end
		end
		
		-- false path with clock domain
		for name_a, clk_a in key_pairs(clock.__list) do
			if clk_a.__active then
				for name_b, clk_b in key_pairs(clock.__list) do
					if clk_b.__active then
						if clk_a ~= clk_b then
							f:Put("set_false_path -from [all_registers -clock " .. name_a .. "] -to [all_registers -clock " .. name_b .. "]\n")
						end
					end
				end
			end
		end
	end
	
	for name, constraint in key_pairs(__constraint_list) do
		f:Put("\n# " .. key .. "\n")
		f:Put(constraint .. "\n")
	end
end

function module:make_code(is_top)
	if self.__active then
		return
	end

	self.__active	= true
	
	if is_top == nil then
		is_top	= true
	end

	-- top definitions
	if is_top then
		LOGI("TOP design name : " .. self.name)
		module.__top	= self
		
		-- defines »ý¼º
		do
			local	f = TextFile()
			if f:Create(sOutPath .. "/" .. self.name .. "_defines.vh") == false then
				__ERROR("Can't create configuration file.")
			end
			
			f:Put(self:get_inception())

			f:Put(	"`ifndef __" .. self.name:upper() .. "_DEFINES_VH__\n"..
					"`define __" .. self.name:upper() .. "_DEFINES_VH__\n"..
					"`include \"testdrive_system.vh\"		// default system defines\n"..
					"`include \"" .. self.name .. "_config.vh\"		// current system configurations\n\n")
			interface.__make_code(f)
			f:Put(	"`endif//__" .. self.name:upper() .. "_DEFINES_VH__\n")
		end
	end
	
	-- make leaf module first
	for name, m in key_pairs(self.modules) do
		m.module:make_code(false)
	end

	-- create top design file
	local	f		= TextFile()
	if f:Create(sOutPath .. "/" .. self.name .. ".sv") == false then
		__ERROR("Can't create top design file.")
	end
	
	-- code inception
	f:Put(self:get_inception())
	
	-------------------------------------------------------------------
	-- common include
	f:Put("`include \"" .. module.__top.name .. "_defines.vh\"\n\n")

	-------------------------------------------------------------------
	-- module declaration
	f:Put("module " .. self.name)

	-------------------------------------------------------------------
	-- parameters
	do
		local	bParam	= false
		for name, param in key_pairs(self.params) do
			if param.is_local == false then
				if bParam == false then
					f:Put(" #(\n")
					bParam	= true
				else
					f:Put(",\n")
				end
				
				f:Put(string.format("\tparameter %-16s = ", name) .. tostring(param.default))
			end
		end
		
		if bParam then
			f:Put("\n)")
		end
	end

	-------------------------------------------------------------------
	-- ports	
	f:Put(" (\n")
	do
		-- clock & reset (active check)	
		local	sPort	= String("")
		do
			local	bFirst	= true
			for i_name, i in key_pairs(self.interfaces) do
				local	clk		= i.interface:get_clock()

				if clk ~= nil then
					if bFirst then
						sPort:Append(	"\t// clock & reset\n")
						bFirst	= false
					end
					
					clk.__active	= true
					
					if clk.__desc == nil then
						sPort:Append("\t" .. string.format("input  %20s%s,", "", clk.name) .."\n")
					else
						sPort:Append(string.format("\t%-40s// %s\n", string.format("input  %20s%s,", "", clk.name), clk.__desc))
					end
				end
			end
			
			-- resets
			if bFirst == false then
				local	use_default_rst = false
				for clk_name, clk in key_pairs(clock.__list) do
					if clk.__active then
						if clk.__rst ~= nil then
							sPort:Append(string.format("\t%-40s// reset of '%s' (active low)\n", string.format("input  %20s%s,", "", clk.__rst), clk.name))
						else
							use_default_rst	= true
						end
					end
				end
				if use_default_rst then
					sPort:Append(string.format("\t%-40s// default global reset (active low)\n", string.format("input  %20s%s,", "", clock.__default_rst)))
				end
			end
		end

		-- interface ports
		for i_name, i in key_pairs(self.interfaces) do
			local	modport	= i.modport
			if modport ~= nil then
				sPort:Append("\n\t// " .. i_name .. ((i.desc == nil) and "" or (" (" .. i.desc .. ")")) .. "\n")
				if is_top then
					if is_top then
						for mp_name, mp_list in key_pairs(modport.data) do
							for id, io_name in key_pairs(mp_list) do
								local	signal	= i.interface.signal[io_name]
								sPort:Append("\t" .. string.format("%-6s ", mp_name) ..
									(string.format("%-20s", (signal.width > 1) and string.format("[%d:0]", signal.width - 1) or "")) ..
									i.prefix .. io_name .. ",\n")
							end
						end
					end
				else
					sPort:Append(string.format("\t%-26s ", i.interface.name .. "." .. modport.name) .. i_name .. ",\n")
				end
			end
		end
		
		sPort:DeleteBack(",")
		
		f:Put(sPort.s)
	end
	f:Put(");\n\n")

	-------------------------------------------------------------------
	-- local parameters
	do
		local	bParam	= false
		for name, param in key_pairs(self.params) do
			if param.is_local then
				if bParam == false then
					f:Put("// local paramters\n")
					bParam	= true
				end
				
				f:Put(string.format("localparam %-19s = ", name) .. tostring(param.default) .. ";\n")
			end
		end

		if bParam then
			f:Put("\n")
		end
	end

	-------------------------------------------------------------------
	-- active interface
	for i_name, i in key_pairs(self.interfaces) do
		local	modport		= i.modport

		if is_top or (modport == nil) then
			f:Put("// interface : " .. i_name .. ((i.desc == nil) and "" or (" (" .. i.desc .. ")")) .. "\n")
			
			f:Put(string.format("i_%-20s ", i.interface.name) .. i_name .. ";\n")
			
			if is_top and (modport ~= nil) then
				for mp_name, mp_list in key_pairs(modport.data) do
					for id, io_name in key_pairs(mp_list) do
						if mp_name == "input" then
							f:Put(string.format("assign %-23s = ", i_name .. "." .. io_name) .. i.prefix .. io_name .. ";\n")
						elseif mp_name == "output" then
							f:Put(string.format("assign %-23s = ", i.prefix .. io_name) .. i_name .. "." .. io_name .. ";\n")
						elseif mp_name == "inout" then
							__ERROR("inout port is not supported.")
						end
					end
				end
			end
			f:Put("\n")
		end
	end
	
	-------------------------------------------------------------------
	-- module instances
	for m_name, m in key_pairs(self.modules) do
		f:Put(m.module.name)
		
		-- parameters
		do
			local	sParam		= String("")
			for p_name, param in key_pairs(self.params) do
				if bFirst then
					bFirst	= false
				end
				sParam:Append("\t" .. string.format(".%-20s", p_name) .. "(")
				sParam:Append((m:get_param(p_name) == nil) and p_name or tostring(m:get_param(p_name)))
				sParam:Append("),\n")
			end

			if sParam:Length() ~= 0 then
				sParam:DeleteBack(",")
				f:Put(" #(\n" .. sParam.s .. ")")
			end
		end
		
		-- ports
		f:Put(" " .. m.name .. " (\n")
		do
			local	sPort			= String("")

			-- clock & reset
			do
				local	clock_list		= {}
				local	use_global_rst	= false
				for i_name, i in key_pairs(m.module.interfaces) do
					if i.modport ~= nil then
						local	clk		= i.interface:get_clock()

						if clk ~= nil then			
							if clock_list[clk.name] == nil then
								clock_list[clk.name]	= 0
								sPort:Append("\t" .. string.format(".%-20s", clk.name) .."(" .. clk.name .. "),\n")
							end
						end
						
						if clk.__rst ~= nil then
							if clock_list[clk.__rst] == nil then
								clock_list[clk.__rst]	= 0
								sPort:Append("\t" .. string.format(".%-20s", clk.__rst) .."(" .. clk.__rst .. "),\n")
							end
						else
							use_global_rst	= true
						end
					end
				end
				
				if use_global_rst then
					sPort:Append("\t" .. string.format(".%-20s", "nRST") .."(" .. "nRST" .. "),\n")
				end
			end
			
			for i_name, i in key_pairs(m.module.interfaces) do
				if i.modport ~= nil then
					sPort:Append("\t" .. string.format(".%-20s", i.name) .."(" .. "),\n")
				end
			end
			
			sPort:DeleteBack(",")
			f:Put(sPort.s)
		end
		f:Put(");\n")
	end
	
	f:Put(self.code .. "\n")

	f:Put(	"endmodule\n")
	
	f:Close()
	
	if is_top then
		self:make_constraint()
	end
end
