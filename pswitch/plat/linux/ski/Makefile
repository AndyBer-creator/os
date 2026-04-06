#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
ifndef PSWITCH_DIR
PSWITCH_DIR = $(CURDIR)/../../..
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
endif

OSALDIR   := ../osal
EXTRA_CFLAGS += -Wall -Werror

ifdef CONFIG_SYS_RTL8380
CORE_FILE=ski_8382
MOD_FILE=modules_8382
endif

ifdef CONFIG_SYS_RTL9300
CORE_FILE=ski_9301
MOD_FILE=modules_9301
endif

ifdef CONFIG_SYS_RTL9310
ifdef CONFIG_SYS_L3_ROUTE_OSPF
CORE_FILE=ski_9311_l3
MOD_FILE=modules_9311_l3
else
CORE_FILE=ski_9311_l2
MOD_FILE=modules_9311_l2
endif
endif

TARGET = ski

all:
	cp -f $(CORE_FILE) $(TARGET).ko
	cp -f $(MOD_FILE) Module.symvers
	$(SYSINSTALL) $(TARGET).ko
	$(SYSINSTALL) Module.symvers

romfs:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules_install

clean:
	@$(RM) $(TARGET).ko
	@$(RM) Module.symvers

.PHONY: clean romfs

