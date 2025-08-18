#lua
LOG_CLEAR()
--system.call	"cleanup_all.bat"
System.Execute("HierarchicalMake", ". -arg \"clean\"", "%TESTDRIVE_PROFILE%Common\\")

--LOG_CLEAR()
LOGI("◈ System clean-up is done!")
