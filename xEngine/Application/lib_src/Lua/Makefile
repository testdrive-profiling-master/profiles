#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= ./Lua.5.2.0/
LIBPATH			:= ../../lib
TARGETPATH		:= ../../../Program
TARGETNAME		:= lua

SRCS			:= \
	$(wildcard $(SRC_PATH)/LuaLibrary5.2.0.cpp)

LIBDIR			:= 
INC				:= -I$(SRC_PATH) -I.

CDEFS			:= 
CFLAGS			:= -O3

BUILD_TARGET	= $(TARGET_A)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
