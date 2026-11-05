/** @file
  Shennong display depex gate (placeholder).

  With SHENNONG_VENDOR_HW = 0 the SimpleFbDxe keeps the ABL GOP framebuffer
  scanning out, so the UniXNU menu is visible without a vendor panel driver.
  This driver exists as a placeholder that can later host a vendor DisplayDxe
  DEPEX once SHENNONG_VENDOR_HW = 1 bring-up begins. Currently a no-op.
**/
#include <Uefi.h>

EFI_STATUS
EFIAPI
ShennongDisplayDepexDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}