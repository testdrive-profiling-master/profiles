--[[
폰트 다운로드 : https://fontawesome.com/download
아이콘 검색 : https://fontawesome.com/search

다운 받은 폰트와 icons.json 파일을 이곳에 복사해 둔다.
--]]
local cjson = require ("cjson", "lua-cjson")

local t = nil

do
	local sJson = String()
	local f		= TextFile()
	f:Open("icons.json")
	t = cjson.decode(f:GetAll())
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

for key, v in key_pairs(t) do
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
