#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= $(TESTDRIVE_PROFILE)Common/System/SystemSim/SimSystem
LIBPATH			:= ../../../../Application/lib
TARGETPATH		:= ../../../../Program
TARGETNAME		:= SimSystem

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(SRC_PATH)/*.cpp)

LIBDIR			:= -L$(LIBPATH) -lSimHDL -lgomp -lngspice
INC				:= -I$(SRC_PATH)/src -I$(TESTDRIVE_PROFILE)common/System/SystemSim/SimHDL -I$(TESTDRIVE_PROFILE)common/System/SystemSim/HDL/dpi -Isrc -I../../../../Application/include -I../../../HDL/dpi

CDEFS			:= -DSYSTEM_IMPLEMENTATION -DSIMLIB_EXPORTS -DUSE_TESTDRIVE
#LDFLAGS			:= --def wrapper.def

BUILD_TARGET	= $(TARGET_SO)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
