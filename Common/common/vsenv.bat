@echo off

:VS_2010
if "%VS100COMNTOOLS%XXX"=="XXX" goto VS_2012
call "%VS100COMNTOOLS%vsvars32.bat"
goto VS_BUILD

:VS_2012
if "%VS110COMNTOOLS%XXX"=="XXX" goto VS_2013
call "%VS110COMNTOOLS%vsvars32.bat"
goto VS_BUILD

:VS_2013
if "%VS120COMNTOOLS%XXX"=="XXX" goto NO_VS_TOOLS
call "%VS120COMNTOOLS%vsvars32.bat"
goto VS_BUILD

:VS_BUILD
@rem copy /y %1 %1.old.sln
@rem devenv %1 /Upgrade
devenv %1 %2 %3
goto EXIT

:NO_VS_TOOLS
echo *E  : fatal error, There is no visual studio tools.
goto EXIT

:EXIT
