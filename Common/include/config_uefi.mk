all: $(SRCS)
	@edk2.bat build -a $(TARGET_ARCH) -b $(BUILD_TARGET)

clean:
	@edk2.bat clean

ifeq ($(PROJECT_TYPE), APPLICATION)
usb:
	@run_as_admin codegen make_uefi_disk $(PROJECT).inf
	@echo Done!

qemu:
	@edk2.bat qemu -m $(PROJECT).inf
	@echo Done!
endif

help:
	@echo "   help  : Display this help and exit"
	@echo "   all   : Build project DEBUG(default)/RELEASE"
	@echo "           * Options"
	@echo "             BUILD_TARGET={RELEASE/DEBUG}"
	@echo "   clean : Clean project"
ifeq ($(PROJECT_TYPE), APPLICATION)
	@echo "   usb   : Make physical USB booting disk"
	@echo "   qemu  : Make QEMU environment for test"
endif
