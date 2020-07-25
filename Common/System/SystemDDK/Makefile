#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= src
LIBPATH			:= ../../lib
TARGETPATH		:= ../../bin
TARGETNAME		:= SystemDDK

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	dllmain.cpp

LIBDIR			:= -L$(LIBPATH) -lSystemHAL -lz
INC				:= -I$(SRC_PATH)

CDEFS			:= -DDDK_EXPORTS -DUSE_TESTDRIVE
CFLAGS			:= -O3

ifdef DEBUG
CFLAGS			:= $(CFLAGS) -g
endif

BUILD_TARGET	= $(TARGET_SO_A)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
