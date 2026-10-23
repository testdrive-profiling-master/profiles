local Arg = ArgTable("UEFI Disk Generator")

Arg:AddOptionFile	("efi_file", nil, nil, nil, "efi_file", "input .efi/.inf file name")

if (Arg:DoParse() == false) then
	return
end

local efi_filename	= String(Arg:GetOptionFile("efi_file", 0))
local base_name		= String()

if efi_filename:CompareBack(".inf") then
	efi_filename:MakeFullPath()
	base_name:GetEnvironment("BASE_NAME@Defines@".. efi_filename.s)
	if #base_name.s == 0 then
		LOGE(efi_filename.s .. " file does not have BASE_NAME.")
		os.exit(1)
	end
	efi_filename.s = "bin/" .. base_name.s .. ".efi"
else
	base_name.s = efi_filename.s
	base_name:CutBack(".")
	base_name:CutFront("/", true)
	base_name:CutFront("\\", true)
end

if lfs.IsExist(efi_filename.s) == false then
	LOGE("EFI file is not existed. : " .. efi_filename.s)
	os.exit(1)
end
base_name		= base_name.s
efi_filename	= efi_filename.s

if lfs.IsExist("S:\\EFI\\BOOT\\bootx64.efi") == false then
	local usb_list = {}
	LOGI("Generate new USB disk for " .. base_name .. ", Searching USB disk...")
	do	-- usb 1개만 얻기 (128GB 이하)
		local sList = String(exec("chcp 437 >nul & echo list disk | diskpart | findstr /R /C:\"Disk [0-9]\""))
		sList:Trim(" \t\r\n")

		local	sItemDelim	= "\r\n"
		local	sTokenDelim	= " \t"
		local	sItem		= sList:Tokenize(sItemDelim)
		
		while sItem:IsEmpty() == false do
			if sItem:Tokenize(sTokenDelim).s == "Disk" then
				local iDiskNum = tonumber(sItem:Tokenize(sTokenDelim).s)
				if sItem:Tokenize(sTokenDelim).s == "Online" then
					local iGB = tonumber(sItem:Tokenize(sTokenDelim).s)
					
					if (iGB <= 128) and (sItem:Tokenize(sTokenDelim).s == "GB") then
						local sType = String(exec("chcp 437 >nul & (echo select disk " .. iDiskNum .. " && echo detail disk) | diskpart | findstr \"Type\""))
						sType:CutFront(":")
						sType:CutBack("\n", true)
						sType:Trim(" \t\r\n")
						
						if sType.s == "USB" then
							usb_list[#usb_list + 1] = {id=iDiskNum, size=iGB}
						end
					end
				end
			end
			sItem = sList:Tokenize(sItemDelim)
		end
	end

	-- only 1 USB disk is allowed.
	if #usb_list == 0 then
		LOGE("No USB disk...")
		os.exit(1)
	elseif #usb_list > 1 then
		LOGE("Too many USB disk is attached on this machine.")
		os.exit(1)
	end

	LOGI("USB#" .. usb_list[1].id .. "[" .. usb_list[1].size .. "GB] is found.")
	LOGI("Create patitions...")
	exec(
		"chcp 437 >nul & (" ..
		"echo select disk " .. usb_list[1].id .. " & " ..
		"echo clean & " ..
		"echo convert gpt &" ..
		"echo create partition efi size=128 &" ..
		"echo create partition primary" ..
		") | diskpart"
	)
	Sleep(500)
	LOGI("Formatting...")
	exec(
		"chcp 437 >nul & (" ..
		"echo select disk " .. usb_list[1].id .. " & " ..
		"echo select partition 1 & " ..
		"echo set id=ebd0a0a2-b9e5-4433-87c0-68b6b72699c7 & " ..
		"echo assign letter=S & " ..
		"echo format fs=fat32 quick label=\"EFI_SYS\" & " ..
		"echo set id=c12a7328-f81f-11d2-ba4b-00a0c93ec93b & " ..
		"echo select partition 2 & " ..
		"echo format fs=fat32 quick label=\"EFI_DATA\" & " ..
		"echo assign letter=T" ..
		") | diskpart"
	)
	exec("mkdir S:\\EFI\\BOOT")
else
	LOGI("Already USB disk is ready! Just copy EFI file.")
end

exec("cp -f \"" .. efi_filename .. "\" S:\\EFI\\BOOT\\bootx64.efi")
LOGI("Done!")
