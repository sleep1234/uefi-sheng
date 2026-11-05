// SPDX-License-Identifier: BSD-3-Clause
// Xiaomi 14 Pro (shennong, SM8650) configuration map.
// Values from the real device uefiplat.cfg extracted 2026-11-05 (SN a59f9c76).
#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR
gConfigurationDescriptor[] = {
  {"AllowNonPersistentVarsInRetail", 0x1},
  {"DDRInfoNotifyFlag", 0x0},
  {"DRAM_CLK_PERIOD_ADDR", 0x240BA050},
  {"DetectRetailUserAttentionHotkey", 0x0},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"DloadCookieAddr", 0x01FD9000},            // [MEASURED] uefiplat.cfg
  {"DloadCookieValue", 0x10},                  // [MEASURED] uefiplat.cfg
  {"EarlyInitCoreCnt", 0x2},                   // [MEASURED] uefiplat.cfg
  {"EnableACPIFallback", 0x0},
  {"EnableDisplayImageFv", 0x1},               // [MEASURED] uefiplat.cfg
  {"EnableDisplayThread", 0x1},                // [MEASURED] uefiplat.cfg
  {"EnableLogFsSyncInRetail", 0x0},
  {"EnableMultiCoreFvDecompression", 0x0},
  {"EnableMultiThreading", 0x1},               // [MEASURED] uefiplat.cfg
  {"EnableOEMSetupAppInRetail", 0x0},
  {"EnableSDHCSwitch", 0x1},
  {"EnableShell", 0x1},
  {"EnableUefiSecAppDebugLogDump", 0x0},
  {"EnableUfsIOC", 0x1},                       // [MEASURED] uefiplat.cfg
  {"EnableVariablePolicyEngine", 0x0},
  {"LoadAutoImageInPILFlag", 0x1},
  {"MaxCoreCnt", 0x8},
  {"MaxLogFileSize", 0x400000},
  {"NumActiveCores", 0x8},                     // [MEASURED] uefiplat.cfg
  {"NumCpus", 0x8},                            // [MEASURED] uefiplat.cfg
  {"PilSubsysDbgCookieAddr", 0x146AA6DC},
  {"PilSubsysDbgCookieVal", 0x53444247},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"SecPagePoolCount", 0x800},
  {"SecurityFlag", 0xC4},                      // [MEASURED] uefiplat.cfg
  {"SharedIMEMBaseAddr", 0x14680000},          // [MEASURED] uefiplat.cfg
  {"ShmBridgememSize", 0xA00000},              // [MEASURED] uefiplat.cfg
  {"USBHS1_Config", 0x0},
  {"UefiMemUseThreshold", 0xE1},
  {"UfsSmmuConfigForOtherBootDev", 0x1},      // [MEASURED] uefiplat.cfg
  {"UsbFnIoRevNum", 0x10001}
};

VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount)
{
  *ConfigurationDescriptor      = gConfigurationDescriptor;
  *ConfigurationDescriptorCount = ARRAY_SIZE (gConfigurationDescriptor);
}