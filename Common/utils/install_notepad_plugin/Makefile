#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= $(TESTDRIVE_DIR)/bin/
TARGETNAME		:= install_notepad_plugin
PRESERVE_RESULT	:= 1
#USE_DEBUG		:= 1			# comment out if you not need a debug mode
#CPP_STANDARD	:= c++20		# C++ standard
#MAIN_COMPILER	:= clang		# uncomment this to use clang compiler (default : gcc)

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