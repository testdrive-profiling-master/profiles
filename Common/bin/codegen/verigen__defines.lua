local	defines				= {}

function SetDefine(name, val)
	if type(name) ~= "string" then
		error("name is not a string type.", 2)
	end
	
	if val == nil then
		error("Definition value must be specified.", 2)
	end

	defines[name]	 = val
end

function ApplyDefines(s)
	for name, val in pairs(defines) do
		s:ReplaceVariable(name, tostring(val))
	end
end