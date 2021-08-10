//system.clear
//system.call	"cleanup_all.bat"
system.call	"HierarchicalMake", ". -arg \"clean\"", "%TESTDRIVE_PROFILE%Common\\"

//system.clear
system.msg "System clean-up is done!\n"
