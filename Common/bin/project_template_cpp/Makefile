#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
INSTALL_PATH	:= .
TARGETNAME		:= Test

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:=
CDEFS			:=
EXTRA_CFLAGS	:= -O0 -std=c++20 -DDEBUG -g
#MAIN_COMPILER	:= clang		# uncomment this code to use clang compiler

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
