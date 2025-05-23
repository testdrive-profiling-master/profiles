-- 문서 속성들
docgen.property["Document_Name"]			= "UserGuide"
docgen.property["IP_Version"]				= "1.00"
docgen.property["Main_Title"]				= "Codegen"
docgen.property["Sub_Title"]				= "(Code Generator)"
docgen.property["IP_Name_First_Page"]		= "User Guide"
docgen.property["IP_Name_Header"]			= "code generator"
docgen.property["Ownership"]				= "clonextop@gmail.com"
docgen.property["Document_Name_Header"]		= "userguide"
docgen.property["Security_Level"]			= "BSD License."
--docgen.property["Water_Mark"]				= "TESTDRIVE OPENSOURCE"		-- 워터마크를 쓰지 않으려면 이부분을 주석 처리 합니다.

-- Revision history (버전, 연도, 월, 일, 설명) : 마지막 버전/날짜가 도큐먼트의 버전/날짜가 됩니다. 날짜 순으로 나열할 것!!!
AddRevision("1.0",	2023, 5,  7,	"Initial release")

-- Term 추가
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://ko.wikipedia.org/wiki/%EB%A3%A8%EC%95%84_(%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4);Wiki>, @<link:http://www.lua.org/;Homepage>)")

-- 챕터 추가
AddParagraph("[[src/introduction.md]]")
AddParagraph("[[src/function_global.md]]")
AddParagraph("[[src/function_String.md]]")
AddParagraph("[[src/function_TextFile.md]]")
AddParagraph("[[src/function_ArgTable.md]]")
AddParagraph("[[src/function_Clipper.md]]")
AddParagraph("[[src/function_Document.md]]")
AddParagraph("[[src/function_Git.md]]")
AddParagraph("[[src/function_Html.md]]")