#------------------------------------
# makefile for util
#------------------------------------
ifndef TARGETPATH
TARGETPATH		:= .
endif

ifndef INSTALL_PATH
INSTALL_PATH	:= $(TESTDRIVE_PROFILE)Common/bin
endif

# select build type (TARGET_EXE, TARGET_SO, TARGET_A, TARGET_SO_A)
ifndef BUILD_TARGET
BUILD_TARGET	= $(TARGET_EXE)
endif

TARGET_DEP		:= $(TARGET_DEP) $(TESTDRIVE_PROFILE)Common/utils/lib/libUtilFramework.a
INC				:= $(INC) -I$(TESTDRIVE_PROFILE)Common/utils/framework -I$(TESTDRIVE_PROFILE)Common/utils/include -I$(TESTDRIVE_PROFILE)Common/utils/lib_src
LIBDIR			:= -L$(TESTDRIVE_PROFILE)Common/utils/lib -lUtilFramework -liconv $(LIBDIR)

#-------------------------------------------------
# common build config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)Common/include/config.mk