#-------------------------------------------------
# testbench
#-------------------------------------------------
SRCS			:= $(SRCS) \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/Testbench.cpp

LIBPATH			:= ../../../lib

INC				:= $(INC) -I. -I../framework -I../../../include
LIBDIR			:= $(LIBDIR) -L$(LIBPATH) -lSystemDDK
CDEFS			:= $(CDEFS) -DUSE_TESTDRIVE

BUILD_TARGET	= $(TARGET_EXE)

#-------------------------------------------------
# advanced util build
#-------------------------------------------------
INSTALL_PATH	:= $(TARGETPATH)
include $(TESTDRIVE_PROFILE)/common/utils/include/util.mk