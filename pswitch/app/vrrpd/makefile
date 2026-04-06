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
EXEC := vrrpd

OBJS := vrrpFuncParse.o vrrpd.o vrrp_main.o 

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

include ../Makefile.app

postrequisite:
	$(ROMFSINST) /etc/inetd.conf
