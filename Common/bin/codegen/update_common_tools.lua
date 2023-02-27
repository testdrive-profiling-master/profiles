tool_path		= os.getenv("TESTDRIVE_DIR") .. "bin/"
profile_path	= os.getenv("TESTDRIVE_PROFILE")
project_path	= os.getenv("PROJECT")

if profile_path == nil or project_path == nil then
	os.exit(1)
end

-- fix linux style path representation
project_path	= String(project_path)
project_path:Replace("\\", "/", true)
project_path	= project_path.s

function UpdateTools(src_path, dst_path, cur_dir, local_path, iDepth)
	do	-- make sure destination directory path
		local attr = lfs.attributes(dst_path .. cur_dir .. "/")
		
		if attr == nil then	-- no directory
			lfs.mkdir(dst_path .. cur_dir)
		elseif attr.mode ~= "directory" then -- not a directory
			LOGW("Can't install tools on '" .. dst_path .. cur_dir .. "', it's not a directory.")
			return
		end
	end
	
	-- update local path
	if iDepth ~= 0 then
		local_path	= local_path .. cur_dir .. "/"
	end
	
	-- update resources
	for fs in lfs.dir(src_path .. cur_dir) do
		local sSrc = String(fs)
		
		if sSrc:CompareFront(".") == false then
			local src_attr	= lfs.attributes(src_path .. cur_dir .. "/" ..fs)
			local dst_attr	= lfs.attributes(dst_path .. cur_dir .. "/" ..fs)
			assert (type(src_attr) == "table")
			
			if src_attr.mode == "directory" then
				UpdateTools(src_path .. cur_dir .. "/", dst_path .. cur_dir .. "/", fs, local_path, iDepth + 1)
			elseif dst_attr == nil then
				print("- New tool resource : " .. local_path .. fs)
				os.execute("cp -f \"" .. src_path .. cur_dir .."/" .. fs .. "\" \"" .. dst_path .. cur_dir .. "\"")
			elseif tonumber(dst_attr.modification) < tonumber(src_attr.modification) then
				print("- Update tool resource : " .. local_path .. fs)
				os.execute("cp -f \"" .. src_path .. cur_dir .."/" .. fs .. "\" \"" .. dst_path .. cur_dir .. "\"")
			end
		end
	end
end

UpdateTools(project_path, profile_path .. "Common/", "bin", "", 0)

do	-- check deprecated tool resources
	local f = TextFile()
	if f:Open(project_path .. "bin/.deprecated") then
		while f:IsEOF() == false do
			local	sPath = String(f:Get())
			sPath:Trim(" \t\r\n")
			sPath:Replace("\\", "/", true)
			
			if sPath:Length() ~= 0 then
				local bDirectory = sPath:CompareBack("/*")
				
				sPath:insert(0, profile_path .. "Common/bin/")
				
				if bDirectory then
					sPath:DeleteBack("/*")
					local attr = lfs.attributes(sPath.s)
					if attr ~= nil and attr.mode == "directory" then
						LOGI("Deprecated tool directory : " .. sPath.s)
						os.execute("rm -rf \"" .. sPath.s .."\"")
					end
				else
					local attr = lfs.attributes(sPath.s)
					if attr ~= nil and attr.mode == "file" then
						
						LOGI("Deprecated tool file : " .. sPath.s)
						os.execute("rm -f \"" .. sPath.s .."\"")
					end
				end
			end
		end
		f:Close()
	end
end

function CheckMsys64Package(dep_file, package_name)
	if lfs.attributes(tool_path .. "msys64/ucrt64/" .. dep_file) == nil then
		LOGI(package_name)
	end
end

if lfs.attributes(project_path .. "bin/.uldate_extra_tools.lua") ~= nil then
	RunScript(project_path .. "bin/.uldate_extra_tools.lua")
end
