#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
TARGETNAME		:= Test

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:=
CDEFS			:=
EXTRA_CFLAGS	:= -O0 -std=c++17 -DDEBUG -g
#MAIN_COMPILER	:= clang		# uncomment this code to using clang compiler

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
