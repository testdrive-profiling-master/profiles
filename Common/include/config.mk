#-------------------------------------------------
#	Toolchain.
#-------------------------------------------------
ifeq ($(OS),Windows_NT)
	ifndef CROSS
	CROSS		:= x86_64-w64-mingw32-
	endif
	CDEFS		:= $(CDEFS) -DWIN32 -D_WIN32 -DWIN64 -D_WIN64
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

PLATFORM	:= $(shell uname -m)
#-------------------------------------------------
#	Library & Include
#-------------------------------------------------
LIBDIR		:= $(LIBDIR) -lpthread -lm
OBJS_RES	:= $(SRCS_RES:.rc=.o)
OBJS		:= $(SRCS:.c=.o)
OBJS		:= $(OBJS:.cc=.o)
OBJS		:= $(OBJS:.cpp=.o)
DEPS		:= $(OBJS:.o=.d)
ifeq ($(OS),Windows_NT)
	LIBDIR		:= -L$(TESTDRIVE_PROFILE)Common/lib -L$(TESTDRIVE_DIR)bin/msys64/usr/lib $(LIBDIR)
	INC			:= $(INC) -I$(TESTDRIVE_DIR)include -I$(TESTDRIVE_PROFILE)Common/include -I$(TESTDRIVE_DIR)bin/msys64/usr/include
endif

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
TARGET_A		:= $(LIBPATH)/lib$(TARGETNAME).a
ifeq ($(OS),Windows_NT)
	TARGET_EXE		:= $(TARGETPATH)/$(TARGETNAME).exe
	TARGET_SO		:= $(TARGETPATH)/$(TARGETNAME).dll
	TARGET_SO_A		:= $(TARGETPATH)/lib$(TARGETNAME).dll
else
	TARGET_EXE		:= $(TARGETPATH)/$(TARGETNAME)
	TARGET_SO		:= $(TARGETPATH)/$(TARGETNAME).so
	TARGET_SO_A		:= $(TARGETPATH)/lib$(TARGETNAME).so
endif

COMPILE_TARGET	:= $(BUILD_TARGET)

ifeq ($(BUILD_TARGET), $(TARGET_SO_A))
	COMPILE_TARGET	:= $(TARGET_SO)
endif

#-------------------------------------------------
# Build version header
#-------------------------------------------------
ifndef BUILD_VERSION
	BUILD_VERSION	:= 0
endif

ifeq ($(BUILD_VERSION), 1)
	VERSION_HEADER	:= .$(TARGETNAME)_version.inl
else
	VERSION_HEADER	:=
endif

#-------------------------------------------------
# 	Build flags.
#-------------------------------------------------
ifdef CROSS
	CDEFS			:= $(CDEFS) -DCROSS_COMPILE
endif

# C++ standard
ifeq ($(CPP_STANDARD),)
	CPP_STANDARD			:= c++20
endif

# enable debugging
ifdef USE_DEBUG
	CFLAGS		:= $(CFLAGS) -O0 -DDEBUG -g
	CDEFS		:= $(CDEFS) -DUSE_DEBUG
else
	CFLAGS		:= $(CFLAGS) -DNDEBUG
ifdef SIMPATH
	CFLAGS		:= $(CFLAGS) -Os -fno-stack-protector
else
	CFLAGS		:= $(CFLAGS) -O3
endif
endif

ifneq ($(BUILD_TARGET), $(TARGET_EXE))
	ifneq ($(MAIN_COMPILER), clang)
		CFLAGS			:= $(CFLAGS) -fPIC
	endif
endif

OPTFLAGS		:= -w -Wall -Wextra -march=native
ifeq ($(PLATFORM), x86_64)
	OPTFLAGS		:= $(OPTFLAGS) -m64 -mfpmath=sse -mieee-fp -mmmx -msse -msse2 -mavx -mavx2
endif
CFLAGS			:= $(CFLAGS) $(OPTFLAGS) $(INCDIR) $(EXTRA_CFLAGS)
CPPFLAGS		:= $(CFLAGS) -std=$(CPP_STANDARD)
ARFLAGS			:= crv
CDEFS			:= $(CDEFS) -D__int64="long long" -D_FILE_OFFSET_BITS=64

#-------------------------------------------------
# Build commands
#-------------------------------------------------

all: dep $(COMPILE_TARGET)
	@echo Compilation is done!

encrypt: $(ENCRYPT_EXTRA:=.encrypted) $(SRCS:=.encrypted) $(SRCS_RES:=.encrypted)
	@echo Encryption is done!

decrypt: $(ENCRYPT_EXTRA:=.decrypted) $(SRCS_ENCRYPTED:.encrypted=.decrypted)
	@echo Decryption is done!

ifeq ($(OS),Windows_NT)
deploy: all
	@echo $(BUILD_TARGET)
	@codegen collect_lib $(BUILD_TARGET)
