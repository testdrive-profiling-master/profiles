#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../../Application/lib
TARGETPATH		:= ../../../../Program
TARGETNAME		:= SimSystem

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

LIBDIR			:= -L$(LIBPATH) -lSimHDL -lgomp -lngspice
INC				:= -I. -Isrc -I../../../../Application/include -I../../../HDL/dpi

CDEFS			:= -DSYSTEM_IMPLEMENTATION -DSIMLIB_EXPORTS -DUSE_TESTDRIVE
EXTRA_CFLAGS	:= -Os -std=c++11 -DNDEBUG
#LDFLAGS			:= --def wrapper.def

BUILD_TARGET	= $(TARGET_SO)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
