#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../Application/lib
TARGETPATH		:= ../../../Program
TARGETNAME		:= PCIeDriverSystem

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

LIBDIR			:= -L$(LIBPATH)
INC				:= -I. -Isrc -I../../../Application/include -I../../../Driver/windows/src

CDEFS			:= -DSYSTEM_IMPLEMENTATION -DUSE_TESTDRIVE
CFLAGS			:= -O3
#LDFLAGS			:= --def wrapper.def

BUILD_TARGET	= $(TARGET_SO)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
