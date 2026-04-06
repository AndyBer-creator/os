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
	{ 0x99076c4c, __VMLINUX_SYMBOL_STR(rsd_erps_port_set) },
	{ 0xff88f8c8, __VMLINUX_SYMBOL_STR(rsd_erps_ints_cfg_notice) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0x307d848e, __VMLINUX_SYMBOL_STR(rsd_l2_mac_clearByPort) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0x691e47c0, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_tx) },
	{ 0xc38183b4, __VMLINUX_SYMBOL_STR(rsd_port_linkStatus_get) },
	{ 0xadba090a, __VMLINUX_SYMBOL_STR(rtk_vlan_stg_set) },
	{ 0x95af61d, __VMLINUX_SYMBOL_STR(rsd_nic_pktVid_get) },
	{ 0x4c3119e3, __VMLINUX_SYMBOL_STR(rtk_vlan_port_add) },
	{ 0x90166553, __VMLINUX_SYMBOL_STR(rsd_erps_enable_set) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0x6f5cecac, __VMLINUX_SYMBOL_STR(osal_timer_stop) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xeed33f44, __VMLINUX_SYMBOL_STR(rtk_stp_mstpState_set) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x4930457c, __VMLINUX_SYMBOL_STR(sys_logic_to_physic_port) },
	{ 0xfde685c2, __VMLINUX_SYMBOL_STR(osal_timer_start) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0x3da4bb5f, __VMLINUX_SYMBOL_STR(rtk_trap_mgmtFrameLearningEnable_set) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x71db3805, __VMLINUX_SYMBOL_STR(rtk_vlan_port_del) },
	{ 0xb2ee8831, __VMLINUX_SYMBOL_STR(osal_timer_init) },
	{ 0xf78f6507, __VMLINUX_SYMBOL_STR(sys_errXlate_ret) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x92edee21, __VMLINUX_SYMBOL_STR(sys_dbg_pkt_dump) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x60a07cb5, __VMLINUX_SYMBOL_STR(rtk_stp_mstpInstance_create) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ski,rtk,osal,rtcore";

