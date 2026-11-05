/** @file
  Shennong key input driver (skeleton).

  On shennong the volume/power keys are exposed through the PMIC SPMI keypad
  (same PM8550/PMK8550 family as other SM8650 boards). This skeleton registers
  a no-op entry point so the platform builds and boots hands-free; the real
  implementation must port the SPMI key matrix from Meizu21KeyInputDxe
  (loolooc/uefi-sheng) and adapt the shennong PMIC slave/peripheral addresses.

  TODO (hardware):
    - Map shennong PMIC SPMI (PMK8550 / PM8550VS) keypad peripheral addresses.
    - Translate volume-up / volume-down / power to UniXNU key events.
    - Replace this stub with the polling/interrupt key driver.
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>

EFI_STATUS
EFIAPI
ShennongKeyInputDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  DEBUG ((
    DEBUG_ERROR,
    "SHENNONGKEY: skeleton loaded; SPMI key matrix not yet ported "
    "(see Meizu21KeyInputDxe for the reference implementation)\n"
    ));
  return EFI_SUCCESS;
}
