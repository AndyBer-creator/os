############################################################################
#
# Makefile -- Top level uClinux makefile.
#
# Copyright (c) 2001-2004, SnapGear (www.snapgear.com)
# Copyright (c) 2001, Lineo
#

VERSIONPKG = 3.2.0
VERSIONSTR = $(CONFIG_VENDOR)/$(CONFIG_PRODUCT) Version $(VERSIONPKG)

############################################################################
#
# Lets work out what the user wants, and if they have configured us yet
#

ifeq (.config,$(wildcard .config))
include .config
ifeq ($(CONFIG_LINUXDIR)/.config,$(wildcard $(CONFIG_LINUXDIR)/.config))
include $(CONFIG_LINUXDIR)/.config
endif
#ifeq ($(SDK_CONFIG), $(wildcard $(SDK_PATH_CONFIG)))
#include $(SDK_CONFIG)
#endif

# DON'T have to create a image contains kernel image(image.ram) and
#file system(romfs.img). We use initramfs instead, reference linux-2.6.x\Documentation\early-userspace\README
#for more detail.
#all: ucfront cksum subdirs romfs image
all: common post_setup post_mkimage
ni: common
else
all: config_error
endif

PRE_SETUP_TARGETS:=

ifeq ("$(wildcard ../../pswitch)","")
ifneq (.config,$(wildcard .config))
CONFIG_SYS_CODEBASE = SDK
endif
endif

#ifneq ($(CONFIG_SYS_CODEBASE), SDK)
#  PRE_SETUP_TARGETS += target.sys.run.setup
#endif


pre_setup: $(PRE_SETUP_TARGETS)


common: pre_setup ucfront cksum subdirs romfs
devmod: romfs modules modules_install post_setup post_mkimage
final: common post_linux


#CNSD6-nickwu-20070926: linux-2.6.x/vmlinux is generated before romfs is prepared for the first time.
#so force to generate linux-2.6.x/vmlinux again to contain the correct initramfs_data.cpio.gz.
post_setup:
	@rm -f $(LINUXDIR)/usr/initramfs_data.cpio.gz
	@if [ -e $(SQFSDIR) ]; then \
	if egrep "^CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS=y" $(PSWITCH_DIR)/config/.config > /dev/null; then \
		rm -f $(ROMFSDIR)/sqfs.img;\
		rm -f $(ROMFSDIR)/modsqfs.img;\
		if egrep "^CONFIG_LINUXDIR=linux-3.18.24.x" $(ROOTDIR)/.config > /dev/null; then \
			mksquashfs_xz $(SQFSDIR) $(ROMFSDIR)/sqfs.img -noappend -no-fragments -comp xz; \
			mksquashfs_xz $(MODSQFSDIR) $(ROMFSDIR)/modsqfs.img -noappend -no-fragments -comp xz; \
		else \
			mksquashfs_lzma $(SQFSDIR) $(ROMFSDIR)/sqfs.img -be -noappend -no-fragments; \
			mksquashfs_lzma $(MODSQFSDIR) $(ROMFSDIR)/modsqfs.img -be -noappend -no-fragments; \
		fi \
	fi \
	fi
	$(MAKEARCH_KERNEL) -j$(HOST_NCPU) -C $(LINUXDIR) $(LINUXTARGET) || exit 1
	@rm -f $(ROOTDIR)/vmlinux.bin $(ROOTDIR)/vmlinux.bix $(IMAGEDIR)/vmlinux.bin $(IMAGEDIR)/vmlinux.bix
	@rm -f $(IMAGEDIR)/jffs2.bin $(IMAGEDIR)/cramfs.bin $(IMAGEDIR)/squashfs.bin $(IMAGEDIR)/squashfs_lzma.bin
	@cp -f $(LINUXDIR)/vmlinux $(IMAGEDIR)/vmlinux
	$(CROSS_COMPILE)objcopy -O binary -R .note -R .comment -S $(LINUXDIR)/vmlinux $(IMAGEDIR)/vmlinux.bin

rtKernel:
	$(MAKEARCH_KERNEL) -j$(HOST_NCPU) -C $(LINUXDIR) $(LINUXTARGET) || exit 1
	@rm -f $(ROOTDIR)/vmlinux.bin $(ROOTDIR)/vmlinux.bix $(IMAGEDIR)/vmlinux.bin $(IMAGEDIR)/vmlinux.bix
	@cp -f $(LINUXDIR)/vmlinux $(IMAGEDIR)/vmlinux
	$(CROSS_COMPILE)objcopy -O binary -R .note -R .comment -S $(LINUXDIR)/vmlinux $(IMAGEDIR)/vmlinux.bin

post_mkimage:
	@cp -f $(IMAGEDIR)/vmlinux.bin $(IMAGEDIR)/vmlinux_org.bin
	@lzma -9 $(IMAGEDIR)/vmlinux_org.bin
ifeq ($(CONFIG_SYS_CODEBASE), PSWITCH)
	@if [ -e $(ADDIMGINFO) ]; then \
		$(ADDIMGINFO) $(IMGINFODIR) $(IMAGEDIR)/vmlinux_org.bin.lzma $(PACKAGE_VERSION);\
	fi
endif
	if egrep "^CONFIG_SYS_RTL9310=y" $(PSWITCH_DIR)/config/.config > /dev/null; then \
		$(MKIMAGE_9310) -A mips -O linux -T kernel -C lzma -a 0x80000000 -e 0x$(shell sed -n '/T kernel_entry/s/ T kernel_entry//p' $(LINUXDIR)/System.map | sed -e 's/ffffffff//') -n "$(PACKAGE_VERSION)" -d $(IMAGEDIR)/vmlinux_org.bin.lzma $(IMAGEDIR)/vmlinux.bix; \
	fi
	if egrep "^CONFIG_SYS_RTL9300=y" $(PSWITCH_DIR)/config/.config > /dev/null; then \
		$(MKIMAGE_9300) -A mips -O linux -T kernel -C lzma -a 0x80000000 -e 0x$(shell sed -n '/T kernel_entry/s/ T kernel_entry//p' $(LINUXDIR)/System.map | sed -e 's/ffffffff//') -n "$(PACKAGE_VERSION)" -d $(IMAGEDIR)/vmlinux_org.bin.lzma $(IMAGEDIR)/vmlinux.bix; \
	fi
	if egrep "^CONFIG_SYS_RTL8390=y" $(PSWITCH_DIR)/config/.config > /dev/null; then \
		$(MKIMAGE_8380) -A mips -O linux -T kernel -C lzma -a 0x80000000 -e 0x$(shell sed -n '/T kernel_entry/s/ T kernel_entry//p' $(LINUXDIR)/System.map | sed -e 's/ffffffff//') -n "$(PACKAGE_VERSION)" -d $(IMAGEDIR)/vmlinux_org.bin.lzma $(IMAGEDIR)/vmlinux.bix; \
	fi
	if egrep "^CONFIG_SYS_RTL8380=y" $(PSWITCH_DIR)/config/.config > /dev/null; then \
		$(MKIMAGE_8380) -A mips -O linux -T kernel -C lzma -a 0x80000000 -e 0x$(shell sed -n '/T kernel_entry/s/ T kernel_entry//p' $(LINUXDIR)/System.map | sed -e 's/ffffffff//') -n "$(PACKAGE_VERSION)" -d $(IMAGEDIR)/vmlinux_org.bin.lzma $(IMAGEDIR)/vmlinux.bix; \
	fi
	@rm -f $(IMAGEDIR)/vmlinux_org.bin.lzma
	@cp $(IMAGEDIR)/vmlinux* $(IMAGEDIR2)

