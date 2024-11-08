-- text 테이블 읽기
local t = CreateFromTable("table.txt", "name")

-- 파일 만들기
do
	local f = TextFile()
	f:Create("output.txt")

	while(t:Size() ~= 0)
	do
		local	v = t:Pop()
		
		if(tonumber(v:Find("age").data) < 50) then	-- table 에서 50 살 미만만 출력하기
			v.data	= "$(name:%-20s) $(age:%20s) $(class:%20s)  $(sex:%20s)";
			print("Item : "..v:Data())		-- 출력
			f:Put(v:Data().."\n");			-- 파일 쓰기
		end
	end
end
