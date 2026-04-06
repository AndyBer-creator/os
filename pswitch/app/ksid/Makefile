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
EXEC := ksid

OBJS_$(CONFIG_SYS_PROTO_ISG) += ksi_isg.o
OBJS_$(CONFIG_SYS_PROTO_AUTHMGR) += ksi_authmgr.o
OBJS_$(CONFIG_SYS_L3_ROUTE) += ksi_l3.o
OBJS := ksid.o ksi_init.o ksi_sys.o ksi_port.o $(OBJS_y) $(OBJS_m)

#app special include
APP_INCLUDE += -I$(SYS_APP_KSIDDIR)

#app lib
LIB     += -L$(SYS_INSTDIR) -lsal -lfds -lcfg -lcfgtbl -lcmd -lclicmd -lglue -lvty -lpthread
ifdef CONFIG_SYS_LIB_CUSTOM
LIB    += -lcustom
endif
ifdef CONFIG_SYS_LIB_TACPLUS
LIB     += -ltacplus
endif
ifdef CONFIG_SYS_LIB_RADIUS
LIB     += -lradius
endif
ifdef CONFIG_SYS_LIB_SSL
LIB += -lssl
LIB += -lcrypt
LIB += -lcrypto
endif
ifdef CONFIG_SYS_LIB_IPC
LIB     += -lipc
endif

ifdef CONFIG_SYS_LIB_UCI
LIB += -luci
endif

ifdef CONFIG_SYS_LIB_UBOX
LIB += -lubox
endif
include ../Makefile.app

postrequisite:
	$(ROMFSINST) /etc/inetd.conf
