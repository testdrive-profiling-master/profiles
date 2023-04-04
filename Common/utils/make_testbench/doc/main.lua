-- 문서 속성들
property["Document_Name"]			= "UserGuide"
property["IP_Version"]				= "1.00"
property["Comprehensive_IP_Title"]	= "Make Testbench from Waveform"
property["IP_Name_First_Page"]		= "User Guide"
property["Business_Unit"]			= "sw"
property["IP_Name_Header"]			= "document_generator"
property["Ownership"]				= "clonextop@gmail.com"
property["Document_Name_Header"]	= "userguide"
property["Security_Level"]			= "BSD License + GPLv2"
--property["Water_Mark"]				= "TESTDRIVE OPENSOURCE"		-- 워터마크를 쓰지 않으려면 이부분을 주석 처리 합니다.

AddRevision("1.0",	2023, 4,  4,	"Initial relase")


-- Term 추가
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("GTKWave", "GTKWave (@<link:https://gtkwave.sourceforge.net/>)")

-- 챕터 추가
AddParagraph("[[src/소개.md]]")
AddParagraph("[[src/사용법.md]]")
