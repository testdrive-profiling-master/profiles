#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
INSTALL_PATH	:= .
TARGETNAME		:= ToolAutoDownload
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/../framework/argtable3/src/*.c) \
	$(wildcard $(SRC_PATH)/../framework/ArgTable.cpp) \
	$(wildcard $(SRC_PATH)/../framework/cstring.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I. -I../framework/
LIBDIR			:=
CDEFS			:= -DUNUSE_CSTRING_ICONV

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
POST_BUILD		= update_libs.bat

include $(TESTDRIVE_PROFILE)Common/include/config.mk
