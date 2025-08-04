@echo off
start /b %TESTDRIVE_DIR%bin\HexEdit.exe MMAP://%PROJECT_NAME%/80000000/%PROJECT_NAME% system memory
