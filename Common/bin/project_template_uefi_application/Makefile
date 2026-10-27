#-------------------------------------------------
# __PROJECT__ application
#-------------------------------------------------
PROJECT			:= __PROJECT__
PROJECT_TYPE	:= APPLICATION
SRC_PATH		:= .
TARGET_ARCH		:= X64

SRCS			:= \
	$(PROJECT).dsc \
	$(PROJECT).inf \
	$(wildcard $(SRC_PATH)/*.c)

#-------------------------------------------------
# common UEFI config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/Common/include/config_uefi.mk
