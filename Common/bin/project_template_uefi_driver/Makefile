#-------------------------------------------------
# __PROJECT__ driver
#-------------------------------------------------
PROJECT			:= __PROJECT__
PROJECT_TYPE	:= DRIVER
SRC_PATH		:= .
TARGET_ARCH		:= X64
BUILD_VERSION	:= 1

SRCS			:= \
	$(PROJECT).dsc \
	$(PROJECT).inf \
	$(wildcard $(SRC_PATH)/*.c)

#-------------------------------------------------
# common UEFI config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/Common/include/config_uefi.mk
