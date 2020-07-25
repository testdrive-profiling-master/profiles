@echo off
cd Program\
if exist "%SIM_OUTPUT_FILE%" goto RUN_GTKWAVE
echo *E: VCD file(%SIM_OUTPUT_FILE%) is not found.
pause
goto END

:RUN_GTKWAVE
@echo Opening Waveform file(%SIM_OUTPUT_FILE%)...
start "View Waveform" "%TESTDRIVE_DIR%bin\msys64\mingw64\bin\gtkwave.exe" -r %TESTDRIVE_DIR%bin\gtkwave.ini -g %SIM_OUTPUT_FILE% %SIM_DO_FILE%

:END
