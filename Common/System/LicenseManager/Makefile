#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../lib
TARGETPATH		:= ../../bin
TARGETNAME		:= TestDrive_LM
PRESERVE_RESULT	:= 1

SRCS			:= \
	$(wildcard $(SRC_PATH)/base/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

SRCS_LIB		:= \
	$(wildcard $(SRC_PATH)/base/*.cpp)

SRCS_ENCRYPTED	:= \
	$(wildcard $(SRC_PATH)/base/*.encrypted) \
	$(wildcard $(SRC_PATH)/*.encrypted)
	
INC				:= -I. -I./base
LIBDIR			:= -lz
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE) $(TARGET_A)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
