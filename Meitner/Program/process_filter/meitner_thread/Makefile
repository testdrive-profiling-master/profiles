#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
LIBPATH			:= ../../../Application/lib
TARGETPATH		:= ..
TARGETNAME		:= meitner_thread.exe

SRCS			:= \
	$(SRC_PATH)/main.cpp

LIBDIR			:=
INC				:= -I../../../Application/include

CDEFS			:= 
CFLAGS			:= -O3
LDFLAGS			:= -mwindows

ifdef DEBUG
CFLAGS			:= $(CFLAGS) -g
endif

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
