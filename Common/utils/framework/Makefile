#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ../lib
TARGETNAME		:= UtilFramework
PRESERVE_RESULT	:= 1
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/argtable3/src/*.c) \
	$(wildcard $(SRC_PATH)/*.cpp)


INC				:= -I.
LIBDIR			:= -liconv
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_A)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
