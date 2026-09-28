@echo off
if not exist vmlinuz (
	explorer .
	echo Need a installation, please read the 'README.md' first.
	goto EXIT
)

powershell -Command "Start-Process cmd -ArgumentList '/c', 'title BusyBox (TestDrive) && call codegen qemu boot' -Wait"

echo *I: BusyBox is down!
:EXIT
