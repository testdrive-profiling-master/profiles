@echo off
del /S /Q	%1\*.d
del /S /Q	%1\*.o
del /Q		%1\*.user
del /Q		%1\*.sdf
del /Q /AH	%1\*.suo
rmdir /S /Q	%1\ipch
rmdir /S /Q	%1\Debug
rmdir /S /Q	%1\Release
rmdir /S /Q	%1\x64
rmdir /S /Q	%1\.settings