local xlsx 	= DocExcel()

if xlsx:Open("input.xlsx") then
	local sheet1	= xlsx:GetSheet("modify")

	-- sheet 출력
	if (sheet1 ~= nil) then
		while(sheet1:GetRow(false)) do
			local s = "\t";
			while(sheet1:GetColumn(false)) do
				s	= s .. string.format("%-15s", sheet1:GetValue())
				
				-- "A1" 부분 "Test." 로 변경하기
				if (sheet1:GetValue() == "A1") then
					sheet1:SetString("Test.")
				end
				
				if (sheet1:GetValue() == "23") then
					sheet1:SetInt("500")
				end
			end
			
			print(s)
		end
		-- 저장
		xlsx:Save("output.xlsx")
	else
		LOGE("Can't find sheet 'Modify'.")
	end
else
	LOGE("Can't open excel file.")
end