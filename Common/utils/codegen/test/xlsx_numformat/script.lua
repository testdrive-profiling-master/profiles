local xlsx 	= DocExcel()

if xlsx:Open("input.xlsx") then
	local sheet1	= xlsx:GetSheet("Sheet1")

	-- sheet 출력
	if (sheet1 ~= nil) then
		while(sheet1:GetRow(false)) do
			local	s		= "\t"
			local	iCol	= 0
			local	sFormat	= ""
			while(sheet1:GetColumn(false)) do
				s		= s .. string.format("%-30s", sheet1:GetValue())
				if iCol == 1 then
					sFormat	= sheet1:GetNumberFormat()
				end
				
				iCol	= iCol + 1
			end
			
			print("Number format = '" .. sFormat .. "'\n" .. s)
		end
	else
		LOGE("Can't open the sheet.")
	end
else
	LOGE("Can't open excel file.")
end