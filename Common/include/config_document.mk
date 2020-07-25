#-------------------------------------------------
# Documents libs & defines config
#-------------------------------------------------
INC				:= $(INC) -I../../../Application/include
LIBDIR			:= $(LIBDIR) -lole32 -loleaut32 -lshell32
CDEFS			:= $(CDEFS) -DTESTDRIVE_DOCUMENT -DDOCUMENT_EXPORTS -DUSE_TESTDRIVE -DUNICODE -D_UNICODE
EXTRA_CFLAGS	:= -O3 -DNDEBUG

BUILD_TARGET	= $(TARGET_SO)

include $(TESTDRIVE_PROFILE)/common/include/config.mk
