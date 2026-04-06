#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR := $(shell pwd)/../../../..
include $(PSWITCH_DIR)/make.config.mk

# target
EXEC := cli
OBJS := vtysh/vtysh_main.o vtysh/vtysh.o cli_main.o cli_readline_use.o cli_debug.o

#app special include
APP_INCLUDE += -I$(SYS_APPINCLUDE) -Ivtysh/ -I$(FLASH_INCDIR)

#app lib
LIB     += -L$(SYS_INSTDIR) -lsal -lcfg -lcfgtbl -lglue -lfds -lcmd -lclicmd -lvty -lreadline -lncurses -lpthread
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
include $(PSWITCH_DIR)/app/Makefile.app
