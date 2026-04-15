@echo off
if not exist vmlinuz (
	explorer .
	echo Need a installation, please read the 'README.md' first.
	goto EXIT
)
start /WAIT "" cmd /c "codegen qemu boot"
echo Done!
:EXIT
