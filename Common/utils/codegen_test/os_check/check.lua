local function GetOS()
	local os_name = 'windows'

	if IsWindows() == false then
		local raw_os_name = ''
		
		-- LuaJIT shortcut
		if jit and jit.os and jit.arch then
			raw_os_name = jit.os
		else
			-- is popen supported?
			local popen_status, popen_result = pcall(io.popen, "")
			if popen_status then
				popen_result:close()
				-- Unix-based OS
				raw_os_name = io.popen('uname -s','r'):read('*l')
			else
				os_name	= 'unknown'
			end
		end

		raw_os_name = (raw_os_name):lower()

		local os_patterns = {
			['linux']	= 'Linux',
			['mac']		= 'Mac',
			['darwin']	= 'Mac',
			['bsd$']	= 'BSD',
			['SunOS']	= 'Solaris',
		}

		for pattern, name in pairs(os_patterns) do
			if raw_os_name:match(pattern) then
				os_name = name
				break
			end
		end
	end
	return os_name
end

LOGI("OS : " .. GetOS())