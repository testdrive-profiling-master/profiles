-- Document properties
property["Document_Name"]			= "UserGuide"
property["IP_Version"]				= "1.00"
property["Main_Title"]				= "TITLE"
property["Sub_Title"]				= "(TITLE Sub Title)"
property["IP_Name_First_Page"]		= "User Guide"
property["IP_Name_Header"]			= "Header Name"
property["Ownership"]				= "someone@mail.com"
property["Document_Name_Header"]	= "userguide"
property["Security_Level"]			= "BSD License."
--property["Water_Mark"]				= "TESTDRIVE OPENSOURCE"		-- If you do not want to use a watermark, comment out this part.

-- Revision history
AddRevision("1.0",	INITIAL_YEAR,  INITIAL_MONTH,  INITIAL_DAY,	"Initial relase")



-- Terms
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://en.wikipedia.org/wiki/Lua_(programming_language);Wiki>, @<link:http://www.lua.org/;Homepage>)")


-- Chapters
AddParagraph("[[src/Introduction.md]]")
AddParagraph("[[src/Contents.md]]")
