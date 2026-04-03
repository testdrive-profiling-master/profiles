#-------------------------------------------------
# Test Project
#-------------------------------------------------
SRC_PATH		:= .
TARGET_ARCH		:= X64
BUILD_TARGET	:= DEBUG

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.dsc) \
	$(wildcard $(SRC_PATH)/*.inf) \
	$(wildcard $(SRC_PATH)/*.c)

all: $(SRCS)
	@edk2.bat build -a $(TARGET_ARCH) -b $(BUILD_TARGET)

clean:
	@edk2.bat clean
