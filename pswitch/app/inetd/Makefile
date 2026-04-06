#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
# target
EXEC := inetd
OBJS := inetd.o config.o

SERVICES =    \"/etc/services\"
INETD_CONF =  \"/etc/inetd.conf\"
POSTREQUISITE = postrequisite

# These changes allow for an additional, optional config file in /etc/config/inetd.conf
# which can be modified
CFLAGS_EXTRA += -DSERVICES=$(SERVICES) -DINETD_CONF=$(INETD_CONF)
CFLAGS_EXTRA += -DCONFIG_IPV6 -DMODIFY_CONFIG -DALT_INETD_CONF=\"/etc/config/inetd.conf\"

PSWITCH_DIR := $(shell pwd)/../..
include ../Makefile.app

postrequisite:
	$(ROMFSINST) /etc/inetd.conf

