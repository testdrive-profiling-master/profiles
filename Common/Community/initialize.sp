profile.tree "Community" {
	network ("TestDrive Wiki",		"testdrive_wiki.sp");
	network ("Telegram",			"telegram.sp");
	network ("Hangout",				"hanouts.sp");
	network ("Gmail",				"gmail.sp");
	network ("Calendar",			"calendar.sp");
}

profile.tree "System" {
	profile ("MinGW Shell",			"%TESTDRIVE_DIR%bin\\msys64\\mingw64.exe");
}

profile.call		"testdrive_wiki.sp"

if.file "..\\private\\initialize.sp"
profile.call		"..\\private\\initialize.sp"
if.end
