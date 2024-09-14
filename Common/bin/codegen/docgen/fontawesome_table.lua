--[[
폰트 다운로드 : https://fontawesome.com/download
아이콘 검색 : https://fontawesome.com/search

다운 받은 폰트와 icons.json 파일을 이곳에 복사해 둔다.
폰트는 otf -> ttf 로 변경한 뒤, 다시 .odttf 로 변환하여 encrypted 된 폰트로 변경하고 암호키를 확인해야 한다.
--]]
local cjson			= require ("cjson", "lua-cjson")
local font_json		= nil

do
	local sJson = String()
	local f		= TextFile()
	if f:Open("icons.json") == false then
		error("No 'icons.json' file.")
	end
	font_json = cjson.decode(f:GetAll())
	f:Close()
end

function GetStringID(t, s)
	for i, val in ipairs(t) do
		if val == s then
			return i
		end
	end
	
	return 0
end


local awesome_font	= {}

for key, v in key_pairs(font_json) do
	if #v.free > 0 then
		for _, font_name in ipairs(v.styles) do
			local font_id = GetStringID(v.free, font_name)	-- is free?
			if font_id > 0 then
				print("     " .. font_name)
			end
		end
		--utf8.char(9658)
	end
end
