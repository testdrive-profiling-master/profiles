#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
TARGETNAME		:= noconsole
#BUILD_VERSION	:= 1
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp)
	
INC				:= -I.
LIBDIR			:=
CDEFS			:= -D_WIN32_IE=0x605 -DWINVER=0x605
LDFLAGS			:= -mwindows

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# Util config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk
