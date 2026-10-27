debug: $(SRCS)
	@edk2.bat build -a $(TARGET_ARCH) -b DEBUG

release: $(SRCS)
	@edk2.bat build -a $(TARGET_ARCH) -b RELEASE

clean:
	@edk2.bat clean

ifeq ($(PROJECT_TYPE), APPLICATION)
usb:
	@edk2.bat build -a $(TARGET_ARCH) -b RELEASE
	@run_as_admin codegen make_uefi_disk $(PROJECT).inf
	@echo Done!

qemu:
	@edk2.bat qemu -m $(PROJECT).inf
	@echo Done!
endif

help:
	@echo "** Makefile command list **"
	@echo "   help    : Display this help and exit"
	@echo "   debug   : Build project debug(default) mode"
	@echo "   release : Build project release mode"
	@echo "   clean   : Clean project"
ifeq ($(PROJECT_TYPE), APPLICATION)
	@echo "   usb     : Make physical USB booting disk"
	@echo "   qemu    : Make QEMU environment for test"
endif
