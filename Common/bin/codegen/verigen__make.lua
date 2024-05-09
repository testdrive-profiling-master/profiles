local	__constraint_list		= {}
local	__contraint_name		= nil	-- current constaint name

verigen.set_constraint = function(name, constraint)
	if name ~= nil then		-- new constraint
		__contraint_name		= name
		
		if constraint == nil then
			__constraint_list[name]	= nil
			return
		else
			__constraint_list[name]	= ""	-- initialize constaint
		end
	else					-- add constraint
		name					= __contraint_name
	end
	
	if name == nil then
		error("The first constraint must be named.", 2)
	end
	
	if #constraint ~= 0 then
		__constraint_list[name]		= __constraint_list[name] .. constraint .. "\n"
	end
end

local function IsImageFileName(filename)
	local fname = String(filename)
	return fname:CompareBack(".png") or fname:CompareFront(".jpg") or fname:CompareBack(".svg") or fname:CompareBack(".gif") or fname:CompareBack(".webp")
end

function module:make_constraint()
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. self.name .. "_constraint.xdc") == false then
		error("Can't create constraint file.", 2)
	end
	
	LOGI("Build contraint : " .. self.name .. "_constraint.xdc")
	
	f:Put("###############################################################################\n")
	f:Put("# clock & reset constraint\n")
	f:Put("###############################################################################\n")

	-- global reset
	f:Put("# resets\n")
	do
		local	reset_list		= {}
		for clk_name, clk in pairs(clock.__list) do
			if clk.__active then
				if clk:get_reset() ~= nil then
					reset_list[clk:get_reset()]	= 0
				end
			end
		end
		
		for rst_name, val in key_pairs(reset_list) do
			f:Put("set_false_path -from [get_ports " .. rst_name .. "]\n")
		end
	end
	
	-- private clock
	f:Put("\n# clocks\n")
	do
		for name, clk in key_pairs(clock.__list) do
			if clk.__active then
				f:Put("create_clock -name " .. name .. " -period " .. string.format("%f", 1000/clk.__speed) .. " [get_ports " .. name .. "]\n")
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
	
	f:Put("\n")
	for name, constraint in key_pairs(__constraint_list) do
		if constraint ~= nil and #constraint > 0 then
			f:Put("# " .. name .. "\n")
			f:Put(constraint .. "\n")
		end
	end
end

local	__graphviz		= String("")
local	__FileList		= String("")
local	__IncludeList	= String("")

function module:find_sub_module_matched_interface(cur_m, cur_i, cur_i_name)
	for name, m in key_pairs(self.sub_module) do
		if m ~= cur_m then
			for i_name, i in key_pairs(m.module.interfaces) do
				if i_name == cur_i_name and i.interface == cur_i then
					return true
				end
			end
		end
	end
	
	return false
end

