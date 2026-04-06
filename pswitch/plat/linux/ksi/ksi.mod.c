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
	{ 0x790a9ea8, __VMLINUX_SYMBOL_STR(sock_release) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x7f9620e5, __VMLINUX_SYMBOL_STR(netlink_unicast) },
	{ 0x7e4fa821, __VMLINUX_SYMBOL_STR(skb_pull) },
	{ 0x2aa8c61f, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x4abbe06f, __VMLINUX_SYMBOL_STR(__alloc_skb) },
	{ 0x141fdfc9, __VMLINUX_SYMBOL_STR(kfree_skb) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0x86ed1136, __VMLINUX_SYMBOL_STR(netlink_ack) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0x59aa4397, __VMLINUX_SYMBOL_STR(__netlink_kernel_create) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0xf2a9036e, __VMLINUX_SYMBOL_STR(__nlmsg_put) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rtcore";

