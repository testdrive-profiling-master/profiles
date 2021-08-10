system.clear
system.msg "Clean up system...\n"

system.call	"cleanup_all.bat"
system.call	"HierarchicalMake", ". -arg \"clean\"", "%PROJECT%Application\\"
system.call	"HierarchicalMake", ". -arg \"clean\"", "%PROJECT%Program\\process_filter\\"
system.call	"HierarchicalMake", ". -arg \"clean\"", "%PROJECT%System\\SubSystems\\"

profile.call "%TESTDRIVE_PROFILE%\\common\\common\\cleanup.sp"

system.msg "Ready for default system libraries...\n"
profile.call "%TESTDRIVE_PROFILE%\\common\\System\\initialize.sp"

system.clear
system.msg "System clean-up is done!\n"
