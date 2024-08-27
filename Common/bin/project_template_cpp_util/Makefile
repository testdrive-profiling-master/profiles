#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
INSTALL_PATH	:= .
TARGETNAME		:= Test
BUILD_VERSION	:= 1
USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:=
CDEFS			:=

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# Util config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk
