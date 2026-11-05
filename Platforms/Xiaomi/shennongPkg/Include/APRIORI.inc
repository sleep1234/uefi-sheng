// Stage-1 firmware modules for Xiaomi 14 Pro (shennong). Mu EDK2 baseline +
// shennong custom drivers. No stock vendor binaries: the previous stage's
// display keeps scanning out, so the boot manager is visible without any
// vendor hardware driver (SHENNONG_VENDOR_HW=0).
APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF shennongPkg/Drivers/ShennongBootMemoryDxe/ShennongBootMemoryDxe.inf
  INF shennongPkg/Drivers/ShennongKeyInputDxe/ShennongKeyInputDxe.inf
}
