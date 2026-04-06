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
	{ 0x217ee6b4, __VMLINUX_SYMBOL_STR(rsd_stp_enable_set) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0x691e47c0, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_tx) },
	{ 0xa23c95ea, __VMLINUX_SYMBOL_STR(rsd_port_enable_get) },
	{ 0x149edff3, __VMLINUX_SYMBOL_STR(osal_thread_exit) },
	{ 0xebed9308, __VMLINUX_SYMBOL_STR(rsd_stp_mstpState_get) },
	{ 0x840d29e7, __VMLINUX_SYMBOL_STR(rsd_stp_init) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x2cd41cdf, __VMLINUX_SYMBOL_STR(rsd_sys_portMacAddr_get) },
	{ 0x47c61ae, __VMLINUX_SYMBOL_STR(rsd_stp_mstpState_set) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x1355eb2f, __VMLINUX_SYMBOL_STR(rsd_stp_mstpInstance_create) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0x70313942, __VMLINUX_SYMBOL_STR(rsd_port_speedDuplex_get) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x16d741a1, __VMLINUX_SYMBOL_STR(rsd_port_errDisable_set) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0xb624a21f, __VMLINUX_SYMBOL_STR(sys_trunk_portTrunkInfo_ret) },
	{ 0x2a7e76fd, __VMLINUX_SYMBOL_STR(rsd_vlan_stg_set) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0x153b583f, __VMLINUX_SYMBOL_STR(sys_port_lPort2Type_ret) },
	{ 0x4286b1bc, __VMLINUX_SYMBOL_STR(sys_trunk_activeMember_get) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0xd159c25f, __VMLINUX_SYMBOL_STR(rsd_stp_mstpInstance_destroy) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xaf4a52f1, __VMLINUX_SYMBOL_STR(sys_trunk_portRole_ret) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=defdb,ski,rtcore";

