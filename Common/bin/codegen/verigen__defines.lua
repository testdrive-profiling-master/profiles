local	__defines				= {}

function SetDefine(name, code, defines)
	if type(name) ~= "string" then
		error("name is not a string type.", 2)
	end
	
	if code == nil then
		error("Definition code must be specified.", 2)
	end
	
	if defines == nil then
		defines	= __defines
	end
	
	local	sName	= String(name)
	local	sArgs	= nil

	-- function type
	if sName:CompareBack(")") and sName:find("(", 0) > 0 then
		sArgs		= String(sName.s)
		sName:CutBack("(", true)
		sName:Trim(" \t")
		sArgs:CutFront("(", false)
		sArgs:CutBack(")", false)
	end

	defines[sName.s]		 = {}
	defines[sName.s].code	 = code
	
	-- decoding arguments
	if sArgs ~= nil then
		local	sTok	= sArgs:Tokenize(",")
		sTok:Trim(" \t")
		
		defines[sName.s].arg	= {}
		
		while sTok:Length() ~= 0 do
			defines[sName.s].arg[#(defines[sName.s].arg) + 1]	= sTok.s
			
			sTok	= sArgs:Tokenize(",")
			sTok:Trim(" \t")
		end
		
		--[[for i, arg in ipairs(defines[sName.s].arg) do
			LOGI("p : " .. arg)
		end--]]
	end
end

function ApplyDefines(s, defines)
	if defines == nil then
		defines	= __defines
	end

	for name, val in pairs(defines) do
		if val.arg == nil then
			-- just variable
			s:ReplaceVariable(name, load("return (" .. val.code .. ")")())
		else
			-- function
			local	sSrc	= String(s.s)
			local	sOut	= String("")
			local	sExpr	= "$" .. name .. "(*)"

			while sSrc.TokenizePos >= 0 do
				sOut:Append(sSrc:TokenizeVariable(sExpr).s)
				
				if sSrc.TokenizePos >= 0 then
					-- get all arguments
					local	sArgs	= sSrc:GetVariable()
					local	val_table	= {}
					local	sTok		= sArgs:Tokenize(",")
					while sTok:Length() ~= 0 do
						val_table[#val_table + 1]	= sTok.s
						sTok		= sArgs:Tokenize(",")
					end
					
					-- make code
					local	sCode		= String(val.code)
					
					for i, arg in ipairs(val.arg) do
						if i <= #val_table then
							sCode:ReplaceVariable(arg, val_table[i])
						else
							sCode:ReplaceVariable(arg, "nil")
						end
					end
					
					sOut:Append(tostring(load("return (" .. sCode.s .. ")")()))
				end
			end
			
			s.s	= sOut.s
		end
	end
end
