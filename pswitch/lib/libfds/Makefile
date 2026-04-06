#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(CURDIR)/../../
include $(PSWITCH_DIR)/make.root.linux.mk
include $(LOADERDIR)/.config

CFLAGS_EXTRA += -DCONFIG_IH_MAGIC_NUMBER=0x$(CONFIG_IH_MAGIC_NUMBER)

# target
LIBNAME = libfds.so
OBJS_y  = $(TARGET)/fds_init.o \
          $(TARGET)/fds_export.o \
          $(TARGET)/fds_default.o \
          $(TARGET)/fds_if_restore.o \

#Put after OBJS
include ../Makefile.lib