post_fs:
	@mkdir -p $(ROOTDIR)/emp_tmp/log
	#@mkfs.jffs2 --eraseblock=64 -b --pad=0x200000 -d $(ROOTDIR)/emp_tmp -o $(IMAGEDIR)/jffs2.bin
	@rm -rf $(ROOTDIR)/emp_tmp

	@if egrep "^CONFIG_CRAMFS=y" $(LINUX_CONFIG) > /dev/null; then \
		mkcramfs $(ROMFSDIR) $(IMAGEDIR)/cramfs.bin; \
	fi
	@if egrep "^CONFIG_SQUASHFS=y" $(LINUX_CONFIG) > /dev/null; then \
		mksquashfs $(ROMFSDIR) $(IMAGEDIR)/squashfs.bin -be -noappend -no-fragments; \
	fi
	@if egrep "^CONFIG_SQUASHFS_LZMA=y" $(LINUX_CONFIG) > /dev/null; then \
		mksquashfs_lzma $(ROMFSDIR) $(IMAGEDIR)/squashfs_lzma.bin -be -noappend -no-fragments; \
	fi

#############################################################################
#Type 5 16MB Flash, cannot support 16MB dual image in 3.5.0. becasue not enough image space.
#Type 5 only support dual image
#Loader     0xD FFFF (896KB) 
#ENV        0xE FFFF (64KB) 
#SYS        0xF FFFF (64KB, 0xF0000) 
#JFFS2 CFG  0x1F FFFF (1024KB, 0x100000) 
#JFFS2 LOG  0x2F FFFF (1024KB, 0x100000) 
#Image1     0x97 FFFF (6656KB, 0x680000) 
#Image2     0xFF FFFF (6656KB, 0x680000) 
#Type 5 32MB Flash
#Loader     0xD FFFF (896KB) 
#ENV        0xE FFFF (64KB) 
#SYS        0xF FFFF (64KB, 0xF0000) 
#JFFS2 CFG  0x1F FFFF (1024KB, 0x100000) 
#JFFS2 LOG  0x2F FFFF (1024KB, 0x100000) 
#Image1     0x117 FFFF (14848KB, 0x1150000) 
#Image2     0x1FF FFFF (14848KB, 0x1150000)  
post_linux: post_setup post_mkimage post_fs
	#16MB T5 dual image
	if [ -e $(IMAGEDIR2)/u-boot.bin ]; then \
		$(ROOTDIR)/tools/packImage $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/u-boot.bin 0x300000 $(IMAGEDIR)/vmlinux.bix 0xd80000 > /dev/null; \
		if [ -e $(IMAGEDIR)/rom.bix ]; then \
			mv $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/rom-T516MB.bix; \
		fi; \
	fi; \

	#32MB T5 dual image
	if egrep "^CONFIG_DUAL_IMAGE=y" $(SDK_CONFIG) > /dev/null; then \
		EXTRA="$(IMAGEDIR)/vmlinux.bix 0xd80000"; \
		if [ -e $(IMAGEDIR2)/u-boot.bin ]; then \
			$(ROOTDIR)/tools/packImage $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/u-boot.bin 0x300000 $(IMAGEDIR)/vmlinux.bix 0xe80000 $$EXTRA > /dev/null; \
		mv $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/rom-T532MB.bix > /dev/null; \
		fi; \
	fi; \

post_linux_u:
	#16MB T5 dual image
	if [ -e $(IMAGEDIR2)/u-boot.bin ]; then \
		$(ROOTDIR)/tools/packImage $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/u-boot.bin 0x300000 $(IMAGEDIR)/vmlinux.bix 0xd80000 > /dev/null; \
		if [ -e $(IMAGEDIR)/rom.bix ]; then \
			mv $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/rom-T516MB.bix; \
		fi; \
	fi; \

	#32MB T5 dual image
	if egrep "^CONFIG_DUAL_IMAGE=y" $(SDK_CONFIG) > /dev/null; then \
		EXTRA="$(IMAGEDIR)/vmlinux.bix 0xd80000"; \
		if [ -e $(IMAGEDIR2)/u-boot.bin ]; then \
			$(ROOTDIR)/tools/packImage $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/u-boot.bin 0x300000 $(IMAGEDIR)/vmlinux.bix 0xe80000 $$EXTRA > /dev/null; \
		mv $(IMAGEDIR)/rom.bix $(IMAGEDIR2)/rom-T532MB.bix > /dev/null; \
		fi; \
	fi; \



############################################################################
#
# Get the core stuff worked out
#

LINUXDIR = $(CONFIG_LINUXDIR)
LIBCDIR  = $(CONFIG_LIBCDIR)
ROOTDIR  = $(shell pwd)
TOPDIR   := $(patsubst %/,%,$(dir $(patsubst %/,%,$(dir $(shell pwd)))))
PATH	 := $(PATH):$(ROOTDIR)/tools
HOSTCC   = cc

IMAGEDIR        = $(ROOTDIR)/images
IMAGEDIR2       = $(ROOTDIR)/../../image
ifeq ($(CONFIG_LOADER_UBOOT), y)
LOADER_PATH     = $(ROOTDIR)/u-boot
else
LOADER_PATH     = $(ROOTDIR)/u-boot-2011.12
endif
LOADERDIR       = $(ROOTDIR)/../../loader/$(CONFIG_LOADERDIR)
SDKDIR          = $(ROOTDIR)/../../sdk
PSWITCH_DIR = $(ROOTDIR)/../../pswitch
PSWITCH_VENDORDIR = $(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)
MODELDIR        = $(ROOTDIR)/../../model

MKIMAGE_8380    = $(ROOTDIR)/tools/mkimage_8380
MKIMAGE_9300    = $(ROOTDIR)/tools/mkimage_9300
MKIMAGE_9310    = $(ROOTDIR)/tools/mkimage_9310

