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
	{ 0xb6e55bd2, __VMLINUX_SYMBOL_STR(rsd_nic_isPortSTPForwad_ret) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0xdf901b4e, __VMLINUX_SYMBOL_STR(rsd_acl_rsv_ruleEnable_set) },
	{ 0x34184afe, __VMLINUX_SYMBOL_STR(current_kernel_time) },
	{ 0xbf2075cf, __VMLINUX_SYMBOL_STR(isg_bindingEntry_set) },
	{ 0xfff4def1, __VMLINUX_SYMBOL_STR(isg_bindingEntryDhcp_del) },
	{ 0x95af61d, __VMLINUX_SYMBOL_STR(rsd_nic_pktVid_get) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xde526d35, __VMLINUX_SYMBOL_STR(rsd_vlan_portOperVlanMembership_get) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0xea3fb524, __VMLINUX_SYMBOL_STR(osal_time_sleep) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xc5a13d16, __VMLINUX_SYMBOL_STR(isg_bindingEntry_del) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0xf5920578, __VMLINUX_SYMBOL_STR(sys_port_lPortmask2uStr) },
	{ 0x613fa751, __VMLINUX_SYMBOL_STR(sys_port_lport2uport_ret) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x16d741a1, __VMLINUX_SYMBOL_STR(rsd_port_errDisable_set) },
	{ 0xf78f6507, __VMLINUX_SYMBOL_STR(sys_errXlate_ret) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x42b29d8c, __VMLINUX_SYMBOL_STR(rtk_l2_addr_get) },
	{ 0xb624a21f, __VMLINUX_SYMBOL_STR(sys_trunk_portTrunkInfo_ret) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0xef17037, __VMLINUX_SYMBOL_STR(sys_util_mac2Str) },
	{ 0x9b588fad, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_alloc) },
	{ 0x5f96b4f7, __VMLINUX_SYMBOL_STR(rsd_nic_pktByDmacBlkPM_tx) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0xf9288f61, __VMLINUX_SYMBOL_STR(rsd_sys_macAddr_get) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x2eb8c2e8, __VMLINUX_SYMBOL_STR(sys_physic_to_logic_port) },
	{ 0x96e4d977, __VMLINUX_SYMBOL_STR(rsd_nic_pktByDmacBlkPort_tx) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x9806c5c4, __VMLINUX_SYMBOL_STR(isg_bindingEntry_exist) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ski,l2g_isg,rtcore,rtk";

