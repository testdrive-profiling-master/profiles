#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETNAME		:= codegen

VERSION_UPDATE: all
	@codegen update_version.lua

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

INC				:= -I../common -I./src -I../lib_src
LIBDIR			:= -llua -lTestDrive_LM -lz -lminizip
CDEFS			:= -DLUABRIDGE_CXX17=1

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../include/util.mk