IMGINFODIR = $(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/custom/imginfo
ADDIMGINFO = $(IMGINFODIR)/addimginfo.sh
RELDIR     = $(ROOTDIR)/release
ROMFSDIR   = $(ROOTDIR)/romfs
ROMFSINST  = romfs-inst.sh
EMPFSDIR   = $(ROOTDIR)/empfs
SCRIPTSDIR = $(ROOTDIR)/config/scripts
TFTPDIR    = /tftpboot
SQFSDIR    = $(ROOTDIR)/sqfs
SQFSINST   = sqfs-inst.sh
SQFSROMDIR = /sqfs
MODSQFSDIR = $(ROOTDIR)/modsqfs
MODSQFSROMDIR = /modsqfs

SYSROMFSDIR = $(ROMFSDIR)

BUILD_START_STRING ?= $(shell date "+%a, %d %b %Y %T %z")
ifndef NON_SMP_BUILD
HOST_NCPU := $(shell if [ -f /proc/cpuinfo ]; then n=`grep -c processor /proc/cpuinfo`; if [ $$n -gt 1 ];then expr $$n \* 2; else echo $$n; fi; else echo 1; fi)
else
HOST_NCPU := 1
endif

ifneq ($(CONFIG_SYS_CODEBASE), SDK)
PSWITCH_PATH_CONFIG    = $(PSWITCH_DIR)/make.path.mk
PSWITCH_INSTALL_CONFIG = $(PSWITCH_DIR)/make.install.mk
PSWITCH_CONFIG_CONFIG  = $(PSWITCH_DIR)/make.config.mk

ifeq ($(PSWITCH_PATH_CONFIG), $(wildcard $(PSWITCH_PATH_CONFIG)))
include $(PSWITCH_PATH_CONFIG)
endif
ifeq ($(PSWITCH_INSTALL_CONFIG), $(wildcard $(PSWITCH_INSTALL_CONFIG)))
include $(PSWITCH_INSTALL_CONFIG)
endif
ifeq ($(PSWITCH_CONFIG_CONFIG), $(wildcard $(PSWITCH_CONFIG_CONFIG)))
include $(PSWITCH_CONFIG_CONFIG)
endif
else
SDK_INSTALL_CONFIG = $(SDKDIR)/make.install.mk
ifeq ($(SDK_INSTALL_CONFIG), $(wildcard $(SDK_INSTALL_CONFIG)))
include $(SDK_INSTALL_CONFIG)
endif
endif

LINUX_CONFIG   = $(ROOTDIR)/$(LINUXDIR)/.config
CONFIG_CONFIG  = $(ROOTDIR)/config/.config
MODULES_CONFIG = $(ROOTDIR)/modules/.config
LOADER_CONFIG  = $(LOADERDIR)/.config
# ifeq ($(CONFIG_LOADER_RTKPLR), y)
# PRELOADER_CONFIG  = $(LOADER_PATH)/plr/release/soc.tcl
# endif
SDK_CONFIG     = $(SDKCFGDIR)/.config
SDK_AUTOCONF   = $(SDKSYSINC)/common/sdk_autoconf.h

MACHARCH      = $(LINUXDIR)/arch/mips/realtek
MACHINC       = $(LINUXDIR)/include/asm-mips/mach-realtek
MACHINCLUDE   = $(ROOTDIR)/$(MACHINC)

FLASH_SRC     = $(LINUXDIR)/drivers/mtd/maps
FLASH_INC     = $(LINUXDIR)/include/linux/mtd
FLASH_SRCDIR  = $(DRV_INCLUDE)/$(FLASH_SRC)
FLASH_INCDIR  = $(DRV_INCLUDE)/$(FLASH_INC)

USBHCD_SRC    = $(LINUXDIR)/drivers/usb/host
USBHCD_INC    = $(LINUXDIR)/include/linux/usb
USBHCD_SRCDIR = $(DRV_INCLUDE)/$(USBHCD_SRC)
USBHCD_INCDIR = $(DRV_INCLUDE)/$(USBHCD_INC)

SDKCFGDIR     = $(SDKDIR)/config
SDKINCCOM     = $(SDKDIR)/include/common
SDKSYSINC     = $(SDKDIR)/system/include

SDK_BUILD     = $(SDKDIR)/build
SDK_LIBDIR    = $(SDKDIR)/lib
SDK_DRVDIR    = $(ROOTDIR)/$(LINUXDIR)/drivers/net/switch
SDK_RTCOREDIR = $(SDK_DRVDIR)/rtcore
SDK_RTKDIR    = $(SDK_DRVDIR)/rtk
SDK_ETHDIR    = $(SDK_DRVDIR)/rtnic
SDK_NETDIR    = $(ROOTDIR)/$(LINUXDIR)/net/switch
SDK_USRDIR    = $(ROOTDIR)/user/switch/sdk
ifeq ($(CONFIG_LINUXDIR), linux-3.18.24.x)
SDK_SELDIR    = $(SDKDIR)/system/linux/linux-3.18.24.x/arch/mips/realtek
endif
SDK_INCLUDE   = $(SDKDIR)/include
DRV_INCLUDE   = $(SDKDIR)/system/linux
SYS_INCLUDE   = $(SDKDIR)/system/include
UT_INCLUDE    = $(SDKDIR)/unittest
APP_LIB_INCLUDE = $(ROOTDIR)/user/switch/realtek
APP_DRV_INCLUDE = $(ROOTDIR)/$(LINUXDIR)/net/switch/realtek

DST_ROOT_MODDIR = $(ROMFSDIR)/lib/modules/2.6.19/kernel
SDK_SRC_MODDIR = $(ROOTDIR)/$(LINUXDIR)/drivers/net/switch/sdk
SDK_DST_MODDIR = $(DST_ROOT_MODDIR)/drivers/net/switch/sdk
PROTO_SRC_MODDIR = $(ROOTDIR)/$(LINUXDIR)/net/switch/sdk
PROTO_DST_MODDIR = $(DST_ROOT_MODDIR)/net/switch/sdk

CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	  else if [ -x /bin/bash ]; then echo /bin/bash; \
	  else echo sh; fi ; fi)

ifeq (config.arch,$(wildcard config.arch))
ifeq ($(filter %_default, $(MAKECMDGOALS)),)
include config.arch
ARCH_CONFIG = $(ROOTDIR)/config.arch
export ARCH_CONFIG
endif
endif

# May use a different compiler for the kernel
KERNEL_CROSS_COMPILE ?= $(CROSS_COMPILE)
ifneq ($(SUBARCH),)
# Using UML, so make the kernel and non-kernel with different ARCHs
MAKEARCH = $(MAKE) ARCH=$(SUBARCH)
MAKEARCH_KERNEL = $(MAKE) ARCH=$(ARCH) SUBARCH=$(SUBARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE)
else
MAKEARCH = $(MAKE) ARCH=$(ARCH)
MAKEARCH_KERNEL = $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE)
endif

#ifeq ($(SDK_CONFIG),$(wildcard $(SDK_CONFIG)))
#include $(SDK_CONFIG)
#endif

DIRS    = $(VENDOR_TOPDIRS) include lib include user

export VENDOR PRODUCT ROOTDIR LINUXDIR LOADERDIR HOSTCC CONFIG_SHELL
export CONFIG_CONFIG LINUX_CONFIG LOADER_CONFIG MODULES_CONFIG ROMFSDIR SCRIPTSDIR
export VERSIONPKG VERSIONSTR ROMFSINST PATH IMAGEDIR RELDIR RELFILES TFTPDIR
export SDK_CONFIG SDK_LIBDIR SDK_BUILD
export BUILD_START_STRING HOST_NCPU PSWITCH_DIR

