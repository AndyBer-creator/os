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
EXEC := dhcpserver

#OBJS_$(CONFIG_SYS_L3_ROUTE) += fpmd_route.o
OBJS := dhcpserver_main.o dhcps.o hashCommon.o dhcpr.o  dhcpHash.o IPPool_func.o  \
		ipLink.o  dhcpSocket.o dhcpserverFuncParse.o $(OBJS_y) $(OBJS_m)

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
LIB += -lipc
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
