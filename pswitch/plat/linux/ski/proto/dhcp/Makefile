#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
ifndef PSWITCH_DIR
PSWITCH_DIR = $(CURDIR)/../../../../..
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
endif

DHCPDIR := ../../../../../proto/dhcp
include $(SYS_PROTO_DHCPDIR)/Makefile

EXTRA_CFLAGS += -Wall -Werror

ifeq ($(CONFIG_SYS_PLAT_LXSKI),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKIDIR)/Module.symvers
endif

ifeq ($(CONFIG_SYS_PROTO_ISG),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKI_PROTO_ISGDIR)/Module.symvers
endif
export KBUILD_EXTRA_SYMBOLS

ALL_OBJ      := $(DHCP_OBJ) ski_dhcp_lkm.o ski_dhcp_lkm_export.o ski_dhcp.o
TARGET = l2g_dhcp
obj-$(CONFIG_SYS_PROTO_DHCP) += $(TARGET).o
$(TARGET)-objs	:= $(ALL_OBJ)

all:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules
	$(SYSINSTALL) $(TARGET).ko

romfs:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules_install

clean:
	@$(RM) $(ALL_OBJ)
	@find \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \) \
		-type f -print | xargs rm -f

