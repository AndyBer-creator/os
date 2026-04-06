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

LLDPDIR := ../../../../../proto/lldp
include $(SYS_PROTO_LLDPDIR)/Makefile

EXTRA_CFLAGS += -Wall -Werror

ifeq ($(CONFIG_SYS_PLAT_LXSKI),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKIDIR)/Module.symvers
endif

ifeq ($(CONFIG_SYS_VOICEVLAN),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKI_PROTO_VOICEVLANDIR)/Module.symvers
endif
export KBUILD_EXTRA_SYMBOLS

ifdef CONFIG_SYS_PROTO_LLDP_MED
ALL_OBJ := ski_lldp_med.o
endif
ALL_OBJ += $(LLDP_OBJ) ski_lldp_lkm.o ski_lldp_lkm_export.o ski_lldp.o
TARGET = l2g_lldp
obj-$(CONFIG_SYS_PROTO_LLDP) += $(TARGET).o
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

