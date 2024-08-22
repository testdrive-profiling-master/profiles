#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= src
LIBPATH			:= ../../lib
TARGETPATH		:= ../../bin
TARGETNAME		:= SystemHAL

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp)

LIBDIR			:=
INC				:= -Isrc

CDEFS			:= -DSYSTEM_EXPORTS -DUSE_TESTDRIVE
CFLAGS			:=

BUILD_TARGET	= $(TARGET_SO_A)

#-------------------------------------------------
# common document config
#-------------------------------------------------
include $(TESTDRIVE_PROFILE)common/include/config.mk
