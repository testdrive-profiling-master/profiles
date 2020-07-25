@echo off
del /Q src\*.*
%TESTDRIVE_DIR%bin\\verilator.exe %1 -sv -cc --compiler gcc -Mdir ./src -prefix SimTop -I%PROJECT%System/HDL  --top-module top
