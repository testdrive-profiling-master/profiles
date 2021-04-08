#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../lib
TARGETPATH		:= .
INSTALL_PATH	:= ../../bin
TARGETNAME		:= TestDrive_LM

SRCS			:= \
	$(wildcard $(SRC_PATH)/base/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

SRCS_ENCRYPTED	:= \
	$(wildcard $(SRC_PATH)/base/*.encrypted) \
	$(wildcard $(SRC_PATH)/*.encrypted)
	
INC				:= -I. -I./base
LIBDIR			:= -lz
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
