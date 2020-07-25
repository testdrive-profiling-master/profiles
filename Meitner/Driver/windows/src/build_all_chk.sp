system.clear

system.msg	"For Checked(x86).\n"
system.call	"build_driver.bat", "chk x86 WIN7 objchk_win7_x86\\i386", ".\\", " : error ", "*E: "
system.msg	"For Checked(x64).\n"
system.call	"build_driver.bat", "chk x64 WIN7 objchk_win7_amd64\\amd64", ".\\", " : error ", "*E: "

system.msg	"WDM Windows driver (Checked) build is done.\n"