function module:make_code(is_top)
	if self.__active or self.enable == false then
		return
	end

	self.__active	= true
	
	if is_top == nil then
		is_top	= true
	end
	
	-- build interface width
	if is_top then
		module.__top	= self
		interface.__build()
		__graphviz:clear()
		__FileList:clear()
		__IncludeList:clear()
	end
	
	-- make leaf module first
	for name, m in key_pairs(self.sub_module) do
		m.module:make_code(false)
	end
	
	__FileList:Append(self.name .. ".sv\n")
	__IncludeList:Append("`include \"" .. self.name .. ".sv\"\n")

	-- top definitions
	if is_top then
		LOGI("Build TOP design : " .. self.name .. ".sv")
	else
		LOGI("Build sub design : " .. self.name .. ".sv")
	end

	-- create top design file
	self:set_current_design()
	local	f	= TextFile()
	if f:Create(sOutPath .. "/" .. self.name .. ".sv") == false then
		error("Can't create top design file.", 2)
	end

	-- build code
	if self.code:Length() > 0 then
		__build_code(self.code)
	end
	
	-- code inception
	f:Put(self:get_inception())
	
	-------------------------------------------------------------------
	-- common include
	f:Put("`include \"" .. module.__top.name .. "_defines.vh\"\n\n")

	-------------------------------------------------------------------
	-- module declaration
	f:Put("module " .. self.name)
	
	local	sDocuments		= String("")
	local	sParameters		= String("")
	local	sPorts			= String("")
	local	sDeclares		= String("")
	local	sBody			= String("")
	local	sGraphviz		= String("URL=\"html/" .. self.name .. ".sv.html\" tooltip = \"" .. self.name .. "\" target=\"" .. self.name .. ".sv\" label=<<table border='" .. (is_top and "1" or "0") .. "' cellborder='1' cellspacing='0' cellpadding='4'><tr><td><b>" .. self.name .. "</b></td></tr>__DOC____PARAM__<tr><td align='left' SIDES='LRB'>__PORT__</td></tr></table>>, fillcolor=\"#__FILL_COLOR__\", shape=plain")
	
	-- document
	if self.document ~= nil then
		for doc_name, doc_file in key_pairs(self.document) do
			if IsImageFileName(doc_name) then
				sDocuments:Append("<tr><td href='cmd://DOC/" .. doc_file .. "' SIDES='LRB' cellspacing='1' cellpadding='1'><img src='" .. codegen.work_path .. doc_name .. "' /></td></tr>\n")
			else
				sDocuments:Append("<tr><td href='cmd://DOC/" .. doc_file .. "' SIDES='LRB' cellspacing='1' cellpadding='1' tooltip='" .. doc_name .. "'><font color=\"#0000FF\" point-size=\"9\"><b><u>[ " .. doc_name .. " ]</u></b></font></td></tr>\n")
			end
		end
	end
	
	if is_top then
		sGraphviz:Replace("__FILL_COLOR__", "ff880022")
	else
		sGraphviz:Replace("__FILL_COLOR__", self.fill_color)

		-- documents
		sGraphviz:Replace("__DOC__", (#sDocuments.s ~= 0) and sDocuments.s or "")

		-- original parameters
		do
			local	sParam	= String("")
			
			for name, param in key_pairs(self.params) do
				if param.is_local == false then
					sParam:Append("<font point-size=\"8\">" .. name .. "</font>\n")
				end
			end
			
			sParam:Replace("\n", "<br align=\"left\"/>", true)
			sGraphviz:Replace("__PARAM__", (#sParam.s ~= 0) and ("<tr><td align='left' SIDES='LRB'>" .. sParam.s .. "</td></tr>") or "")
		end
		
		-- original ports
		do
			local	sPort	= String("")
			
			for i_name, i in key_pairs(self.interfaces) do
				local	modport	= i.modport
				if modport ~= nil then
					sPort:Append("<font point-size=\"8\">" .. i_name  .. (i.__bared and "" or (" <font color=\"gray\">(" .. i.interface.name .. ")</font>")) .. "</font>\n")
				end

				-- force to add port interface's clock
				if i.interface:get_clock() ~= nil then
					self:add_clock(i.interface:get_clock())
				end
			end
			
			sPort:Replace("\n", "<br align=\"left\"/>", true)
			sGraphviz:Replace("__PORT__", sPort.s)
		end
	end

	-------------------------------------------------------------------
	-- module instances
	if verigen.graphviz.max_edge_count > 0 then	-- Limits the maximum number of graphviz edges connected to the same sub-module.
		local	pre_m_name			= ""
		local	pre_module_name		= ""
		local	same_link_count		= 0
		for m_name, m in key_pairs(self.sub_module) do
			if pre_module_name == m.module.name then
				same_link_count		= same_link_count + 1
				
				if same_link_count >= (verigen.graphviz.max_edge_count - 1) then	-- too many link, so hide
					m.__graphviz_hide	= true
				end
			else
				if same_link_count > verigen.graphviz.max_edge_count then			-- last link will be show
					self.sub_module[pre_m_name].__graphviz_hide	= nil
				end
				
				pre_module_name		= m.module.name
				same_link_count		= 0
			end

			pre_m_name			= m_name
		end
		
		if same_link_count > 0 then	-- last end of link must be shown
			self.sub_module[pre_m_name].__graphviz_hide	= nil
		end
	end
	
	do	-- traversal through sub module instances
		local	graphiviz_show	= true
		
		for m_name, m in key_pairs(self.sub_module) do
			local	sModule		= String("")
			local	no_ports	= true
			
			sModule:Append(m.code.prefix)
			
			sModule:Append("\n" .. m.module.name)
			
			-- first hiding and put "....."
			if (graphiviz_show == true) and (m.__graphviz_hide == true) then
				__graphviz:Append("\t" .. self.name .. " -> " .. m.module.name .. " [label=<<table border='0' cellspacing='0' cellpadding='0'><tr><td><b><font point-size=\"16\">......</font></b></td></tr></table>> color=white];\n")
				
			end
			graphiviz_show	= (m.__graphviz_hide ~= true)
			
			
			if graphiviz_show then
				__graphviz:Append("\t" .. self.name .. " -> " .. m.module.name .. " [label=<<table border='0' cellspacing='0' cellpadding='0'><tr><td><b>"..
					((m.graphviz.name_prefix ~= nil) and m.graphviz.name_prefix or "") ..
					m.name ..
					((m.graphviz.name_postfix ~= nil) and m.graphviz.name_postfix or "") .. 
					"</b></td></tr><tr><td align=\"left\">__MODULE__</td></tr></table>>" ..
					((m.graphviz.node ~= nil) and ("," .. m.graphviz.node) or "") ..
					"];\n")
			end
			local sGraphviz_Module	= String("")

			-- parameters
			do
				local	sParam		= String("")
				local	used_param	= {}
				
				for p_name, param in key_pairs(m.module.params) do
					sParam:Append("\t" .. string.format(".%-20s", p_name) .. "(")

					if m:get_param(p_name) == nil then
						sParam:Append(p_name)
						
						if self:get_param(p_name) == nil then
							self:set_param(p_name, param.default)
						end
					else
						sParam:Append(tostring(m:get_param(p_name)))
						used_param[p_name]	= true
						
						sGraphviz_Module:Append(p_name .. " = " .. tostring(m:get_param(p_name)) .. "\n")
					end
					
					sParam:Append("),\n")
				end

				if sParam:Length() ~= 0 then
					sParam:DeleteBack(",")
					sModule:Append(" #(\n" .. sParam.s .. ")")
				end
				
				-- check unused parameters
				for p_name, param in key_pairs(m.param) do
					if used_param[p_name] ~= true then
						LOGW("Parameter '" .. p_name .. "' is not matched on module[" .. m.module.name .. "]'s instance[" .. m.name .. "]")
					end
				end
			end
			
			-- ports
			sModule:Append(" " .. m.name .. " (\n")
			do
				local	sPort			= String("")

				-- clock & reset
				do
					local	clock_list		= {}
					for clk_name, clk in key_pairs(m.module.clocks) do
						self:add_clock(clk)
						
						if clock_list[clk.name] == nil then
							clock_list[clk.name]	= 0
							sPort:Append("\t" .. string.format(".%-20s", clk.name) .."(" .. clk.name .. "),\n")
						end
						
						if clk:get_reset() ~= nil then
							if clock_list[clk:get_reset()] == nil then
								clock_list[clk:get_reset()]	= 0
								sPort:Append("\t" .. string.format(".%-20s", clk:get_reset()) .."(" .. clk:get_reset() .. "),\n")
							end
						end
					end
				end
				
				-- ports
				for i_name, i in key_pairs(m.module.interfaces) do
					--local	sIPort	= String("")
					if i.modport ~= nil then
						--i.interface.__bared --TODO : bared interface?
						sPort:Append("\t" .. string.format(".%-20s", i.name) .."(")
						
						if m:get_port(i_name) ~= nil then	-- specified port
							sPort:Append(tostring(m:get_port(i_name)))
							
							if m.graphviz.ignore_port == nil then
								sGraphviz_Module:Append("." .. i.name .. " (" .. tostring(m:get_port(i_name)) .. ")\n")
							end
						else
							local	i_self	= self:get_interface(i_name)
							
							if i_self == nil then
								sPort:Append(i_name)
								i_self	= self:add_interface(i.interface, i_name)
								
								i_self.prefix		= i.prefix
								i_self.desc			= i.desc
								
								-- search other submodule's same name with same interface
								if self:find_sub_module_matched_interface(m, i.interface, i_name) == false then
									i_self.modport		= i.modport
								end
							else	-- check interface
								if i_self.interface ~= i.interface then
									error("Not same interface type : " .. m.module.name .. "(" .. i.interface.name .. ") != " .. self.name .. "(" .. i_self.name .. ")", 2)
								end
								sPort:Append(i_name)
							end
						end
						
						sPort:Append("),\n")
						
						no_ports	= false
					end
				end
				
				sPort:DeleteBack(",")
				sModule:Append(sPort.s)
			end
			sModule:Append(");\n")
			sModule:Append(m.code.postfix)
			
			if no_ports then
				sModule:Trim(" \r\n")
				sBody:Append("\n/* no ports in module. (commented out for DRC.)\n" .. sModule.s .. "*/\n")
			else
				sBody:Append(sModule.s)
			end
			
			sGraphviz_Module:Replace("\n", "<br align=\"left\"/>", true)
			
			if graphiviz_show then
				if sGraphviz_Module:Length() == 0 then
					__graphviz:Replace("__MODULE__", sGraphviz_Module.s)
				else
					__graphviz:Replace("__MODULE__", "<font point-size=\"6\" color=\"gray\">" .. sGraphviz_Module.s .. "</font>")
				end
			end
		end
	end
	
	-------------------------------------------------------------------
	-- parameters
	do
		for name, param in key_pairs(self.params) do
			if param.is_local == false then
				if sParameters:Length() > 0 then
					sParameters:Append(",\n")
				end
				
				sParameters:Append(string.format("\tparameter %-16s = ", name) .. tostring(param.default))
			end
		end
	end
	
	-------------------------------------------------------------------
	-- local parameters
	do
		local	bParam	= false
		for name, param in key_pairs(self.params) do
			if param.is_local then
				if bParam == false then
					sDeclares:Append("// local paramters\n")
					bParam	= true
				end
				
				sDeclares:Append(string.format("localparam %-19s = ", name) .. tostring(param.default) .. ";\n")
			end
		end

		if bParam then
			sDeclares:Append("\n")
		end
	end
	
	-------------------------------------------------------------------
	-- active interface
	for i_name, i in key_pairs(self.interfaces) do
		local	modport		= i.modport
		
		i.interface.__active	= true

		if is_top or (modport == nil) then
			if i.interface:get_clock() ~= nil then
				self:add_clock(i.interface:get_clock())
			end
		
			if i.__bared then
				if modport == nil then
					sDeclares:Append("// bared interface : " .. i_name .. ((i.desc == nil) and "" or (" (" .. i.desc .. ")")) .. "\n")

					if i.interface:signal_count() == 1 then
						for signal_name, signal in key_pairs(i.interface.signal) do
							sDeclares:Append(string.format("logic %-24s %s;\n", ((signal.width == 1) and "" or string.format("[%d:0]", signal.width - 1)), i_name))
						end
					else
						for signal_name, signal in key_pairs(i.interface.signal) do
							sDeclares:Append(string.format("logic %-24s %s;\n", ((signal.width == 1) and "" or string.format("[%d:0]", signal.width - 1)), signal.name))
						end
					end
				end
			else
				sDeclares:Append("// interface : " .. i_name .. ((i.desc == nil) and "" or (" (" .. i.desc .. ")")) .. "\n")
				sDeclares:Append(string.format("i_%-20s ", i.interface.name) .. i_name .. "();\n")
			end
			
			if is_top and (modport ~= nil) and i.__bared == false then
				for mp_name, mp_list in key_pairs(modport.data) do
					for id, io_name in key_pairs(mp_list) do
						local	bared_io_name	= io_name
						-- force to uppercase name
						if i.interface.__bared_uppercase == true then
							bared_io_name	= bared_io_name:upper()
						elseif i.interface.__bared_uppercase == false then
							bared_io_name	= bared_io_name:lower()
						end
					
						if mp_name == "input" then
							sDeclares:Append(string.format("assign %-23s = ", i_name .. "." .. io_name) .. i:get_prefix() .. bared_io_name .. ";\n")
						elseif mp_name == "output" then
							sDeclares:Append(string.format("assign %-23s = ", i:get_prefix() .. bared_io_name) .. i_name .. "." .. io_name .. ";\n")
						elseif mp_name == "inout" then
							error("inout port is not supported yet.", 2)
						end
					end
				end
			end
			sDeclares:Append("\n")
		end
	end
	
	-------------------------------------------------------------------
	-- ports	
	do
		-- clock & reset (active check)
		local 	iClockCount	= 0
		do
			local	bFirst			= true
			
			for clk_name, clk in key_pairs(self.clocks) do
				if bFirst then
					sPorts:Append(	"\t// clock & reset\n")
					bFirst	= false
				end
				
				iClockCount			= iClockCount + 1
				clk.__active		= true
				
				-- apply clock
				if clk.__desc == nil then
					sPorts:Append("\t" .. string.format("input  %20s%s,", "", clk.name) .."\n")
				else
					sPorts:Append(string.format("\t%-40s// %s\n", string.format("input  %20s%s,", "", clk.name), clk.__desc))
				end
				
				-- apply reset
				if clk:get_reset() ~= nil then
					sPorts:Append(string.format("\t%-40s// reset of '%s' (active low)\n", string.format("input  %20s%s,", "", clk:get_reset()), clk.name))
				end
			end
		end

		-- interface ports
		local	iPortCount	= 0
		for i_name, i in key_pairs(self.interfaces) do
			local	modport	= i.modport
			if modport ~= nil then
				iPortCount	= iPortCount + 1
				if sPorts:Length() > 0 then
					sPorts:Append("\n")
				end
				sPorts:Append("\t// " .. i_name .. ((i.desc == nil) and "" or (" (" .. i.desc .. ")")) .. "\n")
				if is_top or i.__bared then
					for mp_name, mp_list in key_pairs(modport.data) do
						for id, io_name in key_pairs(mp_list) do
							local	signal	= i.interface.signal[io_name]
							
							if signal == nil then
								error("Error on modport[" .. mp_name .. "] : Can't find signal[" .. io_name .. "] on interface[" .. i.interface.name .. "]", 2)
							end
							
							local	sLogic	= is_top and "" or "logic "
							if mp_name == "input" then
								sLogic	= ""
							end
							
							if i.__bared and i.interface:signal_count() == 1 then	-- single signal
								io_name	= i_name
							end
							
							-- force to uppercase name
							if i.interface.__bared_uppercase == true then
								io_name	= io_name:upper()
							elseif i.interface.__bared_uppercase == false then
								io_name	= io_name:lower()
							end
							
							sPorts:Append("\t" .. string.format("%-6s ", mp_name) ..
								(string.format("%-20s", (signal.width > 1) and string.format(sLogic .. "[%d:0]", signal.width - 1) or sLogic)) ..
								i:get_prefix() .. io_name .. ",\n")
						end
					end
				else
					sPorts:Append(string.format("\t%-26s ", (i.interface.__bared and "" or "i_") .. i.interface.name .. "." .. modport.name) .. i_name .. ",\n")
				end
			end
		end
		
		if iPortCount == 0 then
			LOGW("Empty port module : '" .. self.name .. "' module")
		end
		
		sPorts:DeleteBack(",")
	end
	
	-------------------------------------------------------------------
	-- build all codes
	
	if sParameters:Length() > 0 then
		f:Put(" #(\n" .. sParameters.s .. "\n)")
	end
	
	if sPorts:Length() > 0 then
		f:Put(" (\n" .. sPorts.s .. ");\n\n")
	else
		f:Put(" ();\n\n")
	end
	
	if sParameters:Length() > 0 then
		f:Put("// synopsys template\n")
	end
	
	if sDeclares:Length() > 0 then
		f:Put(sDeclares.s)
	end
	
	f:Put(sBody.s)
	
	if self.code:Length() > 0 then
		f:Put("\n" .. self.code.s .. "\n")
	end

	f:Put("endmodule\n")
	
	f:Close()
	__m	= nil
	
	-- apply istyle
	exec("istyle --style=kr -T4 -n \"" .. sOutPath .. "/" .. self.name .. ".sv\"")
	
	-- make html style output
	do
		exec("code_highlighter --ilang=verilog -n " .. sOutPath .. "/" .. self.name .. ".sv " .. sOutPath .. "/html/" .. self.name .. ".sv.html")
		
		-- add link with interface
		if f:Open(sOutPath .. "/html/" .. self.name .. ".sv.html") then
			local	sHtml	= String(f:GetAll())

			f:Close()
			
			local	iPos	= sHtml:find("i_", 1)
			
			while iPos > 0 do
				if (string.match(sHtml:at(iPos-1), "[0-9a-zA-Z_]") == nil) and (string.match(sHtml:at(iPos+2), "[0-9a-zA-Z_]") ~= nil) then
					local	iSize	= 3
					
					-- get interface name
					while string.match(sHtml:at(iPos+iSize), "[0-9a-zA-Z_]") ~= nil do
						iSize	= iSize + 1
					end

					local	i_name		= string.sub(sHtml.s,iPos + 1, iPos + iSize)	-- expression name
					local	i_org_name	= string.sub(sHtml.s,iPos + 3, iPos + iSize)	-- interface name
					
					if (interface.find(i_org_name) ~= nil) then
						-- delete previous expression
						sHtml:erase(iPos, iSize)
						
						-- insert link
						local	sLink	= "<a href=\"" .. module.__top.name .. "_defines.vh.html#" .. i_name .. "\"  target=\"" .. module.__top.name ..  "_defines.vh\"><font color=\"#F00000\"><b>" .. i_name .. "</font></b></a>"
						sHtml:insert(iPos, sLink)
						iPos	= iPos + #sLink
					else
						iPos	= iPos + iSize;
					end
				end

				iPos	= iPos + 1
				iPos	= sHtml:find("i_", iPos)
			end
			
			-- write back
			f:Create(sOutPath .. "/html/" .. self.name .. ".sv.html")
			f:Put("<!DOCTYPE html>\n" ..
				"<html>\n"..
				"<head>\n"..
				"	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n"..
				"	<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n"..
				"	<meta name=\"google\" content=\"notranslate\">\n"..
				"	<style>\n"..
				"	* {\n"..
				"		font-family: 'Cascadia Mono', courier, serif;\n"..
				"		font-size: 14px;\n"..
				"	}\n"..
				"	</style>\n"..
				"	<title>" .. self.name .. ".sv</title>\n"..
				"</head>\n"..
				"<body>\n")
			f:Put(sHtml.s)
			f:Put("</body>\n</html>\n")
			f:Close()
		end
	end
	
	-- graphivz : top final parameters and ports
	if is_top then
		-- documents
		sGraphviz:Replace("__DOC__", (#sDocuments.s ~= 0) and sDocuments.s or "")

		-- original parameters
		do
			local	sParam	= String("")
			
			for name, param in key_pairs(self.params) do
				if param.is_local == false then
					sParam:Append("<font point-size=\"8\">" .. name .. " = " ..  param.default .. "</font>\n")
				end
			end
			
			sParam:Trim(" \n")
			sParam:Replace("\n", "<br align=\"left\"/>", true)
			
			sGraphviz:Replace("__PARAM__", (#sParam.s ~= 0) and ("<tr><td align='left' SIDES='LRB'>" .. sParam.s .. "</td></tr>") or "")
		end
		
		-- original ports
		do
			local	sPort	= String("")
			
			for i_name, i in key_pairs(self.interfaces) do
				local	modport	= i.modport
				if modport ~= nil then
					sPort:Append("<font point-size=\"8\">" .. i_name  .. (i.__bared and "" or (" <font color=\"gray\">(" .. i.interface.name .. ")</font>")) .. "</font>\n")
				end
			end
			
			sPort:Trim(" \n")
			sPort:Replace("\n", "<br align=\"left\"/>", true)
			
			sGraphviz:Replace("__PORT__", sPort.s)
		end
	end

	__graphviz:Append("\t\"" .. self.name .. "\" [" .. sGraphviz.s .. "];\n")
	
	if is_top then
		-- contraint
		self:make_constraint()

		-- common defines
		if f:Create(sOutPath .. "/" .. self.name .. "_defines.vh") == false then
			error("Can't create common definition file.", 2)
		else
			LOGI("Make common defines : " .. self.name .. "_defines.vh")
			
			f:Put(self:get_inception())

			f:Put(	"`ifndef __" .. self.name:upper() .. "_DEFINES_VH__\n"..
					"`define __" .. self.name:upper() .. "_DEFINES_VH__\n"..
					"`include \"testdrive_system.vh\"		// default system defines\n\n")
			interface.__make_code(f)
			
			if __global_defines__.code:Length() > 0 then
				__build_code(__global_defines__.code)
				f:Put("\n" .. __global_defines__.code.s .. "\n")
			end
			
			f:Put(	"`endif//__" .. self.name:upper() .. "_DEFINES_VH__\n")
			f:Close()
			
			-- make html style output
			do
				exec("code_highlighter --ilang=verilog -n " .. sOutPath .. "/" .. self.name .. "_defines.vh " .. sOutPath .. "/html/" .. self.name .. "_defines.vh.html")
				-- add bookmark
				
				if f:Open(sOutPath .. "/html/" .. self.name .. "_defines.vh.html") then
					local	sHtml	= String(f:GetAll())

					f:Close()
					
					local	iPos	= sHtml:find(">interface</font> ", 1)
					
					while iPos > 0 do
						iPos	= iPos + 18

						local	iSize	= 1
						
						-- get interface name
						while string.match(sHtml:at(iPos+iSize), "[0-9a-zA-Z_]") ~= nil do
							iSize	= iSize + 1
						end
						local	i_name	= string.sub(sHtml.s,iPos + 1, iPos + iSize)
						
						-- delete previous expression
						sHtml:erase(iPos, iSize)
						
						-- insert link
						local	sLink	= "<a id=\"" .. i_name .. "\"><font color=\"#F00000\"><b>" .. i_name .. "</b></font></a>"
						sHtml:insert(iPos, sLink)
						iPos	= iPos + #sLink

						iPos	= sHtml:find(">interface</font> ", iPos)
					end
					
					-- write back
					f:Create(sOutPath .. "/html/" .. self.name .. "_defines.vh.html")
					f:Put("<!DOCTYPE html>\n" ..
						"<html>\n"..
						"<head>\n"..
						"	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n"..
						"	<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n"..
						"	<meta name=\"google\" content=\"notranslate\">\n"..
						"	<title>" .. self.name .. "_defines.vh</title>\n"..
						"</head>\n"..
						"<body>\n")
					f:Put(sHtml.s)
					f:Put("</body>\n</html>\n")
					f:Close()
				end
			end
			
			__graphviz:Append("\t\"" .. self.name .. "_defines\" [URL=\"html/" .. self.name .. "_defines.vh.html\" target=\"" .. self.name ..  "_defines.vh\" fillcolor=\"#D0FFD0\"];\n")
			__graphviz:Append("\t\"MAIN_SCRIPT\" -> \"" .. self.name .. "_defines\" [fillcolor=\"#F0C0C0\" style=dotted];\n")
		end
		
		-- Lua source list
		do
			__graphviz:Append("\t\"MAIN_SCRIPT\" [URL=\"cmd://PROJECT\" tooltip = \"Project\" label=<<table border='0' cellborder='1' cellspacing='0' cellpadding='2'><tr><td colspan='2'><b>" .. ((self.__title == nil) and "Project" or self.__title) .. "</b></td></tr>\n");
			
			-- document
			if module.document ~= nil then
				for doc_name, doc_file in key_pairs(module.document) do
					if IsImageFileName(doc_name) then
						__graphviz:Append("<tr><td href='cmd://DOC/" .. doc_file .. "' align='left' SIDES='LR' colspan='2' cellspacing='0' cellpadding='3'><img src='" .. codegen.work_path .. doc_name .. "' /></td></tr>\n")
					else
						__graphviz:Append("<tr><td href='cmd://DOC/" .. doc_file .. "' align='center' SIDES='LR' colspan='2' cellspacing='0' cellpadding='3' tooltip='" .. doc_name .. "'><font color=\"#0000FF\" point-size=\"9\"><b><u>[ " .. doc_name .. " ]</u></b></font></td></tr>\n")
					end
				end
			end
			
			for i, v in ipairs(verigen.lua_files) do
				if v.filename ~= nil then
					local sFileName = String(v.filename)
					local sToolTip	= (v.desc ~= nil) and v.desc or sFileName.s
					sFileName:CutFront("\\")
					
					if v.desc ~= nil then
						local sName = sFileName.s
						sFileName.s = "[" .. sName .. "]"
					end
					
					__graphviz:Append("<tr><td href='cmd://LUA/" .. v.filename .. "' align='right' SIDES='L" .. ((i == #verigen.lua_files) and "B" or "") .. "' cellspacing='0' cellpadding='3' tooltip='" .. sToolTip .. "'><font color='#2020AF' point-size='10'>" .. ((v.desc == nil) and " " or v.desc) .. "</font></td>")
					__graphviz:Append("<td href='cmd://LUA/" .. v.filename .. "' align='left' SIDES='R" .. ((i == #verigen.lua_files) and "B" or "") .. "' cellspacing='0' cellpadding='3' tooltip='" .. sToolTip .. "'><font color='#A0A0AF' point-size='10'>" .. sFileName.s .. "</font></td></tr>")
				end
			end
			
			__graphviz:Append("</table>>, fillcolor=\"#F7F7F0\", shape=plain];");
			__graphviz:Append("	\"MAIN_SCRIPT\" -> \"" .. self.name .. "\" [fillcolor=\"#FFC0C0\" style=dotted];\n")
	
		end
		
		-- graphviz
		if __graphviz:Length() > 0 then
			if f:Create(sOutPath .. "/" .. self.name .. ".dot") then
				LOGI("Make design hierarchy : " .. self.name .. "_hierarchy.svg")
				f:Put(
					"digraph Design_Hierarchy {\n"..
					"fontname=\"Helvetica,Arial,sans-serif\"\n"..
					"node [fontname=\"Helvetica,Arial,sans-serif\", color=\"#00000020\"]\n"..
					"edge [fontname=\"Helvetica,Arial,sans-serif\", fontsize=8, color=\"#00008880\"]\n"..
					--"rankdir=\"LR\"\n"..
					"node [fontsize=12, shape=rect, height=0.25, style=filled, pencolor=\"#00000034\"]\n"..
					__graphviz.s ..
					"}\n")
				f:Close()
				
				exec("dot -Tsvg " .. sOutPath .. "/" .. self.name .. ".dot -o " .. sOutPath .. "/" .. self.name .. "_hierarchy.svg")
				os.remove(sOutPath .. "/" .. self.name .. ".dot")
			end
		end
		
		-- file list
		if f:Create(sOutPath .. "/" .. self.name .. ".f") then
			LOGI("Make design file list : " .. self.name .. ".f")
			f:Put(__FileList.s)
			f:Close()
		end
		
		-- include list
		if f:Create(sOutPath .. "/" .. self.name .. "_includes.vh") then
			f:Put("`ifndef __" .. self.name:upper() .. "_INCLUDES_VH__\n")
			f:Put("`define __" .. self.name:upper() .. "_INCLUDES_VH__\n")
			f:Put(__IncludeList.s)
			f:Put("`endif//__" .. self.name:upper() .. "_INCLUDES_VH__\n")
			f:Close()
		end
		
		-- Post to TestDrive's 'Design map' document 
		do
			local	sWorkPath	= lfs.currentdir()		-- ./verigen_src
			local	sOutputPath	= String(sWorkPath)		-- .
			local	sDesignFile	= self.name .. "_hierarchy.svg"
			
			sOutputPath:Append("\\" .. sOutPath)
			sOutputPath			= sOutputPath.s
			
			PostToDocument("DesignMap", 0, sWorkPath .. ";" .. sOutputPath .. ";" .. sDesignFile .. ";")
		end
	end
end

---------------------------------------------------------------------------------------
-- verilog inline commands
---------------------------------------------------------------------------------------
vfunction("LOG2", function(val, bOverflow)
	local	result	= (math.log(val)/math.log(2))
	
	if bOverflow == nil then
		bOverflow	= false
	end
	
	if bOverflow then
		if math.floor(result) < result then
			result	= math.floor(result + 1)
		end
	end
	
	if math.floor(result) ~= result then
		error("LOG2 fault : input '" .. val .. "' is not 2^N", 2)
	end
	
	return math.floor(result)
end)

vfunction("RANGE", function(size, step)
	local s = String("")

	if size == nil then size = 1 end

	if size <= 0 then
		error("size must be greater than 0.", 2)
	end

	if step == nil then
		step	= 0
	elseif step < 0 then
		error("step must be greater than 0.", 2)
	end
	
	start_pos	= step * size

	local	end_pos = size + start_pos - 1

	if end_pos ~= 0 then
		s:Append("[" .. end_pos .. ":" .. start_pos .. "]")
	end

	return s.s
end)


vfunction("DEMUX_BY_EN", function(width, channel_count, en, data_in, data_out)
	local s	= String("")
	local inst_name = String(data_out)

	inst_name:Replace(".", "_", true)
	inst_name:CutBack("[", true)

	s:Append("wire	[" .. (width-1) .. ":0]	__" .. inst_name.s .. ";")
	s:Append("demux_by_enable #(\n")
	s:Append("	.WIDTH				(" .. width .. "),\n")
	s:Append("	.CHANNELS			(" .. channel_count .. "),\n")
	s:Append("	.TRISTATE			(1)\n")
	s:Append(") demux_en_" .. inst_name.s .. " (\n")
	s:Append("	.EN_BUS				(" .. en .. "),\n")
	s:Append("	.DIN_BUS			(" .. data_in .. "),\n")
	s:Append("	.DOUT				(__" .. inst_name.s .. ")\n")
	s:Append(");\n")
	s:Append("assign	" .. data_out .. " = __" .. inst_name.s .. ";\n")

	return s.s
end)

vfunction("MULTICYCLE", function(module_inst_name, if_name, cycle_count, instance_count, clk)
	if __m == nil then
		error("Multicycle instance can specified only in module code.", 2)
	end
	
	local	m	= __m:get_module(module_inst_name)
	
	if m == nil then
		error("Can't find module instance[" .. tostring(module_inst_name) .. "] in module[" .. __m.name .. "]", 2)
	end
	
	if cycle_count < 2 or cycle_count > 12 then
		error("multicycle count is out of range : " .. cycle_count, 2)
	end
	
	if #m.code.prefix ~= 0 then
		error("Already other code wrapper is assigned.", 2)
	end
	
	if instance_count == nil then
		instance_count	= cycle_count
	end
	
	if (instance_count > cycle_count) or (instance_count < 1) then
		error("Multicycle instance count is out of range.", 2)
	end
	
	if clk == nil then
		clk	= clock.get_default()
		
		if clk == nil then
			error("No clock is existed.", 2)
		end
		
		if clk:get_reset() == nil then
			error("MULTICYCLE instance needs a reset signal on clock[" .. clk.name .. "].", 2)
		end
	end
	
	-- add "genvar i"
	if __m.__genvar_i == nil then
		m.code.prefix	= "genvar i;\n"
		__m.__genvar_i	= true
	end
	
	-- find interface
	local	_i			= nil
	local	input_size	= 0
	local	output_size	= 0
	local	sInput		= ""
	local	sOutput		= ""

	do
		local	interface_count		= 0
		for i_name, i in pairs(m.module.interfaces) do
			if i.modport ~= nil then
				interface_count	= interface_count + 1
				_i	= i
			end
		end
		
		if interface_count ~= 1 then
			error("Multicycle design[" .. m.module.name .. "] must specify one interface port.", 2)
		end
		
		if if_name == nil then
			if_name		= _i.name
		end
		
		
		for port_type, port_list in key_pairs(_i.modport.data) do
			for i, sig_name in ipairs(port_list) do
				
				if port_type == "input" then
					input_size	= input_size + _i.interface.signal[sig_name].width
			
					if #sInput ~= 0 then
						sInput	= sInput .. ", "
					end
					sInput		= sInput .. "$." .. sig_name
				elseif port_type == "output" then
					output_size	= output_size + _i.interface.signal[sig_name].width

					if #sOutput ~= 0 then
						sOutput	= sOutput .. ", "
					end
					sOutput		= sOutput .. "$." .. sig_name
				else
					error("Unknown port type.")
				end
				
			end
		end
	end
	
	local	sInput_temp		= String(sInput)
	local	sOutput_temp	= String(sOutput)
	local	bEx				= (instance_count ~= cycle_count)
	
	sInput	= String(sInput)
	sOutput	= String(sOutput)
	
	sInput:Replace("$", if_name, true)
	sOutput:Replace("$", if_name, true)
	sInput_temp:Replace("$", "__temp", true)
	sOutput_temp:Replace("$", "__temp", true)
	
	m.code.prefix	= m.code.prefix..
		"// multicycle design for " .. module_inst_name .. "\n" ..
		"i_" .. _i.interface.name .. "	" .. if_name .. "();\n"..
		"wire	" .. if_name .. "_ie, " .. if_name .. "_oe" .. (bEx and (", " .. if_name .. "_iready") or "").. ";\n"..
		"generate\n" ..
		"wire	[" .. (input_size * instance_count - 1) .. ":0]	pipe_i;\n"..
		"wire	[" .. (output_size * instance_count - 1) .. ":0]	pipe_o;\n"..
		"wire	[" .. (output_size - 1) .. ":0]	__o;\n\n"..
		"MultiCyclePath" .. (bEx and "Ex" or "") .. " #(\n"..
		"	.IWIDTH		(" .. input_size .. "),\n"..
		"	.OWIDTH		(" .. output_size .. "),\n"..
		"	.CYCLE		(" .. cycle_count .. ")"..
		(bEx and ",\n	.COUNT		(" .. instance_count .. ")\n" or "\n")..
		") multi_pipe (\n"..
		"	.CLK		(" .. clk.name .. "),\n"..
		"	.nRST		(" .. clk:get_reset() .. "),\n"..
		"	.IE			(" .. if_name .. "_ie),\n"..
		"	.IDATA		({" .. sInput.s .. "}),\n"..
		(bEx and ("	.IREADY		({" .. if_name .. "_iready}),\n") or "")..
		"	.PIPE_I		(pipe_i),\n"..
		"	.PIPE_O		(pipe_o),\n"..
		"	.OE			(" .. if_name .. "_oe),\n"..
		"	.ODATA		(__o)\n"..
		");\n\n"..
		"assign	{" .. sOutput.s .. "}	= __o;\n\n" ..
		"for(i=0; i<" .. instance_count .. "; i=i+1) begin\n"..
		"i_" .. _i.interface.name .. "		__temp();" ..
		"assign	{" .. sInput_temp.s .. "}	= pipe_i[`BUS_RANGE(" .. input_size .. ", i)];\n" ..
		"assign	pipe_o[`BUS_RANGE(" .. output_size .. ",i)]	= {" .. sOutput_temp.s .. "};\n"

	-- set multi-cycle constraint
	verigen.set_constraint("multicycle_path_" .. cycle_count, "set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ \"*gen_multicycle.path_" .. cycle_count .. ".i_*\"}] " .. (cycle_count-1))
	verigen.set_constraint(nil, "set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ \"*gen_multicycle.path_" .. cycle_count .. ".i_*\"}] " .. (cycle_count))

	m:set_port(_i.name, "__temp")

	m.code.postfix	= "end\nendgenerate\n"
	
	-- graphviz coordinate
	m.graphviz.name_prefix	= "<font color=\"#0000CF\">"
	m.graphviz.name_postfix	= "</font><font color=\"red\">(" .. (bEx and (instance_count .. "/" .. cycle_count) or tostring(cycle_count)) .. ")</font>"
	m.graphviz.node			= "color=orange,style=\"dashed\""
	m.graphviz.ignore_port	= true
end)
