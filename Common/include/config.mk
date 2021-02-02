#-------------------------------------------------
#	Toolchain.
#-------------------------------------------------
ifndef CROSS
CROSS		:= x86_64-w64-mingw32-
endif

ifeq ($(MAIN_COMPILER), clang)
CC 			:= clang
CXX			:= clang++
else
# default compiler
CC 			:= $(CROSS)gcc
CXX			:= $(CROSS)g++
endif
AR 			:= ar
LD 			:= $(CROSS)ld
NM 			:= $(CROSS)nm
RANLIB 		:= ranlib
OBJCOPY		:= $(CROSS)objcopy
STRIP		:= $(CROSS)strip
PWD			:= $(shell pwd)
#-------------------------------------------------
#	Library & Include
#-------------------------------------------------
LIBDIR		:= $(LIBDIR) -L$(TESTDRIVE_PROFILE)Common/lib -L$(TESTDRIVE_DIR)bin/msys64/usr/lib -lstdc++ -lpthread -lm
OBJS_RES	:= $(SRCS_RES:.rc=.o)
OBJS_C		:= $(SRCS:.c=.o)
OBJS_CC		:= $(OBJS_C:.cc=.o)
OBJS		:= $(OBJS_CC:.cpp=.o)
DEPS_CCACHE	:= $(OBJS:.o=.d)
DEPS		:= $(OBJS:.o=.d.d)
INC			:= $(INC) -I$(TESTDRIVE_DIR)include -I$(TESTDRIVE_PROFILE)Common/include -I$(TESTDRIVE_DIR)bin/msys64/usr/include

#-------------------------------------------------
# 	Build flags.
#-------------------------------------------------
ifdef CROSS
	CDEFS		:= $(CDEFS) -DCROSS_COMPILE
endif

ifeq ($(EXTRA_CFLAGS),)
	EXTRA_CFLAGS	:= -O3 -std=c++11 -DNDEBUG
endif

OPTFLAGS	:= -w -Wall -Wextra -m64 -mfpmath=sse -mieee-fp -mmmx -msse -msse2
CFLAGS		:= $(CFLAGS) $(OPTFLAGS) $(INCDIR) $(EXTRA_CFLAGS)
ARFLAGS		:= crv
CDEFS		:= $(CDEFS) -D__int64="long long" -DWIN32 -D_WIN32 -DWIN64 -D_WIN64

#-------------------------------------------------
# Build sequence
#-------------------------------------------------
TARGET_EXE		:= $(TARGETPATH)/$(TARGETNAME).exe
TARGET_A		:= $(LIBPATH)/lib$(TARGETNAME).a
TARGET_SO		:= $(TARGETPATH)/$(TARGETNAME).dll
TARGET_SO_A		:= $(TARGETPATH)/lib$(TARGETNAME).dll

all: $(BUILD_TARGET)

$(TARGET_EXE): $(OBJS) $(OBJS_RES)
$(TARGET_SO): $(OBJS) $(OBJS_RES)
$(TARGET_A): $(OBJS) $(OBJS_RES)
$(TARGET_SO_A): $(OBJS) $(OBJS_RES)

clean:
	@$(RM) -f $(OBJS) $(OBJS_RES) $(BUILD_TARGET) $(TARGET_EXE).exe $(LIBPATH)/lib$(TARGETNAME).a $(DEPS_CCACHE) $(DEPS)

dust_clean:
	@$(RM) -f $(OBJS) $(OBJS_RES) $(TARGET_A) $(DEPS_CCACHE) $(DEPS)

static:
	@cppcheck -j $(NUMBER_OF_PROCESSORS) $(INC) $(CDEFS) $(CPPCHECK_ARG) -D__MINGW32__ --suppress=preprocessorErrorDirective --inline-suppr --force $(SRCS)

#########################################################################
# Dependency
%.d.d: %.c
	echo 
	@set -e; rm -f $@; \
	$(CC) -M $(CDEFS) $(CFLAGS) $(INC) $< > $@.tmp; \
	sed -e 's,[^.]*.o:,$*.o:,g' < $@.tmp > $@; \
	rm -f $@.tmp

%.d.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CDEFS) $(CFLAGS) -Weffc++ $(INC) $< > $@.tmp; \
	sed -e 's,[^.]*.o:,$*.o:,g' < $@.tmp > $@; \
	rm -f $@.tmp

%.d.d: %.cc
	@set -e; rm -f $@; \
	$(CXX) -M $(CDEFS) $(CFLAGS) -Weffc++ $(INC) $< > $@.tmp; \
	sed -e 's,[^.]*.o:,$*.o:,g' < $@.tmp > $@; \
	rm -f $@.tmp

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS) $(OBJS_RES:.o=.dep)
endif

#-------------------------------------------------
# generic rules
#-------------------------------------------------
%.o: %.c
	@echo '- Compiling... : $<'
	@ccache $(CC) $(CDEFS) $(CFLAGS) $(INC) -MD -c $< -o $@

%.o: %.cpp
	@echo '- Compiling... : $<'
	@ccache $(CXX) $(CDEFS) $(CFLAGS) -Weffc++ $(INC) -MD -c $< -o $@
	
%.o: %.cc
	@echo '- Compiling... : $<'
	@ccache $(CXX) $(CDEFS) $(CFLAGS) -Weffc++ $(INC) -MD -c $< -o $@

%.o: %.rc
	@echo '- Compiling... : $<'
	@windres $(INC) $< -o $@

$(TARGET_EXE):$(OBJS)
	@echo
	@echo '*** Build execution file ***'
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(OBJS_RES) $(LIBDIR)

$(TARGET_A):$(OBJS)
	@echo
	@echo '*** Build Static Library ***'
	$(AR) $(ARFLAGS) $@ $(OBJS) $(OBJS_RES)
	$(RANLIB) $@

$(TARGET_SO):$(OBJS)
	@echo
	@echo '*** Build Shared Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJS) $(OBJS_RES) $(LIBDIR)
	
$(TARGET_SO_A):$(OBJS)
	@echo
	@echo '*** Build Shared Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJS) $(OBJS_RES) $(LIBDIR) -Wl,--out-implib,$(LIBPATH)/lib$(TARGETNAME).a
