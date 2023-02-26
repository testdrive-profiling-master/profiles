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
OBJS		:= $(SRCS:.c=.o)
OBJS		:= $(OBJS:.cc=.o)
OBJS		:= $(OBJS:.cpp=.o)
DEPS		:= $(OBJS:.o=.d)
INC			:= $(INC) -I$(TESTDRIVE_DIR)include -I$(TESTDRIVE_PROFILE)Common/include -I$(TESTDRIVE_DIR)bin/msys64/usr/include

ifndef CPPCHECK_SRCS
CPPCHECK_SRCS	:= $(SRCS)
endif

ifdef SRCS_LIB
OBJS_LIB	:= $(SRCS_LIB:.c=.o)
OBJS_LIB	:= $(OBJS_LIB:.cc=.o)
OBJS_LIB	:= $(OBJS_LIB:.cpp=.o)
else
OBJS_LIB	:= $(OBJS)
endif

ifndef TARGETPATH
	TARGETPATH	:= .
endif

ifndef LIBPATH
	LIBPATH		:= $(TARGETPATH)
endif

#-------------------------------------------------
# Build sequence
#-------------------------------------------------
TARGET_EXE		:= $(TARGETPATH)/$(TARGETNAME).exe
TARGET_A		:= $(LIBPATH)/lib$(TARGETNAME).a
TARGET_SO		:= $(TARGETPATH)/$(TARGETNAME).dll
TARGET_SO_A		:= $(TARGETPATH)/lib$(TARGETNAME).dll

COMPILE_TARGET	:= $(BUILD_TARGET)

ifeq ($(BUILD_TARGET), $(TARGET_SO_A))
	COMPILE_TARGET	:= $(TARGET_SO)
endif

#-------------------------------------------------
# 	Build flags.
#-------------------------------------------------
ifdef CROSS
	CDEFS			:= $(CDEFS) -DCROSS_COMPILE
endif

ifeq ($(EXTRA_CFLAGS),)
ifdef USE_DEBUG
	EXTRA_CFLAGS	:= -O0 -std=c++20 -DDEBUG -g
else
	EXTRA_CFLAGS	:= -O3 -std=c++20 -DNDEBUG
endif
endif

ifneq ($(BUILD_TARGET), $(TARGET_EXE))
	ifneq ($(MAIN_COMPILER), clang)
		CFLAGS			:= $(CFLAGS) -fPIC
	endif
endif

OPTFLAGS		:= -w -Wall -Wextra -m64 -mfpmath=sse -mieee-fp -mmmx -msse -msse2
CFLAGS			:= $(CFLAGS) $(OPTFLAGS) $(INCDIR) $(EXTRA_CFLAGS)
CPPFLAGS		:= $(CFLAGS)
CFLAGS			:= $(subst -std=c++11,,$(CFLAGS))
CFLAGS			:= $(subst -std=c++14,,$(CFLAGS))
CFLAGS			:= $(subst -std=c++17,,$(CFLAGS))
CFLAGS			:= $(subst -std=c++20,,$(CFLAGS))
ARFLAGS			:= crv
CDEFS			:= $(CDEFS) -D__int64="long long" -DWIN32 -D_WIN32 -DWIN64 -D_WIN64

#-------------------------------------------------
# Build commands
#-------------------------------------------------

all: $(COMPILE_TARGET)
	@echo Compilation is done!

encrypt: $(ENCRYPT_EXTRA:=.encrypted) $(SRCS:=.encrypted) $(SRCS_RES:=.encrypted)
	@echo Encryption is done!

decrypt: $(ENCRYPT_EXTRA:=.decrypted) $(SRCS_ENCRYPTED:.encrypted=.decrypted)
	@echo Decryption is done!

clean:
	@$(RM) -f $(OBJS) $(OBJS_RES) $(DEPS)
ifndef PRESERVE_RESULT
	@$(RM) -f $(BUILD_TARGET)

ifeq ($(TARGETPATH)$(BUILD_TARGET), $(LIBPATH)$(TARGET_SO_A))
		@$(RM) -f $(TARGET_A)
endif
endif
	@echo Cleanup is done.

distclean: clean
ifdef SRCS_ENCRYPTED
	@$(RM) -f $(SRCS_ENCRYPTED:.encrypted=) $(ENCRYPT_EXTRA)
endif
	@echo Dist-Cleanup is done.

static:
ifeq ($(strip $(CPPCHECK_SRCS)),)
	@echo *I: No sources to check static analysis.
else
	@cppcheck -j $(NUMBER_OF_PROCESSORS) --suppress=*:*/msys64/* --suppress=*:*/lib_src/* $(INC) $(CDEFS) $(CPPCHECK_ARG) -D__MINGW32__ --platform=win64 --inline-suppr --force $(CPPCHECK_SRCS)
endif

dep:
	@for def_file in $(DEPS); do \
		DependencyPrepare "$$def_file";  \
	done

#-------------------------------------------------
# dependencies
#-------------------------------------------------
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS) $(OBJS_RES:.o=.dep)
endif

#-------------------------------------------------
# encrpyt / decrypt
#-------------------------------------------------
%.encrypted: %
	@echo '- Encrypting... : $<'
	@TestDrive_LM encrypt $<

%.decrypted: %.encrypted
	@echo '- Decrypting... : $<'
	@TestDrive_LM decrypt $<

#-------------------------------------------------
# generic rules
#-------------------------------------------------
%.o: %.c
	@echo '- Compiling... : $<'
	@ccache $(CC) $(CDEFS) $(CFLAGS) $(INC) -MD -c $< -o $@

%.o: %.cpp
	@echo '- Compiling... : $<'
	@ccache $(CXX) $(CDEFS) $(CPPFLAGS) -Weffc++ $(INC) -MD -c $< -o $@
	
%.o: %.cc
	@echo '- Compiling... : $<'
	@ccache $(CXX) $(CDEFS) $(CPPFLAGS) -Weffc++ $(INC) -MD -c $< -o $@

%.o: %.rc
	@echo '- Compiling... : $<'
	@windres $(INC) $< -o $@

$(TARGET_EXE):$(OBJS) $(OBJS_RES) $(TARGET_DEP)
	@echo
	@echo '*** Build execution file ***'
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(OBJS_RES) $(LIBDIR)
ifdef INSTALL_PATH
ifneq ($(INSTALL_PATH), $(TARGETPATH))
	@echo Install to : $(INSTALL_PATH)
	@cp -f $(TARGET_EXE) $(INSTALL_PATH)/
endif
endif

$(TARGET_A):$(OBJS_LIB) $(OBJS_RES) $(TARGET_DEP)
	@echo
	@echo '*** Build Static Library ***'
	$(AR) $(ARFLAGS) $@ $(OBJS_LIB) $(OBJS_RES)
	$(RANLIB) $@

$(TARGET_SO):$(OBJS_LIB) $(OBJS_RES) $(TARGET_DEP)
	@echo
ifeq ($(BUILD_TARGET), $(TARGET_SO_A))
	@echo '*** Build Shared (+implib) Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJS_LIB) $(OBJS_RES) $(LIBDIR) -Wl,--out-implib,$(LIBPATH)/lib$(TARGETNAME).a
else
	@echo '*** Build Shared Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJS_LIB) $(OBJS_RES) $(LIBDIR)
endif
ifdef INSTALL_PATH
ifneq ($(INSTALL_PATH), $(TARGETPATH))
	@echo Install to : $(INSTALL_PATH)
	@cp -f $(TARGET_SO) $(INSTALL_PATH)/
endif
endif
