#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
INSTALL_PATH	:= .
TARGETNAME		:= Test
BUILD_VERSION	:= 1
#USE_DEBUG		:= 1			# comment out if you not need a debug mode

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:= -llua
CDEFS			:= -std=c++20
#MAIN_COMPILER	:= clang		# uncomment this code to use clang compiler

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# Util config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk
