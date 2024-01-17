-- 주요 폴더 리스트 얻기
local main_dir		= "../../../"				-- 탐색 대상
local file_name		= "dir_list.txt"
local PrevList		= VL_String("list","")

t = CreateFileList(main_dir, 1, false, nil)	 -- 하위 탐색 폴더 깊이가 현재는 1

-- 이전에 만든 목록 얻기
do
	local f = TextFile()
	f:Open(file_name)

	while(true)
	do
		local s = f:Get(true)
		
		if (s == nil) then
			break
		end
		
		PrevList:Add(VL_String(s, ""))
	end
end

-- 새로운 목록 갱신
print("Checking...\n")
local new_dir_list	= ""
do
	local IsNew = false
	local f = TextFile()
	f:Create(file_name)
	while(t:Size() ~= 0)
	do
		local	v = t:Pop()
		
		if (v.name == "dir") then
			f:Put(v.data.."\n")

			-- 이전거랑 비교
			if(PrevList:Find(v.data) == nil) then -- 이전거에서 찾기
				print(" * New Dir : "..v.data)    -- 없으면 출력
				IsNew	= true
				new_dir_list	= new_dir_list .. "\t"..v.data .."\n"
			end
		end
	end
	
	if(IsNew == false) then
		print("*I: No difference.")
	else
		if(package.config:sub(1,1) == "/") then	  -- 리눅스의 경우
			os.execute("notify-send \"새로운 폴더 있음.\" \"" .. new_dir_list .. "\"")
		end
	end
end
