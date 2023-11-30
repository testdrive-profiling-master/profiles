-- 엑셀 테이블 읽기
xls = DocExcel()

if xls:Open("table.xlsx") then
	local sheet = xls:GetSheet("Sheet1")
	
	while sheet:GetRow() do
		local str = String("")
		while sheet:GetColumn() do
			str:Append(string.format("%12s", sheet:GetValue()))
		end
		print(str.s)
	end
end
