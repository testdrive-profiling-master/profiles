@echo off
del /Q src\SimTop*
verilator %1 -sv -cc --compiler gcc -Mdir ./src -prefix SimTop -I%PROJECT%System/HDL  --top-module top
