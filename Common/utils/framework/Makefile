#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ../lib
TARGETNAME		:= UtilFramework
PRESERVE_RESULT	:= 1
#USE_DEBUG		:= 1			# comment out if you not need a debug mode
#CPP_STANDARD	:= c++20		# C++ standard
#MAIN_COMPILER	:= clang		# uncomment this to use clang compiler (default : gcc)

SRCS			:= \
	$(wildcard $(SRC_PATH)/argtable3/src/*.c) \
	$(wildcard $(SRC_PATH)/minizip/*.c) \
	$(wildcard $(SRC_PATH)/pugixml/*.cpp) \
	$(wildcard $(SRC_PATH)/ExcelNumFormat/ExcelNumFormat*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)


INC				:= -I.
LIBDIR			:= -liconv -lz -lminizip
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_A)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
