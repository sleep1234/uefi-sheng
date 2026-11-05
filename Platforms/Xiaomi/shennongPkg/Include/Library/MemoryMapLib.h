/** @file
  Shennong platform memory map library API.

  NOTE: this header mirrors the local board implementation. If you copy the
  canonical MemoryMapLib from loolooc/uefi-sheng (meizu21Pkg), replace this
  header with the upstream one so the class contract matches.
**/
#ifndef __SHENNONG_MEMORY_MAP_LIB_H__
#define __SHENNONG_MEMORY_MAP_LIB_H__

#include <Uefi.h>

#define SHENNONG_MEMMAP_MAX_RANGES  64

typedef struct {
  UINT64  Base;
  UINT64  End;       // exclusive
} SHENNONG_MEMORY_RANGE;

/**
  Return the entry (kernel handoff) device tree address, or 0.
**/
UINT64
EFIAPI
MemoryMapLibGetEntryDtb (
  VOID
  );

/**
  Collect reserved-memory regions from the entry DTB.
  Returns the number written (truncated at MaxRegions).
**/
UINTN
EFIAPI
MemoryMapLibGetReservedRegions (
  OUT SHENNONG_MEMORY_RANGE  *Regions,
  IN  UINTN                  MaxRegions
  );

/**
  Number of reserved regions measured on the real device (2026-11-05).
**/
UINTN
EFIAPI
MemoryMapLibGetMeasuredReservedCount (
  VOID
  );

/**
  Static measured reserved-region table (fallback / validation).
**/
CONST SHENNONG_MEMORY_RANGE *
EFIAPI
MemoryMapLibGetMeasuredReserved (
  VOID
  );

#endif // __SHENNONG_MEMORY_MAP_LIB_H__
