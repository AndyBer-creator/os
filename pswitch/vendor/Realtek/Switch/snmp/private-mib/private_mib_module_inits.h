/* modify by hand. */
#ifdef CONFIG_PRIVATE_MIB

#ifdef CONFIG_SYS_MIB_POE_CONF
  if (should_init("PoeMib")) init_PoeMib();
#endif

#ifdef CONFIG_SYS_MIB_HARDWAREINFO_CONF
  if (should_init("sysHWMib")) init_sysHWMib();
#endif
#ifdef CONFIG_SYS_MIB_PORTCTRL_CONF
  if (should_init("sysPortCtrlMib")) init_sysPortCtrlMib();
#endif
#ifdef CONFIG_SYS_MIB_DHCPSNOOPING_CONF
  if (should_init("dhcpSnoopingMib")) init_dhcpSnoopingMib();
#endif

 if (should_init("sysMonitorMib")) init_sysMonitorMib();
if (should_init("sysDeviceMgmtMib")) init_sysDeviceMgmtMib();

if (should_init("snmptrapMgmtMib")) init_snmptrapMgmtMib();

if (should_init("staticVlanCtrlMib")) init_staticVlanCtrlMib();

if (should_init("L2SwitchMib")) init_L2SwitchMib();
#endif
