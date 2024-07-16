do	-- fix for msys2 home path
	local	sFrom	= String()
	local	sTo		= String()
	sFrom.s = package.path
	sFrom:CutBack("..", true)	-- get codegen path
	sFrom:Append("..")
	-- msys2 path
	sTo:GetEnvironment("TESTDRIVE_DIR")
	sTo:Append("bin/msys64/ucrt64")
	-- fix it!
	local sEnv = String(package.path)
	sEnv:Replace(sFrom.s, sTo.s, true)
	sEnv:Append(";" .. codegen.env_path .. "lib/?.lua")
	package.path	= sEnv.s
	sEnv.s = package.cpath
	sEnv:Replace(sFrom.s, sTo.s, true)
	sEnv:Append(";" .. codegen.env_path .. "lib/?." .. (IsWindows() and "dll" or "so"))
	package.cpath	= sEnv.s
end

--Installation of OS package
os.__installed_package = {}

os.require = function(package_name)
	-- build installed package list, if it needed.
	if os.__installed_package["bash"] == nil then
		local	sList	= String(exec("pacman -Q"))

		while (sList.TokenizePos >= 0) do
			local	sPackage	= sList:Tokenize("\r\n")
			sPackage:CutBack(" ", true)
			
			if sPackage:Length() > 0 then
				os.__installed_package[sPackage.s] = true
			end
		end
	end

	-- check already installed?
	if os.__installed_package[package_name] ~= nil then
		if os.__installed_package[package_name] == false then
			LOGE("Required '" .. package_name .. "' package is not found.")
		end
		return os.__installed_package[package_name]
	end

	-- try to install
	LOGI("@3*I: Installing a required '" .. package_name .. "' package, please wait... ")
	local sRun	= String(exec("pacman -S --noconfirm " .. package_name))
	if sRun:CompareFront("error:") == false then
		os.__installed_package[package_name] = true
		LOGI("@4Done.\n")
		return true
	else
		os.__installed_package[package_name] = false
		LOGE("@2Failed!\n")
	end
	
	return false
end

os.require_library = function(lib_name)
	return os.require("mingw-w64-ucrt-x86_64-" .. lib_name)
end

-- fix for lua library auto installation : try to install, if not existed.
local __require = require
function require(mod_name, package_name)
	if package.loaded[mod_name] ~= nil then
		return package.loaded[mod_name]
	else
		local IsExisted, mod = pcall(__require, mod_name)
		
		if IsExisted then
			return mod
		else
			-- try to install from luarocks
			if package_name ~= nil then
				LOGI("@3*I: Try to install required '" .. package_name .. "' Lua package, please wait... ")
				local sMsg = String(exec("luarocks install " .. package_name))
				
				sMsg:TrimLeft("\r\n ")
				sMsg:CutBack("\n", true)
				
				if sMsg:CompareFront("Error:") then
					LOGE("@2Failed!\n")
					sMsg:CutFront("Error: ")
					LOGE(sMsg.s)
					os.exit(1)
				end

				IsExisted, mod = pcall(__require, mod_name)
				
				if IsExisted then
					LOGI("@4Done.\n")
					return mod
				else
					LOGE("Got unexpected error\n" .. mod)
				end
			else
				LOGE("@2" .. mod)
			end
		end
	end
	os.exit(1)
end

-- lfs extension utils
lfs.IsExist = function(path)
	return (lfs.attributes(path) ~= nil)
end

lfs.FullPath = function(path)
	local sCurrentPath = lfs.currentdir()
	
	if (path ~= nil) and (lfs.chdir(path) == true) then
		local sFullPath	= lfs.currentdir()
		lfs.chdir(sCurrentPath)
		return sFullPath
	end
	
	return nil
end

-- sort by key
local function __collect_keys(t, sort)
	local _k = {}
	for k in pairs(t) do
		_k[#_k+1] = k
	end
	table.sort(_k, sort)
	return _k
end

function key_pairs(t)
	local keys = __collect_keys(t, __key_compare)
	local i = 0
	return function()
		i = i+1
		if keys[i] then
			return keys[i], t[keys[i]]
		end
	end
end