.PHONY: ucfront
ucfront: tools/ucfront/*.c
	$(MAKE) -C tools/ucfront
	ln -sf tools/ucfront/ucfront tools/ucfront-gcc
	ln -sf tools/ucfront/ucfront tools/ucfront-g++
	ln -sf tools/ucfront/ucfront-ld tools/ucfront-ld
ifeq ($(CONFIG_LIBCDIR), uClibc-0.9.28)
	@echo "uClibc is 0.9.28!!!"
endif

.PHONY: cksum
cksum: tools/cksum
tools/cksum: tools/sg-cksum/*.c
	$(MAKE) -C tools/sg-cksum
	ln -sf tools/sg-cksum/cksum tools/cksum

.PHONY: des
des: tools/des
tools/des: tools/src-des/*.c
	$(MAKE) -C tools/src-des
	ln -sf $(ROOTDIR)/tools/src-des/des tools/des

.PHONY: loader
loader:
	@if [ ! -e $(MKIMAGE) ]; then \
		$(MAKE) -C $(LOADER_PATH); \
		cp $(LOADER_PATH)/tools/mkimage $(ROOTDIR)/tools/; \
	fi

sdk.bsp.setup:
	@if [ ! -e $(MACHARCH) ]; then \
		ln -s $(SDKDIR)/system/linux/$(MACHARCH) $(MACHARCH); \
	fi
	@if [ ! -e $(MACHINC) ]; then \
		ln -s $(SDKDIR)/system/linux/$(MACHINC) $(MACHINC); \
	fi

.PHONY: sdk.setup sdk.cleanup dist.setup
sdk.setup: flash.setup sdk.bsp.setup usbhcd.setup
	@if [ ! -e $(SDK_LIBDIR) ]; then \
		mkdir -p $(SDK_LIBDIR); \
	fi
	@if [ ! -e $(SDK_DRVDIR)/Makefile ]; then \
		ln -s $(SDK_BUILD)/Makefile.linux.drivers $(SDK_DRVDIR)/Makefile; \
	fi
	@if [ ! -e $(SDK_RTCOREDIR) ]; then \
		ln -s $(SDKDIR)/system/linux/rtcore $(SDK_RTCOREDIR); \
		ln -sf $(SDKDIR)/build/Makefile.rtcore $(SDK_RTCOREDIR)/Makefile.rtcore; \
	fi
	@if [ ! -e $(SDK_RTKDIR) ]; then \
		ln -s $(SDKDIR)/src $(SDK_RTKDIR); \
	fi
	@if [ ! -e $(SDK_ETHDIR) ]; then \
		ln -s $(SDKDIR)/system/linux/rtnic $(SDK_ETHDIR); \
	fi
	@if [ ! -e $(SDK_DRVDIR)/unittest ]; then \
		if [ -e $(SDKDIR)/unittest ]; then \
			ln -s $(SDKDIR)/unittest $(SDK_DRVDIR)/unittest; \
		fi \
	fi
	@if [ ! -e $(SDK_NETDIR)/Makefile ]; then \
		ln -s $(SDK_BUILD)/Makefile.linux.net $(SDK_NETDIR)/Makefile; \
	fi
	@if [ ! -e $(SDK_NETDIR)/rtdrv ]; then \
		ln -s $(SDKDIR)/system/linux/rtdrv $(SDK_NETDIR)/rtdrv; \
	fi
	@if [ ! -e $(SDK_USRDIR) ]; then \
		mkdir -p $(SDK_USRDIR); \
	fi
	@if [ ! -e $(SDK_USRDIR)/Makefile ]; then \
		ln -s $(SDK_BUILD)/Makefile.linux.user $(SDK_USRDIR)/Makefile; \
	fi
	@if [ ! -e $(SDK_USRDIR)/diag ]; then \
		if egrep "^CONFIG_SDK_APP_DIAG_DIR_DIAG=y" $(SDK_CONFIG) > /dev/null; then \
			ln -s $(SDKDIR)/src/app/diag $(SDK_USRDIR)/diag; \
		else \
			if egrep "^CONFIG_SDK_APP_DIAG_DIR_DIAG_V2=y" $(SDK_CONFIG) > /dev/null; then \
				ln -s $(SDKDIR)/src/app/diag_v2 $(SDK_USRDIR)/diag; \
			fi \
		fi \
	fi
	@if [ ! -e $(SDK_USRDIR)/rtk ]; then \
		ln -s $(SDKDIR)/src $(SDK_USRDIR)/rtk; \
	fi
	@if [ ! -e $(SDK_USRDIR)/rtnic ]; then \
		ln -s $(SDKDIR)/system/drv/nic $(SDK_USRDIR)/rtnic; \
	fi
	@if [ ! -e $(SDK_USRDIR)/l2ntfy ]; then \
		ln -s $(SDKDIR)/system/drv/l2ntfy $(SDK_USRDIR)/l2ntfy; \
	fi
	@if [ ! -e $(SDK_USRDIR)/rtcore ]; then \
		ln -s $(SDKDIR)/system $(SDK_USRDIR)/rtcore; \
	fi
	@if [ ! -e $(SDK_USRDIR)/rtusr ]; then \
		ln -s $(SDKDIR)/system/linux/rtk/rtusr $(SDK_USRDIR)/rtusr; \
	fi
	@if [ ! -e $(SDK_USRDIR)/unittest ]; then \
		if [ -e $(SDKDIR)/unittest ]; then \
			ln -s $(SDKDIR)/unittest $(SDK_USRDIR)/unittest; \
		fi \
	fi
	@if [ ! -e $(SDK_USRDIR)/ovs ]; then \
		if [ -e $(SDKDIR)/src/app/ovs ]; then \
			ln -s $(SDKDIR)/src/app/ovs $(SDK_USRDIR)/ovs; \
		fi \
	fi

sdk.cleanup: flash.cleanup usbhcd.cleanup
	@rm -rf $(SDK_DRVDIR)/*
	@rm -rf $(SDK_NETDIR)/*
	@rm -rf $(SDK_LIBDIR)
	@rm -rf $(SDK_USRDIR)
	@rm -rf $(MACHARCH)
	@rm -rf $(MACHINC)

sdk.clean:
	@if [ -e $(SDK_RTCOREDIR) ]; then \
		$(MAKE) -C $(SDK_RTCOREDIR) clean; \
	fi
	@if [ -e $(SDK_ETHDIR) ]; then \
		$(MAKE) -C $(SDK_ETHDIR) clean; \
	fi
	@if [ -e $(SDK_RTKDIR) ]; then \
		$(MAKE) -C $(SDK_RTKDIR) clean; \
	fi

model.clean:
	@if [ -e $(MODELDIR) ]; then \
		$(MAKE) -C $(MODELDIR) clean; \
	fi

dist.setup:
	@if [ ! -e $(IMAGEDIR) ]; then \
		mkdir $(IMAGEDIR); \
	fi
	@if [ ! -e $(IMAGEDIR2) ]; then \
                mkdir $(IMAGEDIR2); \
    fi

flash.setup:
	@if [ ! -e $(FLASH_SRC)/rtk_nor_flash.c ]; then \
		ln -s $(FLASH_SRCDIR)/rtk_nor_flash.c $(FLASH_SRC)/; \
	fi
	@if [ ! -e $(FLASH_SRC)/rtk_spi_flash_mio.c ]; then \
		ln -s $(FLASH_SRCDIR)/rtk_spi_flash_mio.c $(FLASH_SRC)/; \
	fi
	@if [ ! -e $(FLASH_SRC)/rtk_flash_common.c ]; then \
		ln -s $(FLASH_SRCDIR)/rtk_flash_common.c $(FLASH_SRC)/; \
	fi
	@if [ ! -e $(FLASH_INC)/rtk_flash_common.h ]; then \
		ln -s $(FLASH_INCDIR)/rtk_flash_common.h $(FLASH_INC)/; \
	fi
	@if [ ! -e $(FLASH_INC)/rtk_norflash.h ]; then \
		ln -s $(FLASH_INCDIR)/rtk_norflash.h $(FLASH_INC)/; \
	fi
	@if [ ! -e $(FLASH_INC)/rtk_spiflash_mio.h ]; then \
		ln -s $(FLASH_INCDIR)/rtk_spiflash_mio.h $(FLASH_INC)/; \
	fi
	@if [ ! -e $(FLASH_SRC)/rtk_norsf_g3/ ]; then \
		ln -s $(FLASH_SRCDIR)/spi-nor/rtk_norsf_g3/ $(FLASH_SRC)/rtk_norsf_g3; \
	fi
	@if [ ! -e $(FLASH_SRC)/../nand/rtk_spi_nand/ ]; then \
		ln -s $(FLASH_SRCDIR)/../nand/rtk_spi_nand/ $(FLASH_SRC)/../nand/rtk_spi_nand; \
	fi

flash.cleanup:
	@rm -rf $(FLASH_SRC)/rtk_nor_flash.c $(FLASH_SRC)/rtk_spi_flash_mio.c
	@rm -rf $(FLASH_SRC)/rtk_flash_common.c
	@rm -rf $(FLASH_INC)/rtk_flash_common.h $(FLASH_INC)/rtk_norflash.h
	@rm -rf $(FLASH_INC)/rtk_spiflash_mio.h
	@rm -f $(FLASH_SRC)/rtk_norsf_g3
	@rm -f $(FLASH_SRC)/../nand/rtk_spi_nand


usbhcd.setup:
	@if [ ! -e $(USBHCD_SRC)/ehci-rtk_gen1.c ]; then \
		ln -s $(USBHCD_SRCDIR)/ehci-rtk_gen1.c $(USBHCD_SRC)/; \
	fi
	@if [ ! -e $(USBHCD_SRC)/rtk_gen1-res.c ]; then \
		ln -s $(USBHCD_SRCDIR)/rtk_gen1-res.c $(USBHCD_SRC)/; \
	fi

usbhcd.cleanup:
	@rm -rf $(USBHCD_SRC)/ehci-rtk_gen1.c
	@rm -rf $(USBHCD_SRC)/rtk_gen1-res.c

############################################################################

#
# Config stuff,  we recall ourselves to load the new config.arch before
# running the kernel and other config scripts
#

.PHONY: config.tk config.in

#ifneq ($(CONFIG_SYS_CODEBASE), SDK)
#config.in: target.sys.vendor.build
#else
config.in:
#endif
	@chmod u+x config/mkconfig
	config/mkconfig > config.in

config.tk: config.in
	$(MAKE) -C $(SCRIPTSDIR) tkparse
	ARCH=dummy $(SCRIPTSDIR)/tkparse < config.in > config.tmp
	@if [ -f /usr/local/bin/wish ];	then \
		echo '#!'"/usr/local/bin/wish -f" > config.tk; \
	else \
		echo '#!'"/usr/bin/wish -f" > config.tk; \
	fi
	cat $(SCRIPTSDIR)/header.tk >> ./config.tk
	cat config.tmp >> config.tk
	rm -f config.tmp
	echo "set defaults \"/dev/null\"" >> config.tk
	echo "set help_file \"config/Configure.help\"" >> config.tk
	cat $(SCRIPTSDIR)/tail.tk >> config.tk
	chmod 755 config.tk

.PHONY: xconfig
xconfig: config.tk
	@wish -f config.tk
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_xconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_xconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_xconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_SDK=y" .config > /dev/null; then \
		$(MAKE) sdk_xconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_PSWITCH=y" .config > /dev/null; then \
		$(MAKE) pswitch_xconfig; \
	 fi
	@config/setconfig final


.PHONY: kernel_prepare
kernel_prepare:
	KCONFIG_NOTIMESTAMP=1 $(MAKEARCH_KERNEL) -C $(LINUXDIR) prepare

.PHONY: kernel_scripts
kernel_scripts:
	KCONFIG_NOTIMESTAMP=1 $(MAKEARCH_KERNEL) -C $(LINUXDIR) scripts

.PHONY: config
config: config.in dist.setup
	@if [ ! -f .config ]; then \
		@HELP_FILE=config/Configure.help; \
			$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure -d config.in; \
	fi
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL_3_18_24=y" .config > /dev/null; then \
			rm -f $(ROOTDIR)/target; \
		ln -s $(ROOTDIR)/../../sdk/system/linux/linux-3.18.24.x/arch/mips/realtek/sel_chip/ $(ROOTDIR)/target; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_LOADER=y" .config > /dev/null; then \
		$(MAKE) loader_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_SDK=y" .config > /dev/null; then \
		$(MAKE) sdk_config; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
	if egrep "^CONFIG_DEFAULTS_PSWITCH=y" .config > /dev/null; then \
		$(MAKE) pswitch_config; \
	fi \
	 fi
	@config/setconfig final
	@if [ -e $(LOADER_CONFIG) ]; then \
		yes "" | $(MAKE) loader_config; \
	 fi
	@if [ ! -e $(SDK_AUTOCONF) ]; then \
		yes "" | $(MAKE) sdk_config; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
	if [ ! -e $(SYS_AUTOCONF) ]; then \
			yes "" | $(MAKE) pswitch_config; \
		fi; \
		if egrep "^CONFIG_LOADER_UBOOT=y" .config > /dev/null; then \
			echo "Link u-boot image header file";\
			if [ ! -e $(ROOTDIR)/../../u-boot/include/pswitch ]; then  \
				mkdir $(ROOTDIR)/../../u-boot/include/pswitch; \
			fi; \
			ln -f -s $(ROOTDIR)/../../u-boot/include/image.h $(ROOTDIR)/../../u-boot/include/pswitch/image.h;\
		fi; \
	 fi


.PHONY: rtConfig
rtConfig: config.in dist.setup
	@if [ ! -f .config ]; then \
		@HELP_FILE=config/Configure.help; \
			$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure -d config.in; \
	fi
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL_3_18_24=y" .config > /dev/null; then \
			rm -f $(ROOTDIR)/target; \
		ln -s $(ROOTDIR)/../../sdk/system/linux/linux-3.18.24.x/arch/mips/realtek/sel_chip/ $(ROOTDIR)/target; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_LOADER=y" .config > /dev/null; then \
		$(MAKE) loader_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_SDK=y" .config > /dev/null; then \
		$(MAKE) rtSdk_config; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
	if egrep "^CONFIG_DEFAULTS_PSWITCH=y" .config > /dev/null; then \
		$(MAKE) pswitch_config; \
	fi \
	 fi
	@config/setconfig final
	@if [ -e $(LOADER_CONFIG) ]; then \
		yes "" | $(MAKE) loader_config; \
	 fi
	@if [ ! -e $(SDK_AUTOCONF) ]; then \
		yes "" | $(MAKE) rtSdk_config; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
	if [ ! -e $(SYS_AUTOCONF) ]; then \
			yes "" | $(MAKE) pswitch_config; \
		fi; \
		if egrep "^CONFIG_LOADER_UBOOT=y" .config > /dev/null; then \
			echo "Link u-boot image header file";\
			if [ ! -e $(ROOTDIR)/../../u-boot/include/pswitch ]; then  \
				mkdir $(ROOTDIR)/../../u-boot/include/pswitch; \
			fi; \
			ln -f -s $(ROOTDIR)/../../u-boot/include/image.h $(ROOTDIR)/../../u-boot/include/pswitch/image.h;\
		fi; \
	 fi


.PHONY: menuconfig
menuconfig: config.in dist.setup
	$(MAKE) -C $(SCRIPTSDIR)/lxdialog all
	@HELP_FILE=config/Configure.help \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Menuconfig config.in
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL_3_18_24=y" .config > /dev/null; then \
			rm -f $(ROOTDIR)/target; \
		ln -s $(ROOTDIR)/../../sdk/system/linux/linux-3.18.24.x/arch/mips/realtek/sel_chip/ $(ROOTDIR)/target; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_LOADER=y" .config > /dev/null; then \
		$(MAKE) loader_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_SDK=y" .config > /dev/null; then \
		$(MAKE) sdk_menuconfig; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
	 if egrep "^CONFIG_DEFAULTS_PSWITCH=y" .config > /dev/null; then \
		$(MAKE) pswitch_menuconfig; \
	 fi \
	 fi
	@config/setconfig final
	@if [ ! -e $(LOADER_CONFIG) ]; then \
		yes "" | $(MAKE) loader_config; \
	 fi
	@if [ ! -e $(SDK_AUTOCONF) ]; then \
		yes "" | $(MAKE) sdk_config; \
	 else \
		$(MAKE) sdk.setup; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
		if [ ! -e $(SYS_AUTOCONF) ]; then \
			yes "" | $(MAKE) pswitch_config; \
		fi; \
	 fi


.PHONY: rtMenuconfig
rtMenuconfig: config.in dist.setup
	$(MAKE) -C $(SCRIPTSDIR)/lxdialog all
	@HELP_FILE=config/Configure.help \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Menuconfig config.in
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL_3_18_24=y" .config > /dev/null; then \
			rm -f $(ROOTDIR)/target; \
		ln -s $(ROOTDIR)/../../sdk/system/linux/linux-3.18.24.x/arch/mips/realtek/sel_chip/ $(ROOTDIR)/target; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_LOADER=y" .config > /dev/null; then \
		$(MAKE) loader_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_SDK=y" .config > /dev/null; then \
		$(MAKE) rtSdk_menuconfig; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
	 if egrep "^CONFIG_DEFAULTS_PSWITCH=y" .config > /dev/null; then \
		$(MAKE) pswitch_menuconfig; \
	 fi \
	 fi
	@config/setconfig final
	@if [ ! -e $(LOADER_CONFIG) ]; then \
		yes "" | $(MAKE) loader_config; \
	 fi
	@if [ ! -e $(SDK_AUTOCONF) ]; then \
		yes "" | $(MAKE) rtSdk_config; \
	 fi
	@if egrep "^CONFIG_SYS_CODEBASE=PSWITCH" .config > /dev/null; then \
		if [ ! -e $(SYS_AUTOCONF) ]; then \
			yes "" | $(MAKE) pswitch_config; \
		fi; \
	 fi


.PHONY: oldconfig
oldconfig: config.in
	@if [ ! -e $(IMAGEDIR) ]; then \
		mkdir $(IMAGEDIR); \
	fi
	@if [ ! -e $(IMAGEDIR2) ]; then \
                mkdir $(IMAGEDIR2); \
        fi
	HELP_FILE=config/Configure.help \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure -d config.in
	@$(MAKE) oldconfig_linux
	@$(MAKE) oldconfig_modules
	@$(MAKE) oldconfig_config
	@$(MAKE) oldconfig_uClibc
	@$(MAKE) oldconfig_loader
	@chmod u+x config/setconfig
	@config/setconfig final

.PHONY: modules
modules:
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		[ -d $(LINUXDIR)/modules ] || mkdir $(LINUXDIR)/modules; \
		$(MAKEARCH_KERNEL) -C $(LINUXDIR) modules; \
	fi

.PHONY: modules_install
modules_install:
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		[ -d $(SYSROMFSDIR)/lib/modules ] || mkdir -p $(SYSROMFSDIR)/lib/modules; \
		$(MAKEARCH_KERNEL) -C $(LINUXDIR) INSTALL_MOD_PATH=$(SYSROMFSDIR) modules_install; \
		rm -f $(SYSROMFSDIR)/lib/modules/*/build; \
		rm -f $(SYSROMFSDIR)/lib/modules/*/source; \
		find $(SYSROMFSDIR)/lib/modules -type f -name "*o" | xargs -r $(STRIP) -R .comment -R .note -g --strip-unneeded; \
		find $(MODSQFSDIR)/lib/modules "-name '*.builtin' -o -name '*.order'" -type f -print | xargs rm -f; \
	fi

linux_xconfig:
	KCONFIG_NOTIMESTAMP=1 $(MAKEARCH_KERNEL) -C $(LINUXDIR) xconfig
linux_menuconfig:
	KCONFIG_NOTIMESTAMP=1 $(MAKEARCH_KERNEL) -C $(LINUXDIR) menuconfig
linux_config:
	KCONFIG_NOTIMESTAMP=1 $(MAKEARCH_KERNEL) -C $(LINUXDIR) config
loader_xconfig:
	$(MAKEARCH) -C $(LOADER_PATH) xconfig
loader_menuconfig:
	$(MAKEARCH) -C $(LOADER_PATH) menuconfig
loader_config:
	$(MAKEARCH) -C $(LOADER_PATH) config
sdk_xconfig: sdk.setup
	$(MAKEARCH) -C $(SDKCFGDIR) xconfig
sdk_menuconfig: sdk.setup
	$(MAKEARCH) -C $(SDKCFGDIR) menuconfig
sdk_config: sdk.setup
	$(MAKEARCH) -C $(SDKCFGDIR) config
rtSdk_xconfig:
	$(MAKEARCH) -C $(SDKCFGDIR) xconfig
rtSdk_menuconfig:
	$(MAKEARCH) -C $(SDKCFGDIR) menuconfig
rtSdk_config:
	$(MAKEARCH) -C $(SDKCFGDIR) config
ifneq ($(CONFIG_SYS_CODEBASE), SDK)
pswitch_xconfig:
	$(MAKEARCH) -C $(SYS_CFGDIR) xconfig
pswitch_menuconfig:
	$(MAKEARCH) -C $(SYS_CFGDIR) menuconfig
pswitch_config:
	$(MAKEARCH) -C $(SYS_CFGDIR) config
endif
modules_xconfig:
	[ ! -d modules ] || $(MAKEARCH) -C modules xconfig
modules_menuconfig:
	[ ! -d modules ] || $(MAKEARCH) -C modules menuconfig
modules_config:
	[ ! -d modules ] || $(MAKEARCH) -C modules config
modules_clean:
	-[ ! -d modules ] || $(MAKEARCH) -C modules clean
config_xconfig:
	$(MAKEARCH) -C config xconfig
ifeq ($(shell [[ '$(CONFIG_SYS_CODEBASE)' == 'PSWITCH' && '$(CONFIG_LINUXDIR)' == 'linux-3.18.24.x' ]] && echo true ),true)
config_menuconfig:
	$(MAKEARCH) -C user/busybox-1.20.2 menuconfig
else
config_menuconfig:
	$(MAKEARCH) -C config menuconfig
endif
config_config:
	$(MAKEARCH) -C config config
oldconfig_config:
	$(MAKEARCH) -C config oldconfig
oldconfig_modules:
	[ ! -d modules ] || $(MAKEARCH) -C modules oldconfig
oldconfig_linux:
	KCONFIG_NOTIMESTAMP=1 $(MAKEARCH_KERNEL) -C $(LINUXDIR) oldconfig
oldconfig_uClibc:
	[ -z "$(findstring uClibc,$(LIBCDIR))" ] || $(MAKEARCH) -C $(LIBCDIR) oldconfig
oldconfig_loader:
	$(MAKEARCH) -C $(LOADERDIR) board

############################################################################
#
# normal make targets
#

.PHONY: romfs
#ifneq ($(CONFIG_SYS_CODEBASE), SDK)
#romfs: romfs.subdirs modules_install romfs.post romfs.sdk target.sys.install
#else
romfs: romfs.subdirs modules_install romfs.post romfs.sdk
#endif

.PHONY: romfs.subdirs
romfs.subdirs:
	for dir in vendors $(DIRS) ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir romfs || exit 1 ; done
ifeq ($(CONFIG_LINUXDIR), linux-2.6.x)
	@cat $(ROMFSDIR)/etc/rc | sed 's/X.X.XX/2.6.19/g' > $(ROMFSDIR)/etc/rc.new
	@rm $(ROMFSDIR)/etc/rc
	@mv $(ROMFSDIR)/etc/rc.new $(ROMFSDIR)/etc/rc
	@chmod 755 $(ROMFSDIR)/etc/rc
endif
ifeq ($(CONFIG_LINUXDIR), linux-3.18.24.x)
	@cat $(ROMFSDIR)/etc/rc | sed 's/X.X.XX/3.18.24/g' > $(ROMFSDIR)/etc/rc.new
	@rm $(ROMFSDIR)/etc/rc
	@mv $(ROMFSDIR)/etc/rc.new $(ROMFSDIR)/etc/rc
	@chmod 755 $(ROMFSDIR)/etc/rc
endif

.PHONY: romfs.subdirs2
romfs.subdirs2:
	for dir in vendors include lib include ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir romfs || exit 1 ; done
	for dir in user ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir -f Makefile.rtSdkApp romfs || exit 1 ; done
ifeq ($(CONFIG_LINUXDIR), linux-2.6.x)
	@cat $(ROMFSDIR)/etc/rc | sed 's/X.X.XX/2.6.19/g' > $(ROMFSDIR)/etc/rc.new
	@rm $(ROMFSDIR)/etc/rc
	@mv $(ROMFSDIR)/etc/rc.new $(ROMFSDIR)/etc/rc
	@chmod 755 $(ROMFSDIR)/etc/rc
endif
ifeq ($(CONFIG_LINUXDIR), linux-3.18.24.x)
	@cat $(ROMFSDIR)/etc/rc | sed 's/X.X.XX/3.18.24/g' > $(ROMFSDIR)/etc/rc.new
	@rm $(ROMFSDIR)/etc/rc
	@mv $(ROMFSDIR)/etc/rc.new $(ROMFSDIR)/etc/rc
	@chmod 755 $(ROMFSDIR)/etc/rc
endif

.PHONY: romfs.post
romfs.post:
	$(MAKEARCH) -C vendors romfs.post
	-find $(ROMFSDIR)/. -name .svn | xargs -r rm -rf
	@if [ -e $(SQFSDIR) ]; then \
		find $(SQFSDIR)/. -name .svn | xargs -r rm -rf; \
	fi
	@cp uClibc/lib/libgcc_s.so.1 $(ROMFSDIR)/lib/

romfs.sdk:
	@if egrep "^CONFIG_SDK_LINKED_KMOD=y" $(SDK_CONFIG) > /dev/null; then \
	mkdir -p $(SDK_DST_MODDIR)/unittest; \
	cp $(SDK_SRC_MODDIR)/unittest/sdktest.kmod $(SDK_DST_MODDIR)/unittest/sdktest.ko; \
	mkdir -p $(SDK_DST_MODDIR)/rtk; \
	cp $(SDK_SRC_MODDIR)/rtk/rtk.kmod $(SDK_DST_MODDIR)/rtk/rtk.ko; \
	mkdir -p $(PROTO_DST_MODDIR)/rtnic; \
	cp $(PROTO_SRC_MODDIR)/rtnic/rtnic.kmod $(PROTO_DST_MODDIR)/rtnic/rtnic.ko; \
	mkdir -p $(PROTO_DST_MODDIR)/rtdrv; \
	cp $(PROTO_SRC_MODDIR)/rtdrv/rtdrv.kmod $(PROTO_DST_MODDIR)/rtdrv/rtdrv.ko; \
	fi


.PHONY: image
image:
	[ -d $(IMAGEDIR) ] || mkdir $(IMAGEDIR)
	$(MAKEARCH) -C vendors image

.PHONY: release
release:
	make -C release release

%_fullrelease:
	@echo "This target no longer works"
	@echo "Do a make -C release $@"
	exit 1
#
# fancy target that allows a vendor to have other top level
# make targets,  for example "make vendor_flash" will run the
# vendor_flash target in the vendors directory
#

vendor_%:
	$(MAKEARCH) -C vendors $@

.PHONY: linux
linux linux%_only: sqfsdir
	@if [ ! -e $(ROMFSDIR) ]; then \
                mkdir -p $(ROMFSDIR); \
        fi
	@if expr "$(LINUXDIR)" : 'linux-2\.[0-4].*' > /dev/null && \
			 [ ! -f $(LINUXDIR)/.depend ] ; then \
		echo "ERROR: you need to do a 'make dep' first" ; \
		exit 1 ; \
	fi
ifeq ($(CONFIG_LINUXDIR), linux-2.6.x)
	$(MAKEARCH_KERNEL) -j$(HOST_NCPU) -C $(LINUXDIR) $(LINUXTARGET) || exit 1
endif
ifeq ($(CONFIG_LINUXDIR), linux-3.18.24.x)
	$(MAKEARCH_KERNEL) -I$(SDKINCCOM) -j$(HOST_NCPU) -C $(LINUXDIR) $(LINUXTARGET) || exit 1
endif
	if [ -f $(LINUXDIR)/vmlinux ]; then \
		ln -f $(LINUXDIR)/vmlinux $(LINUXDIR)/linux ; \
	fi

.PHONY: sqfsdir
ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
sqfsdir:
	@if [ ! -e $(ROMFSDIR) ]; then \
                mkdir -p $(ROMFSDIR)/sqfs; \
                mkdir -p $(ROMFSDIR)/modsqfs; \
        fi
	@if [ ! -e $(SQFSDIR) ]; then \
                mkdir -p $(SQFSDIR)/bin; \
                mkdir -p $(SQFSDIR)/home/web; \
                mkdir -p $(SQFSDIR)/lib; \
                mkdir -p $(SQFSDIR)/usr/bin; \
	fi
else
sqfsdir:
endif

.PHONY: sparse
sparse:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) C=1 $(LINUXTARGET) || exit 1

.PHONY: sparseall
sparseall:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) C=2 $(LINUXTARGET) || exit 1

.PHONY: subdirs
subdirs: linux modules
	for dir in $(DIRS) ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir || exit 1 ; done

.PHONY: userapp rtSdkApp
userapp:
	for dir in $(DIRS) ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir || exit 1 ; done

rtSdkApp:
	$(MAKEARCH) -C user -f Makefile.rtSdkApp

dep:
	@if [ ! -f $(LINUXDIR)/.config ] ; then \
		echo "ERROR: you need to do a 'make config' first" ; \
		exit 1 ; \
	fi
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) dep

# This one removes all executables from the tree and forces their relinking
.PHONY: relink
relink:
	find user prop vendors -type f -name '*.gdb' | sed 's/^\(.*\)\.gdb/\1 \1.gdb/' | xargs rm -f

#ifneq ($(CONFIG_SYS_CODEBASE), SDK)
#clean: modules_clean sdk.clean target.sys.clean model.clean
#else
clean: modules_clean sdk.clean model.clean
#endif
	for dir in $(LINUXDIR) $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir clean ; done
	@rm -rf $(ROMFSDIR)/*
	@rm -f $(IMAGEDIR)/* $(IMAGEDIR2)/*
	@rm -f config.tk
	@rm -f $(LINUXDIR)/linux
	@rm -f $(LINUXDIR)/include/asm
	@rm -rf $(LINUXDIR)/net/ipsec/alg/libaes $(LINUXDIR)/net/ipsec/alg/perlasm
	@rm -rf sqfs modsqfs

real_clean mrproper: clean
	#-$(MAKEARCH_KERNEL) -C $(LINUXDIR) mrproper
	-$(MAKEARCH) -C config clean
ifneq ($(CONFIG_LIBCDIR), uClibc-0.9.33)
ifneq ($(CONFIG_LIBCDIR), uClibc-0.9.33-485-200)
	@if [ -e uClibc ]; then \
		$(MAKEARCH) -C uClibc distclean; \
	fi
endif
endif
	@if [ -e $(LOADER_PATH) ]; then \
		$(MAKE) -C $(LOADER_PATH) distclean; \
	fi
	@if [ -e $(SDKCFGDIR) ]; then \
		$(MAKE) -C $(SDKCFGDIR) clean; \
	fi
ifneq ($(CONFIG_SYS_CODEBASE), SDK)
	@if [ -e $(SYS_CFGDIR) ]; then \
		$(MAKE) -C $(SYS_CFGDIR) clean; \
	fi
endif
	@if egrep "^CONFIG_LINUXDIR=linux-3.18.24.x" .config > /dev/null; then \
			rm -f $(ROOTDIR)/target; \
	 fi
	@rm -rf romfs config.in config.arch config.tk $(IMAGEDIR2) $(LOADER_PATH)
	@rm -f modules/config.tk images/*
	@rm -rf .config .config.old .oldconfig autoconf.h .menuconfig.log $(LOADER_CONFIG)
	@rm -f tools/cksum tools/ucfront-g++ tools/ucfront-gcc tools/ucfront-ld uClibc lib/uClibc
	@rm -rf sqfs modsqfs
	@rm -rf $(SDKDIR)/src/app/diag/include/parser/cparser_tree.h
	@rm -rf $(SDKDIR)/src/app/diag/src/parser/cparser_tree.c
	@rm -rf $(SDKDIR)/src/app/diag/src/parser/cparser_tree.o
	@rm -rf $(SDKDIR)/src/app/diag_v2/include/parser/cparser_tree.h
	@rm -rf $(SDKDIR)/src/app/diag_v2/src/parser/cparser_tree.c
	@rm -rf $(SDKDIR)/src/app/diag_v2/src/parser/cparser_tree.o
	@rm -rf images
	@rm -rf lib/uClibc-0.9.33
	@rm -rf uClibc-0.9.33/include/config
	@rm -rf uClibc-0.9.33/include/generated
	rm -rf $(LOADERDIR)/.config.old
	@rm -rf $(SDKDIR)/system/linux/rtcore/Makefile.rtcore
	
distclean: mrproper sdk.cleanup
	@find $(TOPDIR) \( -name '*.builtin' -o -name '*.order' \) -type f -print | xargs rm -f
	@find $(KERNELDIR) \( -name '*.cmd' \) -type f -print | xargs rm -f
	@rm -rf user/tinylogin/applet_source_list 		\
		user/tinylogin/config.h 			\
		user/busybox/.config.mkconfig			\
		user/busybox-1.18.3/.config.old			\
		user/busybox-1.18.3/docs/BusyBox.html		\
		user/busybox-1.18.3/docs/busybox.1		\
		user/busybox-1.18.3/docs/busybox.net/BusyBox.html	\
		user/busybox-1.18.3/include/autoconf.h		\
		user/busybox-1.18.3/scripts/basic/docproc	\
		user/busybox-1.18.3/scripts/basic/fixdep	\
		user/busybox-1.18.3/scripts/basic/split-include	\
		user/busybox-1.18.3/scripts/kconfig/conf	\
		lib/uClibc-0.9.28				\
		uClibc-0.9.28/libc.code				\
		linux-2.6.x/.config 				\
		linux-2.6.x/.config.old 			\
		linux-2.6.x/.version				\
		linux-2.6.x/scripts/mod/modpost			\
		linux-2.6.x/scripts/mod/mk_elfconfig		\
		linux-2.6.x/scripts/kallsyms 			\
		linux-2.6.x/scripts/kconfig/conf		\
		linux-2.6.x/scripts/basic/docproc 		\
		linux-2.6.x/scripts/basic/fixdep		\
		linux-2.6.x/Module.symvers 			\
		linux-2.6.x/include/config/auto.conf 		\
		linux-2.6.x/include/config/auto.conf.cmd 	\
		linux-2.6.x/include/config/blk			\
		linux-2.6.x/include/config/debug/preempt.h	\
		linux-2.6.x/include/config/inet6		\
		linux-2.6.x/include/config/ipv6			\
		linux-2.6.x/include/config/ipv6.h		\
		linux-2.6.x/include/config/lock			\
		linux-2.6.x/include/config/module		\
		linux-2.6.x/include/config/preempt/bkl.h	\
		linux-2.6.x/include/config/preempt.h		\
		linux-2.6.x/include/config/proc/sysctl.h	\
		linux-2.6.x/include/config/rtl8328		\
		linux-2.6.x/include/config/rtl8380		\
		linux-2.6.x/include/config/rtl8390		\
		linux-2.6.x/include/config/squashfs		\
		linux-2.6.x/include/config/sysctl.h		\
		linux-2.6.x/include/config/xfrm.h		\
		linux-2.6.x/include/linux/autoconf.h    \
		tools/sg-cksum/cksum				\
		tools/ucfront/ucfront				\
		tools/ucfront/ucfront-ld

%_only:
	@case "$(@)" in \
	*/*) d=`expr $(@) : '\([^/]*\)/.*'`; \
	     t=`expr $(@) : '[^/]*/\(.*\)'`; \
	     $(MAKEARCH) -C $$d $$t;; \
	*)   $(MAKEARCH) -C $(@:_only=);; \
	esac

