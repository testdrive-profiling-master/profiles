#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE  *SystemTable)
{
	DEBUG ((DEBUG_INFO, "[UEFI-__PROJECT__] UefiMain()\n"));
	return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UefiUnload(EFI_HANDLE ImageHandle)
{
  DEBUG ((DEBUG_INFO, "[UEFI-__PROJECT__] UefiUnload()\n"));
  return EFI_SUCCESS;
}
