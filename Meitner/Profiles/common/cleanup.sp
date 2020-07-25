system.clear
system.msg "Clean up system...\n"

system.call	"cleanup_all.bat"
system.call	"%TESTDRIVE_DIR%bin\\HierarchicalMake.exe", ". -arg \"clean\"", "%PROJECT%Application\\"
system.call	"%TESTDRIVE_DIR%bin\\HierarchicalMake.exe", ". -arg \"dust_clean\"", "%PROJECT%Program\\process_filter\\"
system.call	"%TESTDRIVE_DIR%bin\\HierarchicalMake.exe", ". -arg \"clean\"", "%PROJECT%System\\SubSystems\\"

profile.call "%TESTDRIVE_PROFILE%\\common\\common\\cleanup.sp"

system.msg "Ready for default system libraries...\n"
profile.call "%TESTDRIVE_PROFILE%\\common\\System\\initialize.sp"

system.clear
system.msg "System clean-up is done!\n"
