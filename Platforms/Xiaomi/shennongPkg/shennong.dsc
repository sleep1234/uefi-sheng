##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#  Xiaomi 14 Pro (shennong, SM8650) UniXNU board package.
#  Derived from the meizu21Pkg blueprint (loolooc/uefi-sheng, sheng branch):
#  register bases match SM8550/SM8650, the SM8650 differences (CPU 1+5+2,
#  Adreno 750, GMU 750.1) live in UniXNU-sm8650-common / UniXNU-sm8550-common.
#  Device-specific values below are measured on 23116PN5BC (see docs/hardware.md).
##

[Defines]
  PLATFORM_NAME                  = shennong
  PLATFORM_GUID                  = 9B3F2C81-4E5A-4C7D-9B1E-2D6F8A0C3E57
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/shennongPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = shennongPkg/shennong.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = Mu drivers only. The previous stage's display keeps scanning out, so
  #     the boot manager is visible without any vendor hardware driver.
  # 1 = also load the stock vendor hardware drivers (SMEM, clocks, RPMh, UFS,
  #     USB, charger, ...). Their initialization currently blanks the panel.
  #
  SHENNONG_VENDOR_HW             = 0

  #
  # 2 = SM8650-AB (Snapdragon 8 Gen 3 standard, as Xiaomi 14 / shennong)
  # 0,1,3,4 = other SM8650 variants (see LanaiPkg)
  #
  SOC_TYPE                       = 2

!include LanaiPkg/LanaiPkg.dsc.inc

!if ($(USE_CUSTOM_DISPLAY_DRIVER) == 1) && ($(SHENNONG_VENDOR_HW) == 0)
  !error The stock DisplayDxe chain needs SHENNONG_VENDOR_HW = 1
!endif

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|shennongPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|shennongPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf

[PcdsFixedAtBuild]
  #
  # DDR Memory (measured: DRAM 12 GiB starting at 0x80000000)
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack (SM8650 layout: FD at 0xA7100000, size 0x300000; stack sits
  # above the FD window, same as sheng/houji/meizu21)
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"shennong"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"Xiaomi 14 Pro"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"23116PN5BC"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"shennong"

  #
  # Simple Frame Buffer
  #
  # shennong stock panel: 6.73" 1440x3200 LTPO AMOLED, DSC, cmd mode
  # (msm_drm.dsi_display0 = qcom,mdss_dsi_n2_42_02_0a_p2_dsc_cmd).
  # These are the MEASURED ABL-supplied framebuffer geometry; the previous
  # stage's GOP keeps scanning out so the UniXNU menu is visible.
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1440
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|3200
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32
  # Scale 3 gives 56 lines. The log wraps, so the screen always shows the
  # newest lines; the platform debug level is left unchanged.
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferFontScale|3
  # Pause per log line so a video of the screen shows what loads last.
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferDelay|30000

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
!if $(SHENNONG_VENDOR_HW) == 0
  # Intermittent stall right after "MMU: enabled": PlatformPeim scanned the
  # stock UEFI's leftover FD window and called into its scheduler protocol.
  # Without the stock vendor drivers nothing reads those HOBs.
  gQcomPkgTokenSpaceGuid.PcdBuildXblHobs|FALSE
!endif

[LibraryClasses]
  UniXNULib|UniXNUPkg/Library/UniXNULib/UniXNULib.inf
  FdtLib|MdePkg/Library/BaseFdtLib/BaseFdtLib.inf

[Components]
  #
  # UniXNU application + registrar + custom board drivers
  #
  UniXNUPkg/Application/UniXNU/UniXNU.inf
  UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf
  shennongPkg/Drivers/ShennongDisplayDepexDxe/ShennongDisplayDepexDxe.inf
  shennongPkg/Drivers/ShennongGopLogDxe/ShennongGopLogDxe.inf
  shennongPkg/Drivers/ShennongBootMemoryDxe/ShennongBootMemoryDxe.inf
  UniXNUPkg/Driver/UniXNURamDiskDxe/UniXNURamDiskDxe.inf
  shennongPkg/Drivers/ShennongKeyInputDxe/ShennongKeyInputDxe.inf
