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

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xac8f33ba, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x3ce4ca6f, __VMLINUX_SYMBOL_STR(disable_irq) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x4239df7b, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xa0ff6186, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xb28f576d, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xb9178853, __VMLINUX_SYMBOL_STR(complete_and_exit) },
	{ 0xb698ea0c, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x750df5e7, __VMLINUX_SYMBOL_STR(mem_map) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0x2767aa8b, __VMLINUX_SYMBOL_STR(_dma_cache_wback_inv) },
	{ 0x18470f2f, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x95d2e7e4, __VMLINUX_SYMBOL_STR(drv_swcore_chipFamilyId_get) },
	{ 0x464cb435, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xb6091ec0, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0xdd62505d, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xdd557029, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x3a3311d6, __VMLINUX_SYMBOL_STR(down_trylock) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfbf35fb6, __VMLINUX_SYMBOL_STR(mtd_read) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0xb2a80f17, __VMLINUX_SYMBOL_STR(kill_pid) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xa24eda7f, __VMLINUX_SYMBOL_STR(drv_swcore_earlyInit_end) },
	{ 0xbef703a1, __VMLINUX_SYMBOL_STR(flush_signals) },
	{ 0xc94abe43, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xed2e432d, __VMLINUX_SYMBOL_STR(rtk_bspDev_irq_get) },
	{ 0x400528b9, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0xd41e6082, __VMLINUX_SYMBOL_STR(__raw_spin_lock_init) },
	{ 0xf4c357c6, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x366322b9, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x635a97ca, __VMLINUX_SYMBOL_STR(drv_swcore_ioalCB_register) },
	{ 0x796e57e9, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0xece7f290, __VMLINUX_SYMBOL_STR(get_mtd_device_nm) },
	{ 0x7afa89fc, __VMLINUX_SYMBOL_STR(vsnprintf) },
	{ 0x59237210, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xa81f865, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x2beb418e, __VMLINUX_SYMBOL_STR(might_fault) },
	{ 0xfcec0987, __VMLINUX_SYMBOL_STR(enable_irq) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x13804ad4, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x6df1aaf1, __VMLINUX_SYMBOL_STR(kernel_sigaction) },
	{ 0x6e13c32a, __VMLINUX_SYMBOL_STR(up) },
	{ 0xadcbea1f, __VMLINUX_SYMBOL_STR(set_user_nice) },
	{ 0xbb832cfb, __VMLINUX_SYMBOL_STR(drv_swcore_cid_get) },
	{ 0x87280639, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x5e515be6, __VMLINUX_SYMBOL_STR(ktime_get_ts64) },
	{ 0x9e7d6bd0, __VMLINUX_SYMBOL_STR(__udelay) },
	{ 0x760a0f4f, __VMLINUX_SYMBOL_STR(yield) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

