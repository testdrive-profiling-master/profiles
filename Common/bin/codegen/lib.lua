local sArg = String()
for i=0,codegen.ArgumentSize() - 1 do
	sArg:Append(codegen.GetArgument(i) .. " ")
end

if codegen.ArgumentSize() == 0 then
	LOGI("@4Lua library manager with LuaRocks.\n")
end

os.execute("luarocks " .. sArg.s)
