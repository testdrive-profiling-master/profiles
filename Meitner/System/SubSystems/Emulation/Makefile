#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../Application/lib
TARGETPATH		:= ../../../Program
TARGETNAME		:= EmulSystem

SRCS			:= \
	$(SRC_PATH)/src/InterruptService.cpp \
	$(SRC_PATH)/src/SystemMemory.cpp \
	$(SRC_PATH)/src/NativeMemory.cpp \
	$(SRC_PATH)/src/SystemEmul.cpp \
	$(SRC_PATH)/src/ThreadManager.cpp \
	$(SRC_PATH)/src/Util.cpp \
	$(SRC_PATH)/emulation/Emulation.cpp \
	$(SRC_PATH)/dllmain.cpp

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
