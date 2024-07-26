-- Document properties
docgen.property["Document_Name"]			= "UserGuide"
docgen.property["IP_Version"]				= "1.00"
docgen.property["Main_Title"]				= "TITLE"
docgen.property["Sub_Title"]				= "(TITLE's Sub Title)"
docgen.property["IP_Name_First_Page"]		= "User Guide"
docgen.property["IP_Name_Header"]			= "Header Name"
docgen.property["Ownership"]				= "someone@mail.com"
docgen.property["Document_Name_Header"]		= "userguide"
docgen.property["Security_Level"]			= "BSD License."
--docgen.property["Water_Mark"]				= "TESTDRIVE OPENSOURCE"		-- If you do not want to use a watermark, comment out this part.

-- Revision history
AddRevision("1.0",	INITIAL_YEAR,  INITIAL_MONTH,  INITIAL_DAY,	"Initial relase")



-- Terms
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://en.wikipedia.org/wiki/Lua_(programming_language);Wiki>, @<link:http://www.lua.org/;Homepage>)")


-- Chapters
AddParagraph("[[src/Introduction.md]]")
AddParagraph("[[src/Contents.md]]")
