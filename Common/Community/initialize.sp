profile.tree "Community" {
	network ("TestDrive Wiki",		"testdrive_wiki.sp");
}

profile.tree "System" {
	profile ("MinGW Shell",			"%TESTDRIVE_DIR%bin\\msys64\\ucrt64.exe");
}

profile.call		"testdrive_wiki.sp"

if.file "..\\private\\initialize.sp"
profile.call		"..\\private\\initialize.sp"
if.end
