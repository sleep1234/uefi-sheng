# shennong firmware platform (Xiaomi 14 Pro, SM8650)

Xiaomi 14 Pro (`shennong`, SM8650, Adreno 750). Derived from the
`meizu21Pkg` blueprint in `loolooc/uefi-sheng` (sheng branch): the register
bases match SM8550/SM8650, so the SM8650 differences (CPU 1+5+2, Adreno 750,
GMU 750.1) live in `UniXNU-sm8650-common` / `UniXNU-sm8550-common`.

This package is the board package referenced by the device `board.toml`
`[firmware]` section (`dsc = "Platforms/Xiaomi/shennongPkg/shennong.dsc"`).
It builds the `SILICIUM_UEFI.fd` loaded by ABL/DarwinBoot.

## Files

| Path | Role |
| --- | --- |
| `shennong.dsc` | Platform definition; PCDs, memory map, SMBIOS, framebuffer, components |
| `shennong.fdf` | Flash device image; FD geometry from board.toml `[firmware.defines]` |
| `shennong.dec` | Package declaration (library classes) |
| `Include/APRIORI.inc` / `DXE.inc` / `RAW.inc` | DXE dispatch order |
| `Drivers/ShennongBootMemoryDxe` | Adds DRAM from the entry DTB to the GCD map (board-agnostic FDT scan) |
| `Drivers/ShennongDisplayDepexDxe` | DEPEX gate ensuring GOP is up before UniXNU draws |
| `Drivers/ShennongGopLogDxe` | Logs the active GOP framebuffer geometry |
| `Drivers/ShennongKeyInputDxe` | SPMI key input (skeleton; port from Meizu21KeyInputDxe) |
| `Library/MemoryMapLib` | Platform memory map (FTD-driven; reconcile class contract with tree) |
| `Library/ConfigurationMapLib` | Platform configuration map (shared IMEM / DLOAD cookie / #CPUs) |

## Device-specific values (measured on 23116PN5BC)

- DRAM: 12 GiB @ `0x80000000` (`PcdSystemMemoryBase`)
- FD window: `0xA7100000` / `0x300000` (from board.toml `[firmware.defines]`)
- Framebuffer: 1440×3200, 32 bpp — `msm_drm.dsi_display0 =
  qcom,mdss_dsi_n2_42_02_0a_p2_dsc_cmd` (LTPO AMOLED, DSC, cmd mode)
- `SOC_TYPE = 2` (SM8650-AB, identical to Xiaomi 14 / houji)
- `SHENNONG_VENDOR_HW = 0` (Mu drivers only; previous stage GOP keeps scanning out)

## Build

Build through the UniXNU resolver against the `uefi-sheng` tree:

```sh
# from the uefi-sheng checkout, after adding this package under Platforms/Xiaomi/
stuart_build -c Platforms/Xiaomi/shennongPkg/shennong.dsc
# or via the UniXNU firmware build step (board.toml [firmware])
```

The resolved FD path is `Build/shennongPkg/{target}_CLANGPDB/FV/SILICIUM_UEFI.fd`,
matching `board.toml [firmware].fd`.

## Before first boot

- Confirm the 3 MiB FD at `0xA7100000` against the shennong ABL UEFI window.
- Confirm the GOP framebuffer geometry (1440×3200) matches the ABL-supplied
  buffer; if the panel reports different stride/format, adjust `PcdFrameBuffer*`.
- `SharedIMEMBaseAddr` / `DloadCookieAddr` in `ConfigurationMapLib` are seeded
  from the meizu21 reference — verify against the shennong `uefiplat.cfg`
  extracted from the `uefi` partition FV.
- `ShennongKeyInputDxe` is a skeleton; volume/power keys need the SPMI matrix
  ported from `Meizu21KeyInputDxe`.

## Memory and firmware inputs

Firmware input status (mirrors the device-repo `firmware/` manifest):

- COLLECTED: stock `uefiplat.cfg` from the `uefi` partition FV (see
  `firmware/` in the device repo).
- COLLECTED: memory map from the vendor_boot DTB — 35+ stock reserved-memory
  regions; `ShennongBootMemoryDxe` mirrors these plus the 3 MiB FD window.
- NOT REQUIRED: stock DXE driver binaries (encrypted); the open-source EDK2
  baseline with self-written drivers is used (`SHENNONG_VENDOR_HW=0`).

## Verification report (2026-11-05, real device)

Verified on 23116PN5BC / SN a59f9c76. Full evidence in
`docs/MEASURED-HARDWARE.md`.

- MEASURED: `uefiplat.cfg` from the `uefi_a` FV — `SharedIMEMBaseAddr =
  0x14680000`, `DloadCookieAddr = 0x01FD9000`, `NumCpus = 8`,
  `MaxMemoryRegions = 74`, `Version = 3`, `SecurityFlag = 0xC4`,
  `OsTypeString = "LA"`. The three values previously marked VERIFY in
  ConfigurationMapLib are confirmed correct.
- MEASURED: 40 reserved-memory regions in the vendor_boot DTB variant that
  matches `hw-revision = 0x20000` (Pineapple v2, msm-id 0x22D/0x20000).
  `ramoops_region = 0xA7000000/0x400000` contains the FD window
  `0xA7100000/0x300000` — the FD geometry in shennong.dsc is correct.
- MEASURED: GICv3 `0x17100000` + `0x17180000`, timer 19.2 MHz, PSCI
  `smc`, UFS `ufshc@1d84000` (3 reg groups), SPMI PMIC list.
- MEASURED: `dtbo_a` contains `splash_region` @ `0xFC400000` and
  `synaptics,tcm-spi`.
- MEASURED: partitions — `userdata` is 942.74 GiB (1 TB model), 123
  partitions total. The upstream root-volume window covers only 25% of it.
- VERIFIED (CI payload): `EFI/UniXNU/Payloads/kernel.macho.adt` is the
  10,208-byte board ADT (`model=Xiaomi-14-Pro`), byte-identical to
  `make-shennong-adt.py` output after masking the 64-byte random seed.

Still pending: firmware build result, FV inspection, `fastboot boot` on this
device, GOP output, interrupt behavior (GIC-Spurious patch; verify IPCC INTIDs).
The boot image cannot be packaged yet: no shennong v0 template
(`gzip(BootShim + SILICIUM_UEFI.fd)`) exists locally, on the device, or in
any CI artifact.
