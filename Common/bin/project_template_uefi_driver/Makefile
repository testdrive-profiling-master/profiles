#-------------------------------------------------
# __PROJECT__ driver
#-------------------------------------------------
PROJECT			:= __PROJECT__
SRC_PATH		:= .
TARGET_ARCH		:= X64
BUILD_TARGET	?= DEBUG

SRCS			:= \
	__PROJECT__.dsc \
	__PROJECT__.inf \
	$(wildcard $(SRC_PATH)/*.c)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/Common/include/config_uefi_driver.mk
