#------------------------------------
# makefile for util
#------------------------------------
ifndef TARGETPATH
TARGETPATH		:= .
endif
INSTALL_PATH	:= $(TESTDRIVE_PROFILE)Common/bin

# select build type (TARGET_EXE, TARGET_SO, TARGET_A, TARGET_SO_A)
ifndef BUILD_TARGET
BUILD_TARGET	= $(TARGET_EXE)
endif

INC				:= $(INC) -I../framework -I../include -I../lib_src
LIBDIR			:= -L../lib -lframework -liconv $(LIBDIR)

include $(TESTDRIVE_PROFILE)Common/include/config.mk