endif

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
ifeq ($(OS),Windows_NT)
	@cppcheck -j $(NUMBER_OF_PROCESSORS) --suppress=*:*/msys64/* --suppress=*:*/lib_src/* $(INC) $(CDEFS) $(CPPCHECK_ARG) -D__MINGW32__ -D__MINGW64__ -D__GNUC__ --platform=win64 --inline-suppr $(CPPCHECK_SRCS)
else
	@cppcheck -j $(NUMBER_OF_PROCESSORS) --suppress=*:*/lib_src/* $(INC) $(CDEFS) $(CPPCHECK_ARG) -D__GNUC__ --inline-suppr $(CPPCHECK_SRCS)
endif
endif

# version management
ifeq ($(OS),Windows_NT)
ifeq ($(BUILD_VERSION), 1)
$(VERSION_HEADER): $(SRCS)
	@codegen gen_version -p $(TARGETNAME) .$(TARGETNAME)_version.inl

version_minor:
	@codegen gen_version -n -p $(TARGETNAME) .$(TARGETNAME)_version.inl

version_major:
	@codegen gen_version -m -p $(TARGETNAME) .$(TARGETNAME)_version.inl
else
version_minor:
	@echo *E: Version control is not enabled for this project.
	
version_major:
	@echo *E: Version control is not enabled for this project.
endif
endif

# for simulation build sequence
ifdef SIMPATH
SIM_DEPS	:= $(filter-out ../verilator/%,$(DEPS))
SIM_OBJS	:= $(filter-out ../verilator/%,$(OBJS_LIB))
endif

dep:
ifeq ($(OS),Windows_NT)
ifdef SIMPATH
	@DependencyPrepare -s $(SIM_DEPS)
else
	@DependencyPrepare -s $(DEPS)
endif
endif


ifneq ($(OS),Windows_NT)
PACKAGES	:= $(PACKAGES) ccache cppcheck
prepare: $(PACKAGES)
TARGET_DEP	:= $(TARGET_DEP) prepare

$(PACKAGES):
	@if ! dpkg -l | grep $@ -c >>/dev/null; then sudo apt-get --yes install $@; fi
endif

define MAKEFILE_USAGE
** TestDrive Profiling Master's Makefile command list **

  - Build
    [all]         : Build output binaries.
    deploy        : Gethering required MinGW libraries in same output folder
    clean         : Clean up output and intermediate files.
    distclean     : Clean up output, intermediate files and unecryped source files.
    dep           : Build all dependencies.
    static        : Do static code analysis(cppcheck).

  - Version control (must set 'BUILD_VERSION' to 1)
    version_minor : Increase minor version number.
    version_major : Increase major version number.
 
  - Encryption & Decryption (must set 'SRCS_ENCRYPTED')
    encrypt       : Encrypt sources.
    decrypt       : Decrypt sources.
endef

define MAKEFILE_USAGE_newline


endef

help:
	@printf "$(subst $(MAKEFILE_USAGE_newline),\n,$(MAKEFILE_USAGE))\n"

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
%.o: %.c $(TARGET_DEP)
	@echo '- Compiling... : $<'
	@ccache $(CC) $(CDEFS) $(CFLAGS) $(INC) -MD -c $< -o $@
ifeq ($(OS),Windows_NT)
#	ccache bug fix "D\:/" -> "D:/"
	@sed -i 's/\\\:/\:/g' $*.d
endif

%.o: %.cpp $(TARGET_DEP)
	@echo '- Compiling... : $<'
	@ccache $(CXX) $(CDEFS) $(CPPFLAGS) -Weffc++ $(INC) -MD -c $< -o $@
ifeq ($(OS),Windows_NT)
	@sed -i 's/\\\:/\:/g' $*.d
endif
	
%.o: %.cc $(TARGET_DEP)
	@echo '- Compiling... : $<'
	@ccache $(CXX) $(CDEFS) $(CPPFLAGS) -Weffc++ $(INC) -MD -c $< -o $@
ifeq ($(OS),Windows_NT)
	@sed -i 's/\\\:/\:/g' $*.d
endif

ifeq ($(OS),Windows_NT)
%.o: %.rc $(TARGET_DEP)
	@echo '- Compiling... : $<'
	@windres $(INC) $< -o $@
endif

$(TARGET_EXE):$(VERSION_HEADER) $(OBJS) $(OBJS_RES) $(TARGET_DEP)
	@echo
	@echo '*** Build execution file ***'
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(OBJS_RES) $(LIBDIR)
ifndef USE_DEBUG
	@strip --strip-all $(TARGET_EXE)
endif
ifdef INSTALL_PATH
ifneq ($(INSTALL_PATH), $(TARGETPATH))
	@echo Install to : $(INSTALL_PATH)
	@cp -f $(TARGET_EXE) $(INSTALL_PATH)/
endif
endif
ifdef POST_BUILD
	@$(POST_BUILD)
endif

$(TARGET_A):$(VERSION_HEADER) $(OBJS_LIB) $(OBJS_RES) $(TARGET_DEP)
	@echo
	@echo '*** Build Static Library ***'
	$(AR) $(ARFLAGS) $@ $(OBJS_LIB) $(OBJS_RES)
	$(RANLIB) $@

$(TARGET_SO):$(VERSION_HEADER) $(OBJS_LIB) $(OBJS_RES) $(TARGET_DEP)
	@echo
ifeq ($(BUILD_TARGET), $(TARGET_SO_A))
ifdef SIMPATH
	@echo '*** Build HDL Simulation Shared (+implib) Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(SIM_OBJS) ../verilator/\*.o $(OBJS_RES) $(LIBDIR) -Wl,--out-implib,$(LIBPATH)/lib$(TARGETNAME).a
else
	@echo '*** Build Shared (+implib) Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJS_LIB) $(OBJS_RES) $(LIBDIR) -Wl,--out-implib,$(LIBPATH)/lib$(TARGETNAME).a
endif
else
	@echo '*** Build Shared Library ***'
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJS_LIB) $(OBJS_RES) $(LIBDIR)
endif
ifndef USE_DEBUG
	@strip --strip-all $(TARGET_SO)
endif
ifdef INSTALL_PATH
ifneq ($(INSTALL_PATH), $(TARGETPATH))
	@echo Install to : $(INSTALL_PATH)
	@cp -f $(TARGET_SO) $(INSTALL_PATH)/
endif
endif
ifdef POST_BUILD
	@$(POST_BUILD)
endif
