@echo off
SETLOCAL

cd %SIM_TOP_PATH%

verilator bake

ENDLOCAL
