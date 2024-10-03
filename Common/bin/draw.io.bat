@echo off
codegen check_drawio

Echo.%* | findstr /C:"-">nul && (
    "%TESTDRIVE_PROFILE%\Common\bin\drawio\draw.io.exe" %*
) || (
    start /B "draw.io" "%TESTDRIVE_PROFILE%\Common\bin\drawio\draw.io.exe" %*
)
