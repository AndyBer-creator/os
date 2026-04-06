#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
ifndef SYS_ROOTDIR
SYS_ROOTDIR := $(PSWITCH_DIR)
endif
ROOTDIR     := $(SYS_ROOTDIR)/../kernel/uClinux

-include $(ROOTDIR)/.config
-include $(ROOTDIR)/$(CONFIG_LINUXDIR)/.config

LINUXDIR = $(CONFIG_LINUXDIR)
LIBCDIR  = $(CONFIG_LIBCDIR)
PATH	 := $(PATH):$(ROOTDIR)/tools
HOSTCC   = cc

IMAGEDIR        = $(ROOTDIR)/images
IMAGEDIR2       = $(ROOTDIR)/../../image
LOADERDIR       = $(ROOTDIR)/../../loader/$(CONFIG_LOADERDIR)
SDKDIR          = $(ROOTDIR)/../../sdk

MKIMAGE    = $(LOADERDIR)/tools/mkimage
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

BUILD_START_STRING ?= $(shell date "+%a, %d %b %Y %T %z")
ifndef NON_SMP_BUILD
HOST_NCPU := $(shell if [ -f /proc/cpuinfo ]; then n=`grep -c processor /proc/cpuinfo`; if [ $$n -gt 1 ];then expr $$n \* 2; else echo $$n; fi; else echo 1; fi)
else
HOST_NCPU := 1
endif

LINUX_CONFIG   = $(ROOTDIR)/$(LINUXDIR)/.config
CONFIG_CONFIG  = $(ROOTDIR)/config/.config
MODULES_CONFIG = $(ROOTDIR)/modules/.config
LOADER_CONFIG  = $(LOADERDIR)/.config
SDK_CONFIG     = $(SDKCFGDIR)/.config
SDK_AUTOCONF   = $(SDKSYSINC)/common/rt_autoconf.h

MACHARCH      = $(LINUXDIR)/arch/mips/realtek
MACHINC       = $(LINUXDIR)/include/asm-mips/mach-realtek
MACHINCLUDE   = $(ROOTDIR)/$(MACHINC)

FLASH_SRC     = $(LINUXDIR)/drivers/mtd/maps
FLASH_INC     = $(LINUXDIR)/include/linux/mtd
FLASH_SRCDIR  = $(DRV_INCLUDE)/$(FLASH_SRC)
FLASH_INCDIR  = $(DRV_INCLUDE)/$(FLASH_INC)

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

ARCH_CONFIG = $(ROOTDIR)/config.arch

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

export VENDOR PRODUCT ROOTDIR LINUXDIR LOADERDIR HOSTCC CONFIG_SHELL
export CONFIG_CONFIG LINUX_CONFIG LOADER_CONFIG MODULES_CONFIG ROMFSDIR SCRIPTSDIR
export VERSIONPKG VERSIONSTR ROMFSINST PATH IMAGEDIR RELDIR RELFILES TFTPDIR
export SDK_CONFIG SDK_LIBDIR SDK_BUILD
export BUILD_START_STRING HOST_NCPU PSWITCH_DIR
export SQFSDIR SQFSROMDIR

ifdef PSWITCH_USER_MODE
UCLINUX_BUILD_USER=1
endif
-include $(ARCH_CONFIG)
-include $(LINUX_CONFIG)
-include $(CONFIG_CONFIG)
-include $(MODULES_CONFIG)
-include $(SDK_CONFIG)

export STRIP
