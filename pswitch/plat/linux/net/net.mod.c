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
	{ 0xa0ff6186, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xafb6d35d, __VMLINUX_SYMBOL_STR(drv_nic_init) },
	{ 0x6708145d, __VMLINUX_SYMBOL_STR(dev_change_flags) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xfdffa2ea, __VMLINUX_SYMBOL_STR(dev_get_flags) },
	{ 0xc7a4fbed, __VMLINUX_SYMBOL_STR(rtnl_lock) },
	{ 0x4189ee4d, __VMLINUX_SYMBOL_STR(drv_nic_rx_register) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xcac471b7, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0xde247a58, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x377104b7, __VMLINUX_SYMBOL_STR(netif_tx_wake_queue) },
	{ 0xdd557029, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x5b067df0, __VMLINUX_SYMBOL_STR(rtk_dev) },
	{ 0xec2da743, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0x97409243, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x15ee80be, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_irq) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x400528b9, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xd41e6082, __VMLINUX_SYMBOL_STR(__raw_spin_lock_init) },
	{ 0x3b4a7350, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x796e57e9, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xe33af449, __VMLINUX_SYMBOL_STR(drv_nic_rx_start) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0xb742fd7, __VMLINUX_SYMBOL_STR(simple_strtol) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x2c9689c, __VMLINUX_SYMBOL_STR(consume_skb) },
	{ 0x6e720ff2, __VMLINUX_SYMBOL_STR(rtnl_unlock) },
	{ 0xcad8229, __VMLINUX_SYMBOL_STR(alloc_etherdev_mqs) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rtcore";

