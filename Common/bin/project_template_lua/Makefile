#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= .
INSTALL_PATH	:= .
TARGETNAME		:= Test
BUILD_VERSION	:= 1			# automate version counting to ".$(TARGETNAME)_version.inl"
#USE_DEBUG		:= 1			# comment out if you not need a debug mode
#CPP_STANDARD	:= c++20		# C++ standard
#MAIN_COMPILER	:= clang		# uncomment this to use clang compiler (default : gcc)

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:= -llua
CDEFS			:=

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# Util config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk
