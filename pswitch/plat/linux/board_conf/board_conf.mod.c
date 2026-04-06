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
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x8531b604, __VMLINUX_SYMBOL_STR(seq_open) },
	{ 0x74685627, __VMLINUX_SYMBOL_STR(drv_generalCtrlGPIO_pin_init) },
	{ 0x8102c3e2, __VMLINUX_SYMBOL_STR(drv_generalCtrlGPIO_dataBit_get) },
	{ 0xdc90b63, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xbb91651c, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xacb61993, __VMLINUX_SYMBOL_STR(drv_generalCtrlGPIO_dev_init) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xb5a4cc82, __VMLINUX_SYMBOL_STR(drv_generalCtrlGPIO_devEnable_set) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfbf35fb6, __VMLINUX_SYMBOL_STR(mtd_read) },
	{ 0x9ea702fb, __VMLINUX_SYMBOL_STR(hwp_myHwProfile) },
	{ 0x553cd825, __VMLINUX_SYMBOL_STR(seq_putc) },
	{ 0x9b4c9818, __VMLINUX_SYMBOL_STR(drv_extGpio_direction_get) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x400528b9, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x9bea595c, __VMLINUX_SYMBOL_STR(get_mtd_device) },
	{ 0xa7a3f4e9, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x6977b1eb, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0xb742fd7, __VMLINUX_SYMBOL_STR(simple_strtol) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x3d67c65c, __VMLINUX_SYMBOL_STR(seq_release) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rtcore";

