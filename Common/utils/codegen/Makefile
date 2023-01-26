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

INC				:= -I../common -I./src
LIBDIR			:= -llua -lTestDrive_LM -lz -lminizip
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../include/util.mk
