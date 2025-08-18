#lua

ProfileTree("Community"):AddItem("network", "TestDrive Wiki", "testdrive_wiki.sp")
ProfileTree("System"):AddItem("profile", "MinGW Shell", "%TESTDRIVE_DIR%bin\\msys64\\ucrt64.exe")

System.CallProfile("testdrive_wiki.sp")

if lfs.attributes("..\\private\\initialize.sp", "mode") == "file" then
	System.CallProfile("..\\private\\initialize.sp")
end
