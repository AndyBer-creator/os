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
	{ 0xb6e55bd2, __VMLINUX_SYMBOL_STR(rsd_nic_isPortSTPForwad_ret) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0x29c8073, __VMLINUX_SYMBOL_STR(rsd_vlan_adminEntry_get) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0x691e47c0, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_tx) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0x8145c32e, __VMLINUX_SYMBOL_STR(sys_port_linkStatus_ret) },
	{ 0x9e4123ae, __VMLINUX_SYMBOL_STR(rsd_trunk_member_get) },
	{ 0x2e4e5247, __VMLINUX_SYMBOL_STR(sys_util_bitmaskIsClear_ret) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xd698d112, __VMLINUX_SYMBOL_STR(rsd_gvrp_enable_set) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0x6f5cecac, __VMLINUX_SYMBOL_STR(osal_timer_stop) },
	{ 0xde526d35, __VMLINUX_SYMBOL_STR(rsd_vlan_portOperVlanMembership_get) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0xba6489ee, __VMLINUX_SYMBOL_STR(rsd_vlan_adminEntryExist_get) },
	{ 0xf9511502, __VMLINUX_SYMBOL_STR(rsd_vlan_portGvrpVlan_add) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xd87ddccf, __VMLINUX_SYMBOL_STR(drv_nic_pkt_free) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0x81612727, __VMLINUX_SYMBOL_STR(rsd_vlan_defaultVid_get) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0xf5920578, __VMLINUX_SYMBOL_STR(sys_port_lPortmask2uStr) },
	{ 0xe40e56e3, __VMLINUX_SYMBOL_STR(rsd_gvrp_init) },
	{ 0xfde685c2, __VMLINUX_SYMBOL_STR(osal_timer_start) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x16489634, __VMLINUX_SYMBOL_STR(rsd_vlan_gvrpVlan_create) },
	{ 0xb2ee8831, __VMLINUX_SYMBOL_STR(osal_timer_init) },
	{ 0x898ac19c, __VMLINUX_SYMBOL_STR(rsd_vlan_portGvrpVlan_del) },
	{ 0xa891b99c, __VMLINUX_SYMBOL_STR(rsd_vlan_operEntry_get) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xf6907b59, __VMLINUX_SYMBOL_STR(osal_time_udelay) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0x7cf8acf8, __VMLINUX_SYMBOL_STR(rsd_vlan_portTrunkAllowedVlan_get) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xe7601c7c, __VMLINUX_SYMBOL_STR(rsd_vlan_gvrpVlan_destroy) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x443a2a81, __VMLINUX_SYMBOL_STR(defdb_str_ret) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xaf4a52f1, __VMLINUX_SYMBOL_STR(sys_trunk_portRole_ret) },
	{ 0xebd9981f, __VMLINUX_SYMBOL_STR(rsd_vlan_operEntryExist_get) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=defdb,ski,osal,rtcore";

