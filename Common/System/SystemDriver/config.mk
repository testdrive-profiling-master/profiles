SRC_PATH		:= .
TARGETPATH		:= ../../../Program

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*.cpp) \
	$(wildcard $(TESTDRIVE_PROFILE)common/System/SystemDriver/src/*.cpp) \
	$(wildcard $(TESTDRIVE_PROFILE)common/System/SystemDriver/*.cpp)

INC				:= -Isrc -I$(TESTDRIVE_PROFILE)common/System/SystemDriver -I$(TESTDRIVE_PROFILE)common/System/SystemDriver/src $(INC)
CDEFS			:= -DSYSTEM_IMPLEMENTATION -DUSE_TESTDRIVE
CFLAGS			:= -O3
#LDFLAGS			:= --def wrapper.def

BUILD_TARGET	= $(TARGET_SO)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk