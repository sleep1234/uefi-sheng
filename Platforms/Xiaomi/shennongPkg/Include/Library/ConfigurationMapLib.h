/** @file
  Shennong platform configuration map library API.

  NOTE: this header mirrors the local board implementation. If you copy the
  canonical ConfigurationMapLib from loolooc/uefi-sheng (meizu21Pkg), replace
  this header with the upstream one so the class contract matches.
**/
#ifndef __SHENNONG_CONFIGURATION_MAP_LIB_H__
#define __SHENNONG_CONFIGURATION_MAP_LIB_H__

#include <Uefi.h>

/**
  Number of CPUs exposed to the OS (shennong: 8, 1+5+2).
**/
UINT32
EFIAPI
ConfigurationMapLibGetNumCpus (
  VOID
  );

/**
  Shared IMEM base address (VERIFY against extracted uefiplat.cfg).
**/
UINT64
EFIAPI
ConfigurationMapLibGetSharedImem (
  VOID
  );

/**
  DLOAD cookie address (VERIFY against extracted uefiplat.cfg).
**/
UINT64
EFIAPI
ConfigurationMapLibGetDloadCookie (
  VOID
  );

#endif // __SHENNONG_CONFIGURATION_MAP_LIB_H__
