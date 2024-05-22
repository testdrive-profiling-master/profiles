-- 문서 속성들
property["Document_Name"]			= "UserGuide"
property["IP_Version"]				= "1.00"
property["Main_Title"]				= "Docgen"
property["Sub_Title"]				= "(Document Generator)"
property["IP_Name_First_Page"]		= "User Guide"
property["IP_Name_Header"]			= "document_generator"
property["Ownership"]				= "clonextop@gmail.com"
property["Document_Name_Header"]	= "userguide"
property["Security_Level"]			= "BSD License."
--property["Water_Mark"]				= "TESTDRIVE OPENSOURCE"		-- 워터마크를 쓰지 않으려면 이부분을 주석 처리 합니다.

-- Revision history (버전, 연도, 월, 일, 설명) : 마지막 버전/날짜가 도큐먼트의 버전/날짜가 됩니다. 날짜 순으로 나열할 것!!!
AddRevision("1.0",	2019,  2,  4,	"Initial relase")
AddRevision("1.1",	2020,  4,  2,	"add Visio page to picture & bookmark automation")
AddRevision("1.2",	2021,  7,  3,	"add word's @<b>@@<property>@</b> tag variable")
AddRevision("1.3",	2021, 11, 12,	"support excel table")
AddRevision("1.4",	2024,  2,  5,	"add lua function call in paragraph")
AddRevision("1.5",	2024,  4,  9,	"add lua snippet option on command line")
AddRevision("1.6",	2024,  5,  17,	"Markdown can be specified as the input source file")
AddRevision("1.7",	2024,  5,  21,	"add Excel chart image reference and inline code expression")
AddRevision("1.8",	2024,  5,  22,	"add export file format : html, odt, rtf, xml, txt")


-- Term 추가
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://ko.wikipedia.org/wiki/%EB%A3%A8%EC%95%84_(%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4);Wiki>, @<link:http://www.lua.org/;Homepage>)")
AddTerm("WORD", "Microsft Office's word processor")
AddTerm("Visio", "Microsoft Office's Visio\
- Diagramming and vector graphics application")
AddTerm("PDF", "Portable Document Format (@<link:https://ko.wikipedia.org/wiki/PDF;Wiki>)")
AddTerm("markdown", "Mark-up language (@<link:https://ko.wikipedia.org/wiki/%EB%A7%88%ED%81%AC%EB%8B%A4%EC%9A%B4;Wiki>)")
AddTerm("VBA", "Microsoft's Visual Basic for Application")


lua_table_example = {
	HeaderCount = 2,
	{{"Head A", merge={1,2}}, {"Head B", merge={3,1}}, {width=300, style="TableTextCenter"}, ""},
	{"", "Sub A", "Sub B", "Sub C"},
	{"index1", "a", "1", "aa"},
	{"index2", "b", "2", "bb"},
	{"index3", {"c", merge={2,1}}, "", "@<color:ff0000>cc@</color>"}
}

docgen.code_type	= "lua"

-- 챕터 추가
AddParagraph("[[src/Introduction.md]]")
AddParagraph("[[src/Lua_expression.md]]")
AddParagraph("[[src/Pragraph_expression.md]]")
