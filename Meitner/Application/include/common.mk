#-------------------------------------------------
# Documents libs & defines config
#-------------------------------------------------
LIBPATH			:= ../../../lib

INC				:= $(INC) -I../../../include
LIBDIR			:= $(LIBDIR) -L$(LIBPATH) -lSystemDDK
CDEFS			:= $(CDEFS) -DUSE_TESTDRIVE

BUILD_TARGET	= $(TARGET_EXE)

include $(TESTDRIVE_PROFILE)/common/include/config.mk
