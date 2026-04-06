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
EXEC := handler

OBJS                          := handler.o
OBJS                          += handler_sys.o
OBJS                          += handler_l3.o
OBJS_$(CONFIG_SYS_PROTO_ISG)  += handler_ipsg.o
OBJS_$(CONFIG_SYS_UI_SNMP)    += handler_snmp.o
OBJS += $(OBJS_y) $(OBJS_m)

#app lib
LIB     += -L$(SYS_INSTDIR) -lsal -lcfg -lcfgtbl -lglue -lfds -lcmd -lclicmd -lvty
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
