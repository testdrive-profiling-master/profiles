all: $(SRCS)
	@edk2.bat build -a $(TARGET_ARCH) -b $(BUILD_TARGET)

clean:
	@edk2.bat clean
