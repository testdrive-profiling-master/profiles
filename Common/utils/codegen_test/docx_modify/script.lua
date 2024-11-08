local doc 	= DocWord()

LOGI("O")
if doc:Open("input.docx") then

LOGI("X")
	local	aaa = 3.5

	-- 문자열 변경
	--[[
	local text_mod	= VL_String("", "")
	text_mod:Add(VL_String("TARGET", "hyungKi.jeong"))
	text_mod:Add(VL_String("INDEX_1", "A0"))
	text_mod:Add(VL_String("INDEX_2", "A1"))
	text_mod:Add(VL_String("DATA_1_2", string.format("%d", aaa) .. "Mbps"))
	text_mod:Add(VL_String("DATA_2_3", "Mike"))
	--]]
	--doc:Modify(text_mod)
LOGI("T")
	-- 그림 변경
	doc:ReplaceImage("IMAGE FIRST", "image/image_B.png")

	-- 서브 문서 변경
	LOGI("A")
	doc:ReplaceSubDocument("table", "sub_documents/table.2.docx")
LOGI("B")
	-- 저장
	LOGI("Saving...")
	doc:Save("output.docx")
else
	LOGE("Can't open 'input.docx'")
end
