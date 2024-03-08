#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETNAME		:= codegen
BUILD_VERSION	:= 1

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

INC				:= -I../common -I./src -I../lib_src
LIBDIR			:= -llua -lTestDrive_LM -lz -lminizip -lssl -llibgit2 -lncurses
CDEFS			:= -DLUABRIDGE_CXX17=1

#-------------------------------------------------
# common util config
#-------------------------------------------------
include ../include/util.mk
