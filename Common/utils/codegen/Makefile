#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETNAME		:= codegen

VERSION_UPDATE: all
	@codegen gen_version -p codegen .codegen_version.inl

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
