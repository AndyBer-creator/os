#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xac8f33ba, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x825af07d, __VMLINUX_SYMBOL_STR(defdb_val_ret) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0x307d848e, __VMLINUX_SYMBOL_STR(rsd_l2_mac_clearByPort) },
	{ 0x6761c8d2, __VMLINUX_SYMBOL_STR(rsd_acl_entry_del) },
	{ 0x4e6a0f1b, __VMLINUX_SYMBOL_STR(rsd_vlan_portGuestVlanActive_set) },
	{ 0x7949bfbb, __VMLINUX_SYMBOL_STR(rsd_auth_macBasedState_set) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0x691e47c0, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_tx) },
	{ 0xa154964b, __VMLINUX_SYMBOL_STR(rsd_auth_init) },
	{ 0x95af61d, __VMLINUX_SYMBOL_STR(rsd_nic_pktVid_get) },
	{ 0x90401f22, __VMLINUX_SYMBOL_STR(rsd_l2_mac_search) },
	{ 0x2e4e5247, __VMLINUX_SYMBOL_STR(sys_util_bitmaskIsClear_ret) },
	{ 0x149edff3, __VMLINUX_SYMBOL_STR(osal_thread_exit) },
	{ 0x51dd93ed, __VMLINUX_SYMBOL_STR(rsd_auth_unauthOpenPort_add) },
	{ 0x3fe552bf, __VMLINUX_SYMBOL_STR(rsd_auth_eapolEnable_set) },
	{ 0x72fbf1bc, __VMLINUX_SYMBOL_STR(rsd_acl_entryCounter_get) },
	{ 0xd604eba2, __VMLINUX_SYMBOL_STR(osal_wait_event) },
	{ 0xed72abb9, __VMLINUX_SYMBOL_STR(sys_l3_ipv4Prefix2Mask_ret) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xd59843cc, __VMLINUX_SYMBOL_STR(rsd_vlan_portAdminPvid_get) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0x6f5cecac, __VMLINUX_SYMBOL_STR(osal_timer_stop) },
	{ 0x5bb93b52, __VMLINUX_SYMBOL_STR(rsd_auth_portEapolEnable_set) },
	{ 0xb1fb6e2b, __VMLINUX_SYMBOL_STR(svi_handler_register) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0xd8422a99, __VMLINUX_SYMBOL_STR(osal_wait_module_create) },
	{ 0xea3fb524, __VMLINUX_SYMBOL_STR(osal_time_sleep) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xdd68b650, __VMLINUX_SYMBOL_STR(rsd_sys_mgmtIPv4Addr_get) },
	{ 0x4f475e22, __VMLINUX_SYMBOL_STR(svi_handler_unregister) },
	{ 0x2cd41cdf, __VMLINUX_SYMBOL_STR(rsd_sys_portMacAddr_get) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0xce5f8332, __VMLINUX_SYMBOL_STR(rsd_auth_portBasedAuthStatus_set) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0xb9a47ba9, __VMLINUX_SYMBOL_STR(osal_timer_refresh) },
	{ 0xaac87c4c, __VMLINUX_SYMBOL_STR(rsd_auth_portBasedState_set) },
	{ 0xfde685c2, __VMLINUX_SYMBOL_STR(osal_timer_start) },
	{ 0x90d0a03, __VMLINUX_SYMBOL_STR(rsd_vlan_hostAuthorVlan_add) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0xb2ee8831, __VMLINUX_SYMBOL_STR(osal_timer_init) },
	{ 0xb6f3cc2b, __VMLINUX_SYMBOL_STR(osal_time_sysUptime_get) },
	{ 0x9960c536, __VMLINUX_SYMBOL_STR(rsd_l2_limit_handler_register) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x360541ea, __VMLINUX_SYMBOL_STR(rsd_auth_unauthOpenPort_del) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x18c736c7, __VMLINUX_SYMBOL_STR(rsd_acl_entryCounter_clr) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0xe41576c5, __VMLINUX_SYMBOL_STR(rsd_l2_mac_del) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x8db60739, __VMLINUX_SYMBOL_STR(osal_wake_up) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xd7c6e807, __VMLINUX_SYMBOL_STR(rsd_auth_unauthPktAction_set) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x88a22e72, __VMLINUX_SYMBOL_STR(rsd_auth_macBasedAuthMac_del) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x364a5512, __VMLINUX_SYMBOL_STR(rsd_l2_limit_portCtrl_set) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xaf4a52f1, __VMLINUX_SYMBOL_STR(sys_trunk_portRole_ret) },
	{ 0x82018df8, __VMLINUX_SYMBOL_STR(rsd_auth_macBasedAuthMac_add) },
	{ 0x66d16e6c, __VMLINUX_SYMBOL_STR(rsd_vlan_guestVid_set) },
	{ 0x2482e688, __VMLINUX_SYMBOL_STR(vsprintf) },
	{ 0x1bbabc11, __VMLINUX_SYMBOL_STR(rsd_acl_entry_add) },
	{ 0x79d6de9d, __VMLINUX_SYMBOL_STR(rsd_vlan_hostAuthorVlan_del) },
	{ 0x2fdebb69, __VMLINUX_SYMBOL_STR(rsd_vlan_portAuthorVlan_set) },
	{ 0x5a357732, __VMLINUX_SYMBOL_STR(ksi_msg_notify) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=defdb,ski,rtcore,osal,net,ksi";

