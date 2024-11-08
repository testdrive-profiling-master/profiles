-- 엑셀 테이블 읽기
xls = DocExcel()

if xls:Open("table.xlsx") then
	local sheet		= xls:GetSheet("Sheet1")
	local col_width	= 0
	
	-- first row
	sheet:GetRow()
	do
		local str = String("")
		while sheet:GetColumn() do
			str:Append(string.format("%12s", sheet:GetValue(true)))
			col_width	= col_width + 1
		end
		print(str.s)
	end
	
	-- rest rows
	while sheet:GetRow() do
		local str = String("")
		 for i=1,col_width do
			sheet:GetColumn()
			str:Append(string.format("%12s", sheet:GetValue(true)))
		end
		print(str.s)
	end
end
