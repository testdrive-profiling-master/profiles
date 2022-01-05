@echo off

@rem install extra libraries
if not exist "%TESTDRIVE_DIR%bin/msys64/ucrt64/include/microhttpd.h" (
	%TESTDRIVE_DIR%bin/msys64/usr/bin/pacman -S --noconfirm mingw-w64-ucrt-x86_64-libmicrohttpd
)
