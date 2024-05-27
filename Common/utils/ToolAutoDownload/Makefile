#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
INSTALL_PATH	:= .
TARGETNAME		:= ToolAutoDownload

SRCS			:= \
	$(wildcard $(SRC_PATH)/../framework/argtable3/src/*.c) \
	$(wildcard $(SRC_PATH)/../framework/ArgTable.cpp) \
	$(wildcard $(SRC_PATH)/../framework/cstring.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I. -I../framework/
LIBDIR			:=
CDEFS			:= -DUNUSE_CSTRING_ICONV
EXTRA_CFLAGS	:= -O3 -std=c++20 -DNDEBUG
#MAIN_COMPILER	:= clang		# uncomment this code to using clang compiler

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
POST_BUILD		= update_libs.bat

include $(TESTDRIVE_PROFILE)Common/include/config.mk
