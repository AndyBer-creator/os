#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR := $(shell pwd)/../..
include $(PSWITCH_DIR)/make.config.mk

# target
EXEC = telnetd
OBJS = telnetd.o state.o termstat.o slc.o sys_term.o \
        utility.o global.o authenc.o logwtmp.o logout.o

POSTREQUISITE = postrequisite

CFLAGS_EXTRA += -DPARANOID_TTYS -DUSE_TERMIO -DKLUDGELINEMODE -D_GNU_SOURCE -g

ifdef CONFIG_USER_TELNETD_DOES_NOT_USE_OPENPTY
CFLAGS_EXTRA += -DCONFIG_USER_TELNETD_DOES_NOT_USE_OPENPTY
endif

ifeq ($(CONFIG_IPV6),y)
TCPVER = tcp6
else
TCPVER = tcp
endif

include ../Makefile.app

postrequisite:
	$(ROMFSINST) -e CONFIG_SYS_APP_TELNETD \
                -a "telnet  stream $(TCPVER) nowait root /bin/telnetd" /etc/inetd.conf
