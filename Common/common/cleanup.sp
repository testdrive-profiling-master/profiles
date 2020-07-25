//system.clear
//system.call	"cleanup_all.bat"
system.call	"%TESTDRIVE_DIR%bin\\HierarchicalMake.exe", ". -arg \"dust_clean\"", "%TESTDRIVE_PROFILE%Common\\"

//system.clear
system.msg "System clean-up is done!\n"
