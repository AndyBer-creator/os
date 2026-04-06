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
	{ 0x9f978d3e, __VMLINUX_SYMBOL_STR(rsd_mcast_lookupMode_get) },
	{ 0x96b3eb7, __VMLINUX_SYMBOL_STR(unitMapStruct) },
	{ 0xdb22feb7, __VMLINUX_SYMBOL_STR(rsd_ip6McastEntryIncCPU_set) },
	{ 0xeaf1150e, __VMLINUX_SYMBOL_STR(rsd_mcast_igmp_unKnownMcast_action_set) },
	{ 0x825af07d, __VMLINUX_SYMBOL_STR(defdb_val_ret) },
	{ 0x79abce7c, __VMLINUX_SYMBOL_STR(rsd_ipMcastEntry_add) },
	{ 0xb6e55bd2, __VMLINUX_SYMBOL_STR(rsd_nic_isPortSTPForwad_ret) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0xc44d2ad2, __VMLINUX_SYMBOL_STR(rsd_ipMcastEntry_set) },
	{ 0x8eef3285, __VMLINUX_SYMBOL_STR(rsd_ip6McastEntry_get) },
	{ 0x29c8073, __VMLINUX_SYMBOL_STR(rsd_vlan_adminEntry_get) },
	{ 0x691e47c0, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_tx) },
	{ 0xc38183b4, __VMLINUX_SYMBOL_STR(rsd_port_linkStatus_get) },
	{ 0x858cde32, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_port_add) },
	{ 0x95af61d, __VMLINUX_SYMBOL_STR(rsd_nic_pktVid_get) },
	{ 0x9e4123ae, __VMLINUX_SYMBOL_STR(rsd_trunk_member_get) },
	{ 0x2e4e5247, __VMLINUX_SYMBOL_STR(sys_util_bitmaskIsClear_ret) },
	{ 0x149edff3, __VMLINUX_SYMBOL_STR(osal_thread_exit) },
	{ 0xc9896d01, __VMLINUX_SYMBOL_STR(rsd_ip6McastEntry_del) },
	{ 0x6b356f7b, __VMLINUX_SYMBOL_STR(rsd_ip6McastEntry_set) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xb8721947, __VMLINUX_SYMBOL_STR(rsd_igmp_enable_set) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0x7193af99, __VMLINUX_SYMBOL_STR(rsd_nic_pktBlkPM_flood) },
	{ 0x1b4eaa1f, __VMLINUX_SYMBOL_STR(sys_util_bitmaskIsEqual_ret) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xd87ddccf, __VMLINUX_SYMBOL_STR(drv_nic_pkt_free) },
	{ 0xfc5c8c66, __VMLINUX_SYMBOL_STR(rsd_igmp_lookupMode_set) },
	{ 0x7c3e0df8, __VMLINUX_SYMBOL_STR(rsd_mld_lookupMode_set) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xadbfb468, __VMLINUX_SYMBOL_STR(sys_l3_ipPoolNum_ret) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x9ea702fb, __VMLINUX_SYMBOL_STR(hwp_myHwProfile) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0xbd3ebb66, __VMLINUX_SYMBOL_STR(osal_time_usecs_get) },
	{ 0x6784be53, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_delete) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0xf5920578, __VMLINUX_SYMBOL_STR(sys_port_lPortmask2uStr) },
	{ 0xa608948d, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_create) },
	{ 0x7de367ef, __VMLINUX_SYMBOL_STR(rsd_mcast_ipMcastPkt2CpuEnable_set) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0xc10e3619, __VMLINUX_SYMBOL_STR(rsd_mld_enable_set) },
	{ 0xfad1a86f, __VMLINUX_SYMBOL_STR(rsd_mcast_unKnown_drop_set) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0x3eb9800, __VMLINUX_SYMBOL_STR(rsd_mcast_mcastLeakyEnable_set) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x8f90d3bd, __VMLINUX_SYMBOL_STR(rsd_ipMcastEntry_get) },
	{ 0x90d5f5ea, __VMLINUX_SYMBOL_STR(rsd_l2McastEntryIncCPU_set) },
	{ 0xa891b99c, __VMLINUX_SYMBOL_STR(rsd_vlan_operEntry_get) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0x73e4f852, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_port_delete) },
	{ 0xd7c282a, __VMLINUX_SYMBOL_STR(rsd_vlan_portAdminIgrFilterEnable_get) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x876d7cd7, __VMLINUX_SYMBOL_STR(rsd_l2McastEntry_del) },
	{ 0xb624a21f, __VMLINUX_SYMBOL_STR(sys_trunk_portTrunkInfo_ret) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0xeac9120c, __VMLINUX_SYMBOL_STR(rsd_l2McastEntry_get) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0x6e714b8, __VMLINUX_SYMBOL_STR(rsd_l2McastEntry_set) },
	{ 0x5abd16aa, __VMLINUX_SYMBOL_STR(rsd_ipMcastEntry_del) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xc8e211e4, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_clone) },
	{ 0x5c4e4f3a, __VMLINUX_SYMBOL_STR(rsd_nic_checksum_get) },
	{ 0xb3c739ae, __VMLINUX_SYMBOL_STR(rsd_mcast_l2_addressRsv_set) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0x38c87f7b, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_entry_by_used_cnt) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x925b257e, __VMLINUX_SYMBOL_STR(sys_l3_ipPoolSortEntryPtr_ret) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xca5fda18, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_is_exist_by_mac_vlan) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xaf4a52f1, __VMLINUX_SYMBOL_STR(sys_trunk_portRole_ret) },
	{ 0xd15df705, __VMLINUX_SYMBOL_STR(rsd_l2_mcast_entry_used_cnt) },
	{ 0xb6d79311, __VMLINUX_SYMBOL_STR(rsd_ip6McastEntry_add) },
	{ 0xb0258d8b, __VMLINUX_SYMBOL_STR(rsd_ipMcastEntryIncCPU_set) },
	{ 0x54103006, __VMLINUX_SYMBOL_STR(sys_trunk_member_get) },
	{ 0xe63ded0f, __VMLINUX_SYMBOL_STR(rsd_l2McastEntry_add) },
	{ 0xebd9981f, __VMLINUX_SYMBOL_STR(rsd_vlan_operEntryExist_get) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ski,rtcore,defdb";

