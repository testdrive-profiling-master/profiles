#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETNAME		:= codegen
BUILD_VERSION	:= 1
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/clipper/*.cpp) \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

INC				:= -I../common -I./src -I../lib_src
LIBDIR			:= -llua -lTestDrive_LM -lz -lminizip -lssl -llibgit2 -lncurses -lqrencode
CDEFS			:= -DLUABRIDGE_CXX17=1

#-------------------------------------------------
# common util config
#-------------------------------------------------
include ../include/util.mk
