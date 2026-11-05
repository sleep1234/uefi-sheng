/** @file
  Shennong display depex gate.

  The Mu-Silicium SimpleFbDxe keeps the previous stage's (ABL/stock UEFI) GOP
  framebuffer scanning out, so the UniXNU boot manager is visible without a
  vendor panel driver. This driver exists only to express a DEPEX dependency on
  the Graphics Output Protocol, guaranteeing the framebuffer is published before
  UniXNUDxe draws the menu. It performs no hardware access.

  TODO (hardware): when SHENNONG_VENDOR_HW=1, replace the no-op body with the
  shennong panel/backlight enable sequence (qcom,mdss_dsi_n2_42_02_0a_p2_dsc_cmd
  cmd-mode panel, 1440x3200 LTPO AMOLED).
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
ShennongDisplayDepexDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop;
  EFI_STATUS                    Status;

  Status = gBS->LocateProtocol (
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  (VOID **)&Gop
                  );
  if (!EFI_ERROR (Status) && (Gop != NULL) && (Gop->Mode != NULL)) {
    DEBUG ((
      DEBUG_ERROR,
      "SHENNONGDISP: GOP mode %u, fb 0x%lx, %u x %u\n",
      (UINT32)Gop->Mode->Mode,
      Gop->Mode->FrameBufferBase,
      (UINT32)Gop->Mode->Info->HorizontalResolution,
      (UINT32)Gop->Mode->Info->VerticalResolution
      ));
  } else {
    DEBUG ((DEBUG_ERROR, "SHENNONGDISP: GOP not ready (%r)\n", Status));
  }

  return EFI_SUCCESS;
}
