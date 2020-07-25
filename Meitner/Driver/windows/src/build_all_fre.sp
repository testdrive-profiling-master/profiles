system.clear

system.msg	"For Free(x86).\n"
system.call	".\\build_driver.bat", "fre x86 WIN7 objfre_win7_x86\\i386", ".\\", " : error ", "*E: "
system.msg	"For Free(x64).\n"
system.call	".\\build_driver.bat", "fre x64 WIN7 objfre_win7_amd64\\amd64", ".\\", " : error ", "*E: "

system.msg	"WDM Windows driver (Free) build is done.\n"