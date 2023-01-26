#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ../lib
TARGETNAME		:= framework
PRESERVE_RESULT	:= 1

SRCS			:= \
	$(wildcard $(SRC_PATH)/argtable3/src/*.c) \
	$(wildcard $(SRC_PATH)/minizip/*.c) \
	$(wildcard $(SRC_PATH)/pugixml/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)


INC				:= -I.
LIBDIR			:= -liconv -lz -lminizip
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_A)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
