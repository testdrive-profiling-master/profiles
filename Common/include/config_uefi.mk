#-------------------------------------------------
# Build version header
#-------------------------------------------------
ifndef BUILD_VERSION
	BUILD_VERSION	:= 0
endif

VERSION_OPTION		:=
ifneq ($(BUILD_VERSION), 0)
	ifeq ($(BUILD_VERSION), 2)
		VERSION_OPTION	:= -t
	endif
	VERSION_HEADER	:= .$(PROJECT)_version.inl
else
	VERSION_HEADER	:=
endif

all:debug

debug: $(SRCS) $(VERSION_HEADER)
	@edk2.bat build -a $(TARGET_ARCH) -b DEBUG

release: $(SRCS) $(VERSION_HEADER)
	@edk2.bat build -a $(TARGET_ARCH) -b RELEASE

clean:
	@edk2.bat clean

ifeq ($(PROJECT_TYPE), APPLICATION)
usb: $(SRCS) $(VERSION_HEADER)
	@edk2.bat build -a $(TARGET_ARCH) -b RELEASE
	@run_as_admin codegen make_uefi_disk $(PROJECT).inf
	@echo Done!

qemu:
	@edk2.bat qemu -m $(PROJECT).inf
	@echo Done!
endif

define MAKEFILE_USAGE
** UEFI Makefile command list **
    help          : Display this help and exit

  - Build
    [debug]       : Build project debug(default) mode
    release       : Build project release mode
    clean         : Clean up output and intermediate files.
endef

ifeq ($(PROJECT_TYPE), APPLICATION)
define MAKEFILE_USAGE_APPLICATION

    usb           : Make physical USB booting disk
    qemu          : Make QEMU application environment

endef
MAKEFILE_USAGE	+= $(MAKEFILE_USAGE_APPLICATION)
endif

define MAKEFILE_USAGE_VERSION

  - Version control (must set 'BUILD_VERSION' to 1/2)
    version_minor : Increase minor version number.
    version_major : Increase major version number.
endef
MAKEFILE_USAGE	+= $(MAKEFILE_USAGE_VERSION)

define MAKEFILE_USAGE_newline


endef

help:
	@printf "$(subst $(MAKEFILE_USAGE_newline),\n,$(MAKEFILE_USAGE))\n"

ifneq ($(BUILD_VERSION), 0)
$(VERSION_HEADER): $(SRCS)
	@codegen.exe gen_version -p $(PROJECT) $(VERSION_OPTION) $(VERSION_HEADER)

version_minor:
	@codegen.exe gen_version -n -p $(PROJECT) $(VERSION_OPTION) $(VERSION_HEADER)

version_major:
	@codegen.exe gen_version -m -p $(PROJECT) $(VERSION_OPTION) $(VERSION_HEADER)
else
version_minor:
	@echo *E: Version control is not enabled for this project.
	
version_major:
	@echo *E: Version control is not enabled for this project.
endif
