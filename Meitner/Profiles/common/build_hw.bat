@echo off
SETLOCAL

if exist "%PROJECT%System\SubSystems\Simulation\verilator\SimTop.cpp" (
	del /Q "%PROJECT%System\SubSystems\Simulation\verilator\Sim*"
)

if "%SIM_WAVE_MODE%"=="File output" (
	echo 	Waveform configuration : File output mode.
	if "%SIM_MULTITHREADING%"=="Disable" (
		set SIM_DEFINITION=%SIM_DEFINITION% --trace-fst -trace-max-width 4096
	) else (
		set SIM_DEFINITION=%SIM_DEFINITION% --trace-fst-thread -trace-max-width 4096
	)
) else if "%SIM_WAVE_MODE%"=="Interactive" (
	echo 	Waveform configuration : Interactive[with 'GTKWave'] mode.
	set SIM_DEFINITION=%SIM_DEFINITION% --trace -trace-max-width 4096
) else (
	echo 	Waveform configuration : None[fastest] mode.
)

@rem half for S/W, half for H/W
set /A thread_count=%NUMBER_OF_PROCESSORS% - 2
if %thread_count% leq 1 set /A thread_count=2

if "%SIM_MULTITHREADING%"=="Enable(HDL)" (
	echo 	Multithreading : Enable[HDL]
	set SIM_DEFINITION=%SIM_DEFINITION% --threads %thread_count% --threads-dpi none
) else if "%SIM_MULTITHREADING%"=="Enable(HDL+DPI)" (
	echo 	Multithreading : Enable[HDL+DPI]
	set SIM_DEFINITION=%SIM_DEFINITION% --threads %thread_count% --threads-dpi all
) else (
	echo 	Multithreading : Disable
)

verilator_bin %SIM_TOP_FILE% %1 %2 %3 %4 %SIM_DEFINITION% -sv -cc --autoflush --compiler gcc --output-split 100000 -Mdir ../SubSystems/Simulation/verilator -prefix SimTop --top-module top
ENDLOCAL