%_clean:
	@case "$(@)" in \
	*/*) d=`expr $(@) : '\([^/]*\)/.*'`; \
	     t=`expr $(@) : '[^/]*/\(.*\)'`; \
	     $(MAKEARCH) -C $$d $$t;; \
	*)   $(MAKEARCH) -C $(@:_clean=) clean;; \
	esac

%_default:
	@if [ ! -f "vendors/$(@:_default=)/config.device" ]; then \
		echo "vendors/$(@:_default=)/config.device must exist first"; \
		exit 1; \
	 fi
	-make clean > /dev/null 2>&1
	cp vendors/$(@:_default=)/config.device .config
	chmod u+x config/setconfig
	yes "" | config/setconfig defaults
	config/setconfig final
	make dep
	make

config_error:
	@echo "*************************************************"
	@echo "You have not run make config."
	@echo "The build sequence for this source tree is:"
	@echo "1. 'make config' or 'make xconfig'"
	@echo "2. 'make dep'"
	@echo "3. 'make'"
	@echo "*************************************************"
	@exit 1

prune: ucfront
	@for i in `ls -d linux-* | grep -v $(LINUXDIR)`; do \
		rm -fr $$i; \
	done
	$(MAKE) -C lib prune
	$(MAKE) -C user prune
	$(MAKE) -C vendors prune

dist-prep:
	-find $(ROOTDIR) -name 'Makefile*.bin' | while read t; do \
		$(MAKEARCH) -C `dirname $$t` -f `basename $$t` $@; \
	 done

############################################################################


