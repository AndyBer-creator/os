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
	{ 0xbd3ebb66, __VMLINUX_SYMBOL_STR(osal_time_usecs_get) },
	{ 0xe2eedf28, __VMLINUX_SYMBOL_STR(osal_free) },
	{ 0x27c9bcdf, __VMLINUX_SYMBOL_STR(drv_nic_diagPkt_send) },
	{ 0xd8b534b2, __VMLINUX_SYMBOL_STR(osal_sem_mutex_create) },
	{ 0x36d4b82d, __VMLINUX_SYMBOL_STR(log_initErrEn) },
	{ 0x36bbff2c, __VMLINUX_SYMBOL_STR(osal_sem_destroy) },
	{ 0x20769a3e, __VMLINUX_SYMBOL_STR(hwp_defaultProfilePort_update) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x80e2fd76, __VMLINUX_SYMBOL_STR(osal_sem_take) },
	{ 0x96731fda, __VMLINUX_SYMBOL_STR(ioal_mem32_field_read) },
	{ 0xa9e22ad6, __VMLINUX_SYMBOL_STR(drv_nic_txData_set) },
	{ 0xac9d1bf8, __VMLINUX_SYMBOL_STR(osal_isr_register) },
	{ 0xc2196715, __VMLINUX_SYMBOL_STR(ioal_mem32_read) },
	{ 0x330ee399, __VMLINUX_SYMBOL_STR(ioal_param_phyXfmrRj45Impd_get) },
	{ 0x5bf0b7fc, __VMLINUX_SYMBOL_STR(hwp_chipInfo_update) },
	{ 0x96b3eb7, __VMLINUX_SYMBOL_STR(unitMapStruct) },
	{ 0xf6907b59, __VMLINUX_SYMBOL_STR(osal_time_udelay) },
	{ 0x8071e23, __VMLINUX_SYMBOL_STR(hwp_portmaskValid_Chk) },
	{ 0x61c13ef6, __VMLINUX_SYMBOL_STR(osal_thread_create) },
	{ 0x43c1a8a2, __VMLINUX_SYMBOL_STR(osal_sem_give) },
	{ 0x640f65f5, __VMLINUX_SYMBOL_STR(hwp_defaultProfilePort_build) },
	{ 0x80c350b, __VMLINUX_SYMBOL_STR(unitInfoEmpty) },
	{ 0x2870e8cc, __VMLINUX_SYMBOL_STR(serdesDescpEmpty) },
	{ 0x8020b45d, __VMLINUX_SYMBOL_STR(log_initMsgEn) },
	{ 0x92234f65, __VMLINUX_SYMBOL_STR(rtk_unit2devID) },
	{ 0x8eabe493, __VMLINUX_SYMBOL_STR(rt_log) },
	{ 0xb217c45e, __VMLINUX_SYMBOL_STR(osal_sem_create) },
	{ 0x9ea702fb, __VMLINUX_SYMBOL_STR(hwp_myHwProfile) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x95d2e7e4, __VMLINUX_SYMBOL_STR(drv_swcore_chipFamilyId_get) },
	{ 0x315ce31f, __VMLINUX_SYMBOL_STR(drv_swcore_register_dump) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xbf8727a7, __VMLINUX_SYMBOL_STR(osal_time_usleep) },
	{ 0xadf75d0c, __VMLINUX_SYMBOL_STR(ioal_param_ledInitFlag_get) },
	{ 0xdaebe915, __VMLINUX_SYMBOL_STR(drv_nic_tag_set) },
	{ 0x2af4af1b, __VMLINUX_SYMBOL_STR(osal_alloc) },
	{ 0xfc3bc63e, __VMLINUX_SYMBOL_STR(osal_sem_mutex_give) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x478e5baa, __VMLINUX_SYMBOL_STR(hwp_parsedInfo_buildup) },
	{ 0xd8d9103f, __VMLINUX_SYMBOL_STR(hwp_useDefHwp) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x197aaa42, __VMLINUX_SYMBOL_STR(hwp_defaultProfilePhy_build) },
	{ 0xd604eba2, __VMLINUX_SYMBOL_STR(osal_wait_event) },
	{ 0xd8422a99, __VMLINUX_SYMBOL_STR(osal_wait_module_create) },
	{ 0xbd5bf996, __VMLINUX_SYMBOL_STR(hwp_profilePhy_update) },
	{ 0x1f03565a, __VMLINUX_SYMBOL_STR(osal_sem_mutex_take) },
	{ 0x5ce0f7c, __VMLINUX_SYMBOL_STR(rt_chip_familyIndex_get) },
	{ 0xa5573c12, __VMLINUX_SYMBOL_STR(drv_rtl8231_mdcSem_register) },
	{ 0x20eb391c, __VMLINUX_SYMBOL_STR(rtk_dev2unitID) },
	{ 0xfbc9842e, __VMLINUX_SYMBOL_STR(ioal_mem32_field_write) },
	{ 0x3e19c8bb, __VMLINUX_SYMBOL_STR(ioal_phy_reset) },
	{ 0xdcfd83c4, __VMLINUX_SYMBOL_STR(osal_time_mdelay) },
	{ 0x502a4a52, __VMLINUX_SYMBOL_STR(ioal_mem32_write) },
	{ 0x8db60739, __VMLINUX_SYMBOL_STR(osal_wake_up) },
	{ 0x49aeddf5, __VMLINUX_SYMBOL_STR(hwp_profilePhy_del) },
	{ 0xd32f3014, __VMLINUX_SYMBOL_STR(hwp_get_offset_by_baseport_port) },
	{ 0x1e6d26a8, __VMLINUX_SYMBOL_STR(strstr) },
	{ 0x2e87427c, __VMLINUX_SYMBOL_STR(ioal_param_sysMac_get) },
	{ 0x149edff3, __VMLINUX_SYMBOL_STR(osal_thread_exit) },
	{ 0x501ed11c, __VMLINUX_SYMBOL_STR(portDescpEmpty) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rtcore";

