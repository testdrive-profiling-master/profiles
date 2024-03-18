-- document properties
property["Document_Name"]			= "Verigen user guide"
property["IP_Version"]				= "1.04"
property["Main_Title"]				= "Verigen"
property["Sub_Title"]				= "(verilog generator)"
property["IP_Name_First_Page"]		= "User Guide"
property["IP_Name_Header"]			= "document_generator"
property["Ownership"]				= "clonextop@gmail.com"
property["Document_Name_Header"]	= "userguide"
property["Security_Level"]			= "BSD License."
--property["Water_Mark"]				= "TESTDRIVE OPENSOURCE"		-- watermark text, if you don't wan't it, comment it.

-- Revision history (버전, 연도, 월, 일, 설명) : 마지막 버전/날짜가 도큐먼트의 버전/날짜가 됩니다. 날짜 순으로 나열할 것!!!
AddRevision("1.00",	2023, 5,  8,	"Initial relase.")
AddRevision("1.01",	2023, 6,  8,	"add interface:set_top_uppercase function.")
AddRevision("1.02",	2023, 6,  12,	"add default clock, add vfunctions. ($LOG2, $RANGE, $DEMUX_BY_EN, $MULTICYCLE)")
AddRevision("1.03",	2023, 11, 2,	"fix module definition.")
AddRevision("1.04",	2024, 3, 18,	"Combines set_modport/add_modport functions into one and extends functionality.")

-- Term 추가
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://ko.wikipedia.org/wiki/%EB%A3%A8%EC%95%84_(%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4);Wiki>, @<link:http://www.lua.org/;Homepage>)")

-- 챕터 추가
AddParagraph("[[src/introduction.md]]")
AddParagraph("[[src/fast_follow.md]]")
AddParagraph("[[src/macros.md]]")
AddParagraph("[[src/instructions.md]]")
AddParagraph("[[src/appendix.md]]")
