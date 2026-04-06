include version.mk

CONFIG_VENDOR_DEFAULT := Realtek
OS_TYPE := uClinux
KERNEL_DIR := kernel
LOADER_DIR := loader/u-boot-2011.12
SDK_DIR := sdk
PSWITCH_DIR := pswitch
OEM_DIR := oemconfig
BIN_DIR := bin
DIFF_LOG := $(OS_TYPE)_diff.log
ABS_ROOT_DIR := $(shell pwd)

ifeq ($(KERNEL_DIR)/$(OS_TYPE)/.config,$(wildcard $(KERNEL_DIR)/$(OS_TYPE)/.config))
-include $(KERNEL_DIR)/$(OS_TYPE)/.config
endif

.PHONY : all image loader pswitch sdk both menuconfig clean loaderclean distclean

sinclude $(OEM_DIR)/defConf.mk

all:
	make -C $(KERNEL_DIR)/$(OS_TYPE) common
	make pswitch
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage

pswitch:
	make -C $(PSWITCH_DIR)

image:
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage

loader:
	make -C $(LOADER_DIR)

final: loader all
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_linux

menuconfig:
	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
	    ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi
	make -C $(KERNEL_DIR)/$(OS_TYPE) menuconfig
	make -C $(PSWITCH_DIR) setup

config:
	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
            ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi
	make -C $(KERNEL_DIR)/$(OS_TYPE) config
	make -C $(PSWITCH_DIR) setup


clean:
	make -C $(PSWITCH_DIR) clean
	make -C $(KERNEL_DIR)/$(OS_TYPE) clean
	rm -rf image_yfd_*/

loaderclean:
	make -C $(LOADER_DIR) distclean
	rm -rf $(LOADER_DIR)/arch/otto/include/asm/arch-rtl839x/cross_env.h
	rm -rf $(LOADER_DIR)/arch/otto/include/asm/arch-rtl839x/reg_map_util.h
	rm -rf $(LOADER_DIR)/board/Realtek/include/rtk_flash_common.h
	rm -rf $(LOADER_DIR)/.boards.depend

webenglishonly:
	rm -rf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org/*
	cp -rf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org_double/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org/
	cp -rf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org_en/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org/

webnormal:
	rm -rf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org/*
	cp -rf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org_double/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/webui/web_org/

normalclean:
	make -C $(PSWITCH_DIR) distclean
	make -C $(KERNEL_DIR)/$(OS_TYPE) distclean

distclean: normalclean loaderclean kernelclean

kernelclean:
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/lib/uClibc-0.9.33-485-200
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/.config
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/.config.old
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/extra/config/conf
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/extra/config/zconf.hash.c
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/extra/config/zconf.lex.c
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/extra/config/zconf.tab.c
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/include/config
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/uClibc-0.9.33-485-200/include/generated
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.config
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.config.old
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.missing-syscalls.d
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.tmp_System.map
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.tmp_versions
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.tmp_vmlinux1
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.tmp_vmlinux2
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/.version
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/Module.symvers
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/System.map
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/generated/asm-offsets.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/generated/bounds.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/auto.conf
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/blk/dev/loop
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/blk/dev/loop.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/blk/dev/ram
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/blk/dev/ram.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/cpu/mips4k.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/inline
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/jffs2
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/kernel.release
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/misc
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/mtd
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/mtd.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/preempt/voluntary.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/rtl9310
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/soc/cpu/mips4k.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/soc/support/mips4k.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/squashfs
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/squashfs.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/strip
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/tristate.conf
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/xz
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/config/zlib/deflate.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/generated/autoconf.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/generated/compile.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/include/generated/utsrelease.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/kernel/time/hz.bc
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/kernel/time/timeconst.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/lib/crc32table.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/lib/gen_crc32table
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/basic/fixdep
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/genksyms/genksyms
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/kallsyms
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/kconfig/conf
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/mod/devicetable-offsets.s
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/mod/mk_elfconfig
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/mod/modpost
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/scripts/sortextable
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/usr/.initramfs_data.cpio.d
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/usr/gen_init_cpio
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/usr/initramfs_data.cpio.gz
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/linux-3.18.24.x/vmlinux
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/.config
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/.config.old
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/applets/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/applets/applet_tables
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/applets/usage
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/applets/usage_pod
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/archival/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/archival/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/archival/libarchive/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/console-tools/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/console-tools/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/coreutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/coreutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/coreutils/libcoreutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/debianutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/debianutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/docs/BusyBox.html
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/docs/BusyBox.txt
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/docs/busybox.1
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/docs/busybox.net
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/docs/busybox.pod
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/old_e2fsprogs/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/old_e2fsprogs/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/old_e2fsprogs/blkid/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/old_e2fsprogs/e2p/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/old_e2fsprogs/ext2fs/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/e2fsprogs/old_e2fsprogs/uuid/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/editors/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/editors/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/findutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/findutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/NUM_APPLETS.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/applet_tables.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/applets.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/autoconf.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/bbconfigopts.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/bbconfigopts_bz2.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/config
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/usage.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/include/usage_compressed.h
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/init/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/init/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/libbb/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/libbb/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/libpwdgrp/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/loginutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/loginutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/mailutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/mailutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/miscutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/miscutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/modutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/modutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/networking/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/networking/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/networking/libiproute/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/networking/udhcp/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/networking/udhcp/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/printutils/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/printutils/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/procps/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/procps/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/runit/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/runit/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/basic/docproc
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/basic/fixdep
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/basic/split-include
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/kconfig/conf
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/kconfig/lex.zconf.c
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/kconfig/zconf.hash.c
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/scripts/kconfig/zconf.tab.c
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/selinux/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/selinux/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/shell/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/shell/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/sysklogd/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/sysklogd/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/util-linux/Config.in
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/util-linux/Kbuild
	rm -rf $(KERNEL_DIR)/$(OS_TYPE)/user/busybox-1.20.2/util-linux/volume_id/Kbuild

target:
	@for i in $(dir) ; do \
		if [ ! -d $$i ]; then                       \
			echo "****** Directory $$i is not exist"; \
			exit 1; \
		else                                            \
			echo "****** Build $$i"; \
			cd $(dir) && make && make romfs && cd - ;\
		fi; \
	done
	make image;
