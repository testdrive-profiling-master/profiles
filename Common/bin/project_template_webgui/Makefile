#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= bin
INSTALL_PATH	:= bin
TARGETNAME		:= Test
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)

SRCS_RES		:= \
	$(SRC_PATH)/res/Resource.rc

INC				:= -I. -Ires
LIBDIR			:=
BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# WebGUI config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util_gui.mk
