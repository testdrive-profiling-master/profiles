#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= $(TESTDRIVE_DIR)/bin/
TARGETNAME		:= install_notepad_plugin
PRESERVE_RESULT	:= 1
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I../common -I.
LIBDIR			:=
CDEFS			:=

#-------------------------------------------------
# common util config
#-------------------------------------------------
include ../include/util.mk