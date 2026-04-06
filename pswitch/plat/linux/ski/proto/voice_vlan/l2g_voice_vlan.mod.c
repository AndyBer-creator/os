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
	{ 0x825af07d, __VMLINUX_SYMBOL_STR(defdb_val_ret) },
	{ 0x2a86b345, __VMLINUX_SYMBOL_STR(rsd_vlan_voice_chip_cosSrcEntry_set) },
	{ 0x5b757faf, __VMLINUX_SYMBOL_STR(rsd_nic_handler_unregister) },
	{ 0xb46de5c4, __VMLINUX_SYMBOL_STR(rsd_vlan_portAdminMode_get) },
	{ 0x58cc3325, __VMLINUX_SYMBOL_STR(sys_port_lPort2uStr) },
	{ 0x6db2ef23, __VMLINUX_SYMBOL_STR(rsd_nic_pkt_free) },
	{ 0xa1ec4b71, __VMLINUX_SYMBOL_STR(rsd_vlan_voice_entry_del) },
	{ 0x95af61d, __VMLINUX_SYMBOL_STR(rsd_nic_pktVid_get) },
	{ 0x2e4e5247, __VMLINUX_SYMBOL_STR(sys_util_bitmaskIsClear_ret) },
	{ 0x149edff3, __VMLINUX_SYMBOL_STR(osal_thread_exit) },
	{ 0x16c9050, __VMLINUX_SYMBOL_STR(rsd_acl_cb_register) },
	{ 0xec63ee9b, __VMLINUX_SYMBOL_STR(rsd_vlan_voice_chip_cosAllEntry_set) },
	{ 0x57084b30, __VMLINUX_SYMBOL_STR(rsd_qos_cosQueue_get) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xe07a1dc2, __VMLINUX_SYMBOL_STR(sys_notifier_observer_register) },
	{ 0x6f5cecac, __VMLINUX_SYMBOL_STR(osal_timer_stop) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x377f543, __VMLINUX_SYMBOL_STR(sys_dbg) },
	{ 0x81612727, __VMLINUX_SYMBOL_STR(rsd_vlan_defaultVid_get) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0xfde685c2, __VMLINUX_SYMBOL_STR(osal_timer_start) },
	{ 0xe37fd484, __VMLINUX_SYMBOL_STR(sys_notifier_observer_unregister) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0xb2ee8831, __VMLINUX_SYMBOL_STR(osal_timer_init) },
	{ 0xc9ff5966, __VMLINUX_SYMBOL_STR(sys_port_lPortStr_ret) },
	{ 0x7bc30378, __VMLINUX_SYMBOL_STR(rsd_nic_handler_register) },
	{ 0x67932d45, __VMLINUX_SYMBOL_STR(rsd_vlan_portVoiceVlanActive_set) },
	{ 0x26f3a9ff, __VMLINUX_SYMBOL_STR(rsd_vlan_voiceVid_get) },
	{ 0x6fa9f3b, __VMLINUX_SYMBOL_STR(rsd_vlan_voice_chip_cosSrcEntry_del) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0x95319f1b, __VMLINUX_SYMBOL_STR(sys_log) },
	{ 0xef17037, __VMLINUX_SYMBOL_STR(sys_util_mac2Str) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x15eef2ba, __VMLINUX_SYMBOL_STR(osal_sem_mutex_destroy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xaf4a52f1, __VMLINUX_SYMBOL_STR(sys_trunk_portRole_ret) },
	{ 0x82ac09c5, __VMLINUX_SYMBOL_STR(rsd_vlan_voice_chip_cosAllEntry_del) },
	{ 0x48f4cffb, __VMLINUX_SYMBOL_STR(rsd_vlan_voice_chip_mbrEntry_set) },
	{ 0x4373a279, __VMLINUX_SYMBOL_STR(rsd_vlan_portVoiceVlanActive_get) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ski,defdb,rtcore,osal";

