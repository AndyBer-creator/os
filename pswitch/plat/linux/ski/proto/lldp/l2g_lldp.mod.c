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
	{ 0x17966798, __VMLINUX_SYMBOL_STR(sys_notifier_event_dispatcher) },
	{ 0x62149e84, __VMLINUX_SYMBOL_STR(rsd_sys_info_get) },
	{ 0x6c185fb8, __VMLINUX_SYMBOL_STR(board_lldp_poePortL2Data_set) },
	{ 0x825af07d, __VMLINUX_SYMBOL_STR(defdb_val_ret) },
	{ 0xc25f2e49, __VMLINUX_SYMBOL_STR(rsd_switch_maxFrameSize_get) },
	{ 0x7de15fcc, __VMLINUX_SYMBOL_STR(sys_dbg_isSet_ret) },
	{ 0xb6e55bd2, __VMLINUX_SYMBOL_STR(rsd_nic_isPortSTPForwad_ret) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0xa0e708a0, __VMLINUX_SYMBOL_STR(rsd_port_descp_get) },
	{ 0x58cc3325, __VMLINUX_SYMBOL_STR(sys_port_lPort2uStr) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0x691e47c0, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_tx) },
	{ 0xdf901b4e, __VMLINUX_SYMBOL_STR(rsd_acl_rsv_ruleEnable_set) },
	{ 0x95af61d, __VMLINUX_SYMBOL_STR(rsd_nic_pktVid_get) },
	{ 0x149edff3, __VMLINUX_SYMBOL_STR(osal_thread_exit) },
	{ 0x34d39594, __VMLINUX_SYMBOL_STR(rsd_vlan_name_get) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xda64c7c9, __VMLINUX_SYMBOL_STR(voice_vlan_state_get) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0xde526d35, __VMLINUX_SYMBOL_STR(rsd_vlan_portOperVlanMembership_get) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xdd68b650, __VMLINUX_SYMBOL_STR(rsd_sys_mgmtIPv4Addr_get) },
	{ 0x2cd41cdf, __VMLINUX_SYMBOL_STR(rsd_sys_portMacAddr_get) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0x81612727, __VMLINUX_SYMBOL_STR(rsd_vlan_defaultVid_get) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0xd1d3a6e6, __VMLINUX_SYMBOL_STR(rsd_sys_jiffies_get) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x61651be, __VMLINUX_SYMBOL_STR(strcat) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0x26f3a9ff, __VMLINUX_SYMBOL_STR(rsd_vlan_voiceVid_get) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xed741a60, __VMLINUX_SYMBOL_STR(rsd_port_macPhyInfo_get) },
	{ 0xca3458c8, __VMLINUX_SYMBOL_STR(sys_port_isLPortPoe_ret) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x62361979, __VMLINUX_SYMBOL_STR(rsd_sys_intfIpv6Addr_get) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0x186e79ae, __VMLINUX_SYMBOL_STR(rsd_sys_hz_get) },
	{ 0xef17037, __VMLINUX_SYMBOL_STR(sys_util_mac2Str) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0xd33c9bc3, __VMLINUX_SYMBOL_STR(rsd_trunk_port2Trunk_get) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0xde421261, __VMLINUX_SYMBOL_STR(board_lldp_poePortL2Data_get) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0xf98935f2, __VMLINUX_SYMBOL_STR(sys_util_ip2Str) },
	{ 0x309493cb, __VMLINUX_SYMBOL_STR(rsd_mirror_group_get) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x88e77e1f, __VMLINUX_SYMBOL_STR(rsd_vlan_portOperPvid_get) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x4b1a69d5, __VMLINUX_SYMBOL_STR(voice_vlan_qos_get) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ski,defdb,rtcore,l2g_voice_vlan";

