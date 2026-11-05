/** @file
  Shennong platform memory map library.

  This is the board-specific MemoryMapLib consumed by shennong.dsc. The SM8650
  memory layout is taken from a REAL Xiaomi 14 Pro (23116PN5BC) measured on
  2026-11-05: the vendor_boot_a DTB variant matching hw-revision 0x20000
  ("Pineapple v2 SoC", qcom,msm-id 0x22D/0x20000) contains 40 reserved-memory
  regions. See docs/MEASURED-HARDWARE.md §4 and
  _research/device/hardware-measured.json.

  NOTE: the vendor_boot base DTB has an empty /memory reg (XBL fills DRAM at
  boot), so the FDT-driven scan at runtime is the primary source. The static
  table below is the measured reference used for validation / fallback when no
  valid entry DTB is published.

  IMPORTANT: the authoritative MemoryMapLib class contract (function names and
  signatures) lives in the uefi-sheng tree (LanaiPkg / QcomPkg). If the build
  reports unresolved symbols from this class, copy the real implementation from
  loolooc/uefi-sheng Platforms/Meizu/meizu21Pkg/Library/MemoryMapLib and keep
  the measured table.

  The actual GCD memory addition is performed by ShennongBootMemoryDxe at DXE
  dispatch; this library exposes the entry DTB and a reserved-range accessor.
**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/FdtLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryMapLib.h>

STATIC UINT64  mEntryDtb = 0;

//
// Measured reserved-memory regions (40), sorted by base address.
// { Base, End(exclusive) } — from vendor_boot_a DTB @0x0, hw-revision 0x20000.
// The UEFI FD window 0xA7100000..0xA7400000 sits inside ramoops_region.
//
STATIC CONST SHENNONG_MEMORY_RANGE  mMeasuredReserved[] = {
  { 0x80000000ULL, 0x80E00000ULL },  // gunyah_hyp_region (DRAM base)
  { 0x80E00000ULL, 0x81200000ULL },  // cpusys_vm_region
  { 0x816E0000ULL, 0x816E0800ULL },  // dump_display_region
  { 0x81A00000ULL, 0x81A40000ULL },  // xbl_dtlog_region
  { 0x81BFF000ULL, 0x81C00000ULL },  // xbl_md_region
  { 0x81C00000ULL, 0x81C60000ULL },  // xbl_aop_image_region
  { 0x81C60000ULL, 0x81C80000ULL },  // aop_cmd_db_region
  { 0x81C80000ULL, 0x81CF4000ULL },  // aop_tme_uefi_merged_region
  { 0x81CF4000ULL, 0x81CF5000ULL },  // chipinfo_region
  { 0x81D00000ULL, 0x81F00000ULL },  // smem_region
  { 0x81F00000ULL, 0x81F20000ULL },  // adsp_mhi_region
  { 0x824A0000ULL, 0x825A0000ULL },  // pvmfw_region
  { 0x82600000ULL, 0x82700000ULL },  // global_sync_region
  { 0x82700000ULL, 0x82800000ULL },  // tz_stat_region
  { 0x82800000ULL, 0x84800000ULL },  // qdss_region
  { 0x84800000ULL, 0x84A00000ULL },  // qlink_logging_region
  { 0x84A00000ULL, 0x84C00000ULL },  // xbl_ramdump_region
  { 0x86B00000ULL, 0x8B400000ULL },  // mpss_dsm_region
  { 0x8B400000ULL, 0x8BC00000ULL },  // mpss_dsm_region_2
  { 0x8BC00000ULL, 0x9B000000ULL },  // mpss_region
  { 0x9B000000ULL, 0x9B080000ULL },  // q6_mpss_dtb_region
  { 0x9B080000ULL, 0x9B090000ULL },  // ipa_fw_region
  { 0x9B090000ULL, 0x9B09A000ULL },  // ipa_gsi_region
  { 0x9B09A000ULL, 0x9B09C000ULL },  // gpu_microcode_region
  { 0x9B09C000ULL, 0x9B0A0000ULL },  // lost_reg_mem
  { 0x9B0A0000ULL, 0x9B280000ULL },  // spss_region_region
  { 0x9B280000ULL, 0x9B2E0000ULL },  // spu_secure_shared_memory_region
  { 0x9B2E0000ULL, 0x9B300000ULL },  // spu_secure_shared_memory_region
  { 0x9B300000ULL, 0x9BB00000ULL },  // camera_region
  { 0x9BB00000ULL, 0x9C300000ULL },  // video_region
  { 0x9C300000ULL, 0x9CA00000ULL },  // cvp_region
  { 0x9CA00000ULL, 0x9DE00000ULL },  // cdsp_region
  { 0x9DE00000ULL, 0x9DE80000ULL },  // q6_cdsp_dtb_region
  { 0x9DE80000ULL, 0x9DF00000ULL },  // q6_adsp_dtb_region
  { 0x9DF00000ULL, 0xA2580000ULL },  // adspslpi_region
  { 0xA7000000ULL, 0xA7400000ULL },  // ramoops_region (contains UEFI FD window)
  { 0xD8000000ULL, 0xD8800000ULL },  // tz_merged_region
  { 0xF3800000ULL, 0xF7C00000ULL },  // trust_ui_vm_region
  { 0xFF100000ULL, 0xFF3DD000ULL },  // hwfence-shmem
  { 0xFF800000ULL, 0xFFE00000ULL },  // llcc_lpi_region
};

/**
  Return the number of measured reserved regions (for validation / fallback).
**/
UINTN
EFIAPI
MemoryMapLibGetMeasuredReservedCount (
  VOID
  )
{
  return ARRAY_SIZE (mMeasuredReserved);
}

