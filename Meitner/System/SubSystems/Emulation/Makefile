#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../Application/lib
TARGETPATH		:= ../../../Program
TARGETNAME		:= EmulSystem

SRCS			:= \
	$(wildcard $(SRC_PATH)/emulation/*.cpp) \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

LIBDIR			:= -L$(LIBPATH)
INC				:= -I. -Isrc -Iemulation -I../../../Application/include

CDEFS			:= -DSYSTEM_IMPLEMENTATION -DUSE_TESTDRIVE
CFLAGS			:= -O3
#LDFLAGS			:= --def wrapper.def

BUILD_TARGET	= $(TARGET_SO)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
