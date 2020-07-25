@echo off
SETLOCAL

for /f "tokens=*" %%I in ('dir /b /ad %2') do (
	call %1 %2\%%I
)

ENDLOCAL
