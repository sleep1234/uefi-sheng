/** @file
  Shennong GOP log driver.

  Logs the active Graphics Output Protocol framebuffer geometry so the first
  real boot can confirm the ABL-supplied 1440x3200 panel framebuffer matches
  the PcdFrameBuffer* values in shennong.dsc. No hardware access.
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
ShennongGopLogDxeEntry (
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
  if (EFI_ERROR (Status) || (Gop == NULL) || (Gop->Mode == NULL) || (Gop->Mode->Info == NULL)) {
    DEBUG ((DEBUG_ERROR, "SHENNONGGOP: no GOP (%r)\n", Status));
    return EFI_SUCCESS;
  }

  DEBUG ((
    DEBUG_ERROR,
    "SHENNONGGOP: fb base 0x%lx size 0x%lx, %u x %u, fmt %u\n",
    Gop->Mode->FrameBufferBase,
    Gop->Mode->FrameBufferSize,
    (UINT32)Gop->Mode->Info->HorizontalResolution,
    (UINT32)Gop->Mode->Info->VerticalResolution,
    (UINT32)Gop->Mode->Info->PixelFormat
    ));
  return EFI_SUCCESS;
}
