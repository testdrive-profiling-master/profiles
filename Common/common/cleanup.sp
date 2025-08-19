#lua
System.Execute("cleanup_all.bat")
LOG_CLEAR()
System.Execute("HierarchicalMake", ". -arg \"clean\"", "%TESTDRIVE_PROFILE%Common\\")
System.Execute("HierarchicalMake", ". -arg \"clean\"", "%PROJECT%Application\\")
System.Execute("HierarchicalMake", ". -arg \"clean\"", "%PROJECT%System\\SubSystems\\")

LOGI("¢Â System clean-up is done!")
