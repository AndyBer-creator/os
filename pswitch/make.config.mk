#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
include $(PSWITCH_DIR)/make.path.mk

SYS_ROOTDIR = $(PSWITCH_DIR)
SYS_CFGDIR   = $(SYS_ROOTDIR)/config
SYS_CONFIG   = $(SYS_CFGDIR)/.config
SYS_AUTOCONF = $(SYS_PLAT_COMMONDIR)/sys_autoconf.h

ifeq ($(SYS_CONFIG), $(wildcard $(SYS_CONFIG)))
-include $(SYS_CONFIG)
endif

SYSINSTALL = @cp -flt $(SYS_INSTDIR)

ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
SYSROMFSDIR  = $(MODSQFSDIR)
SYSROMFSINST = $(SQFSINST)
else
SYSROMFSDIR  = $(ROMFSDIR)
SYSROMFSINST = $(ROMFSINST)
endif

export SYS_CONFIG
