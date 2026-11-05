/** @file
  Shennong platform configuration map library.

  Supplies the board configuration consumed by the DXE/PEI core (number of
  CPUs, shared IMEM / DLOAD cookie addresses, etc).

  All constants below are MEASURED on a real Xiaomi 14 Pro (23116PN5BC,
  SN a59f9c76, HyperOS OS4.0.25.0.XEOCNXM) on 2026-11-05: extracted from the
  stock `uefiplat.cfg` embedded in the `uefi_a` partition FV (text block
  0x5103C..0x51F5D, uefi_a sha256 37be230e...). See docs/MEASURED-HARDWARE.md §5.

  They were originally seeded from the meizu21 reference; the real device
  confirms the values are identical, so the VERIFY markers are now removed.

  IMPORTANT: the authoritative ConfigurationMapLib class contract (function
  names/signatures) lives in the uefi-sheng tree (QcomPkg). If the build reports
  unresolved symbols from this class, copy the real implementation from
  loolooc/uefi-sheng Platforms/Meizu/meizu21Pkg/Library/ConfigurationMapLib
  and keep these measured constants.
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/ConfigurationMapLib.h>

// ---- uefiplat.cfg [Config] / [ConfigParameters] (实机) ----
#define SHENNONG_CFG_VERSION            3
#define SHENNONG_MAX_MEMORY_REGIONS    74
#define SHENNONG_CONFIG_PARAM_COUNT    64
#define SHENNONG_OS_TYPE_STRING        "LA"

#define SHENNONG_SHARED_IMEM          0x14680000ULL
#define SHENNONG_DLOAD_COOKIE         0x01FD9000ULL
#define SHENNONG_DLOAD_COOKIE_VALUE   0x10
#define SHENNONG_NUM_CPUS             8
#define SHENNONG_NUM_ACTIVE_CORES     8
#define SHENNONG_SECURITY_FLAG        0xC4
#define SHENNONG_SHM_BRIDGE_MEM_SIZE  0xA00000ULL
#define SHENNONG_EARLY_INIT_CORE_CNT  2
#define SHENNONG_DRAM_CLK_PERIOD_ADDR 0x240BA050ULL

/**
  Return the number of CPUs the platform exposes to the OS.
  uefiplat.cfg: NumCpus = 8, NumActiveCores = 8 (measured).
**/
UINT32
EFIAPI
ConfigurationMapLibGetNumCpus (
  VOID
  )
{
  return SHENNONG_NUM_CPUS;
}

/**
  Return the shared IMEM base address.
  uefiplat.cfg: SharedIMEMBaseAddr = 0x14680000 (measured).
**/
UINT64
EFIAPI
ConfigurationMapLibGetSharedImem (
  VOID
  )
{
  return SHENNONG_SHARED_IMEM;
}

/**
  Return the DLOAD cookie address.
  uefiplat.cfg: DloadCookieAddr = 0x01FD9000, DloadCookieValue = 0x10 (measured).
**/
UINT64
EFIAPI
ConfigurationMapLibGetDloadCookie (
  VOID
  )
{
  return SHENNONG_DLOAD_COOKIE;
}
