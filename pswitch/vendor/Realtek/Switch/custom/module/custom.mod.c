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
	{ 0xdf901b4e, __VMLINUX_SYMBOL_STR(rsd_acl_rsv_ruleEnable_set) },
	{ 0xf74c288a, __VMLINUX_SYMBOL_STR(rsd_acl_rsv_rule_update) },
	{ 0xa106cf52, __VMLINUX_SYMBOL_STR(board_lldp_poe_register) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x26d7bfa7, __VMLINUX_SYMBOL_STR(ski_handler_register) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0x8fa76785, __VMLINUX_SYMBOL_STR(sys_logic_port_num_ret) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ski,rtcore";

