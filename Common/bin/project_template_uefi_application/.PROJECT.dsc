[Defines]
	PLATFORM_NAME				= __PROJECT___Platform
	PLATFORM_GUID				= __GUID__
	PLATFORM_VERSION			= 0.1
	DSC_SPECIFICATION			= 0x00010005
	OUTPUT_DIRECTORY			= Build
	SUPPORTED_ARCHITECTURES		= X64
	BUILD_TARGETS				= DEBUG|RELEASE
	SKUID_IDENTIFIER			= DEFAULT

[LibraryClasses]
	UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf

	UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
	UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

	UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
	BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
	BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
	MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

	PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
	DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
	IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf

	PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
	DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
	RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
	StackCheckLib|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf

[Components]
	./__PROJECT__.inf
