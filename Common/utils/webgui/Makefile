#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
TARGETNAME		:= webgui
INSTALL_PATH	:= $(TESTDRIVE_PROFILE)Common/bin
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp)

INC				:= -I.
LIBDIR			:= -lshlwapi -lole32 -lversion -lmicrohttpd -lWs2_32 -lgdi32 `pkg-config --libs-only-l jsoncpp`
CDEFS			:= -D_WIN32_IE=0x605 -DWINVER=0x605

BUILD_TARGET	= $(TARGET_A)

#-------------------------------------------------
# Util config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk
