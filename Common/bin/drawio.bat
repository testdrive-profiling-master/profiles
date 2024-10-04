@echo off
if "%1" == "install" (
	rm -rf "%TESTDRIVE_PROFILE%\Common\bin\drawio"
	codegen check_drawio
	goto END
)

codegen check_drawio

Echo.%* | findstr /C:"-">nul && (
    "%TESTDRIVE_PROFILE%\Common\bin\drawio\draw.io.exe" %*
) || (
    start /B "draw.io" "%TESTDRIVE_PROFILE%\Common\bin\drawio\draw.io.exe" %*
)
:END