/**
  Return a pointer to the static measured reserved-region table.
**/
CONST SHENNONG_MEMORY_RANGE *
EFIAPI
MemoryMapLibGetMeasuredReserved (
  VOID
  )
{
  return mMeasuredReserved;
}

/**
  Return the entry (kernel handoff) device tree address published in the
  Silicium entry-argument HOB, or 0 if not present.
**/
UINT64
EFIAPI
MemoryMapLibGetEntryDtb (
  VOID
  )
{
  return mEntryDtb;
}

/**
  Collect reserved-memory regions from the entry DTB into Regions.
  Returns the number written (truncated at MaxRegions).
**/
UINTN
EFIAPI
MemoryMapLibGetReservedRegions (
  OUT SHENNONG_MEMORY_RANGE  *Regions,
  IN  UINTN                  MaxRegions
  )
{
  CONST VOID  *Fdt;
  INT32       Reserved;
  INT32       Node;
  UINTN       Count;

  Fdt = (CONST VOID *)(UINTN)mEntryDtb;
  if ((Fdt == NULL) || (FdtCheckHeader (Fdt) != 0) || (Regions == NULL)) {
    return 0;
  }

  Count    = 0;
  Reserved = FdtPathOffset (Fdt, "/reserved-memory");
  if (Reserved < 0) {
    return 0;
  }

  for (Node = FdtFirstSubnode (Fdt, Reserved); Node >= 0; Node = FdtNextSubnode (Fdt, Node)) {
    CONST UINT8  *Reg;
    INT32        Length;
    UINT64       Base;
    UINT64       Size;

    Reg = FdtGetProp (Fdt, Node, "reg", &Length);
    if ((Reg == NULL) || (Length < (INT32)(4 * sizeof (UINT32)))) {
      continue;
    }

    Base = Fdt64ToCpu (ReadUnaligned64 (Reg));
    Size = Fdt64ToCpu (ReadUnaligned64 (Reg + 2 * sizeof (UINT32)));
    if ((Size == 0) || (Count >= MaxRegions)) {
      continue;
    }

    Regions[Count].Base = Base;
    Regions[Count].End  = Base + Size;
    Count++;
  }

  return Count;
}

STATIC
VOID
EFIAPI
MemoryMapLibConstructor (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *Hob;

  DEBUG ((DEBUG_ERROR, "SHENNONGMAP: measured reserved regions = %u; "
          "FD window 0xA7100000-0xA7400000 inside ramoops_region 0xA7000000-0xA7400000\n",
          (UINT32)ARRAY_SIZE (mMeasuredReserved)));

  Hob = GetFirstGuidHob (&gSiliciumEntryArgHobGuid);
  if (Hob != NULL) {
    mEntryDtb = *(UINT64 *)GET_GUID_HOB_DATA (Hob);
    DEBUG ((DEBUG_ERROR, "SHENNONGMAP: entry DTB 0x%lx, %u bytes\n",
            mEntryDtb,
            (mEntryDtb != 0) ? (UINT32)FdtTotalSize ((VOID *)(UINTN)mEntryDtb) : 0));
  } else {
    DEBUG ((DEBUG_ERROR, "SHENNONGMAP: no entry argument HOB; using measured table\n"));
  }
}
