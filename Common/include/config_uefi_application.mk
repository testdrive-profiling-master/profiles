all: $(SRCS)
	@edk2.bat build -a $(TARGET_ARCH) -b $(BUILD_TARGET)

clean:
	@edk2.bat clean

usb:
	@run_as_admin codegen make_uefi_disk $(PROJECT).inf
	@echo Done!

qemu:
	@edk2.bat qemu -m $(PROJECT).inf
	@echo Done!
