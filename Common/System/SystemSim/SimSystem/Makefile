#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../lib
TARGETPATH		:= ../../../bin
TARGETNAME		:= SimSystem

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

LIBDIR			:= -L$(LIBPATH) -lSimHDL -lgomp -lngspice
INC				:= -I. -Isrc -I../HDL/dpi

CDEFS			:= -DSYSTEM_IMPLEMENTATION -DSIMLIB_EXPORTS -DUSE_TESTDRIVE
#LDFLAGS			:= --def wrapper.def

BUILD_TARGET	= $(TARGET_SO)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
