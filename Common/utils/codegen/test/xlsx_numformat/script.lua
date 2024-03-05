local xlsx 	= DocExcel()

if xlsx:Open("input.xlsx") then
	local sheet1	= xlsx:GetSheet("Sheet1")

	-- sheet 출력
	if (sheet1 ~= nil) then
		sheet1.debug = true
		while(sheet1:GetRow(false)) do
			local s = "\t";
			while(sheet1:GetColumn(false)) do
				s	= s .. string.format("%-30s", sheet1:GetValue())
			end
			
			print(s)
		end
	else
		LOGE("Can't open the sheet.")
	end
else
	LOGE("Can't open excel file.")
end