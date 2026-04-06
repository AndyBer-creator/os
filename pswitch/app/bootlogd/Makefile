#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
# target
EXEC := bootklogd
OBJS := bootklogd.o

#app special include
APP_INCLUDE += -I$(LOADERDIR)/include

PSWITCH_DIR := $(CURDIR)/../..
include ../Makefile.app

#bootklogd must install at romfs
SYSROMFSINST    :=$(ROMFSINST)

