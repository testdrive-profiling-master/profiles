#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= bin
INSTALL_PATH	:= bin
TARGETNAME		:= Test
#USE_DEBUG		:= 1
#WEBUI_SECURED	:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)

SRCS_RES		:= \
	$(SRC_PATH)/res/Resource.rc

INC				:= -I. -Ires
LIBDIR			:= 
CDEFS			:= -D_WIN32_IE=0x0500 -DWINVER=0x500
ifndef USE_DEBUG
LDFLAGS			:= -mwindows
endif

ifdef WEBUI_SECURED
LIBDIR			:= $(LIBDIR) -lwebui-2-secure-static -lws2_32 -lwinmm -lole32 -ldwmapi -lcrypto -lssl
else
LIBDIR			:= $(LIBDIR) -lwebui-2-static -lws2_32 -lwinmm -lole32 -ldwmapi
endif

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# Util config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk
