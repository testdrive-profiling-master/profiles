#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../Application/lib
TARGETPATH		:= ../../../Program
TARGETNAME		:= PCIeDriverSystem

SRCS			:= \
	$(SRC_PATH)/src/InterruptService.cpp \
	$(SRC_PATH)/src/SystemMemory.cpp \
	$(SRC_PATH)/src/NativeMemory.cpp \
	$(SRC_PATH)/src/PCIeDriver.cpp \
	$(SRC_PATH)/src/SystemDriver.cpp \
	$(SRC_PATH)/src/ThreadManager.cpp \
	$(SRC_PATH)/src/Util.cpp \
	$(SRC_PATH)/dllmain.cpp

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
