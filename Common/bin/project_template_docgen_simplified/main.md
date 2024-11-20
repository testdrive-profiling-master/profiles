```[lua]
-- *** inline lua script ***
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
```


# Introduction

If you have any ideas with this, please make suggestions to Hyunng-Ki Jeong(@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>).



 
## Main functions

Do something valuable...

* Do 1
* Do 2
** Do Do 1
**> Do Do Do.


:::Note
note me!


# first Contents

It's first @<b>@<color:FF0000>contents@</color>@</b>. \
Good luck!

```cpp
#include <stdio.h>

int main(int argc, const char* argv[]) {
	printf("Hello simplified docgen!!!\n");
	return 0;
}
```

 
## media contents
 
@<img:#media/docgen.png;0.5;docgen image>

 
@<tbl:media/tables.xlsx;table_1;table example>

 
@<img:media/tables.xlsx[chart:Population in the world];0.9;chart example>