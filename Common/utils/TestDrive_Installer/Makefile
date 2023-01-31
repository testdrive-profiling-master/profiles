#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ./release/
TARGETNAME		:= TestDrive_Installer

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:= -liconv -lssl -llibgit2
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
BUILD_TARGET	= $(TARGET_EXE)
include $(TESTDRIVE_PROFILE)Common/include/config.mk
