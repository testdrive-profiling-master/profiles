#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ../../bin
TARGETNAME		:= TestDrive_LM

SRCS			:= \
	$(wildcard $(SRC_PATH)/base/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)
	
	
INC				:= -I. -I./base
LIBDIR			:= -lz
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
