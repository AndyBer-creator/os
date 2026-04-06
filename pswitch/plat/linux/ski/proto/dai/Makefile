#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
ifndef PSWITCH_DIR
PSWITCH_DIR := $(CURDIR)/../../../../..
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
endif

DAIDIR := ../../../../../proto/dai
include $(SYS_PROTO_DAIDIR)/Makefile

EXTRA_CFLAGS += -Wall -Werror

ifeq ($(CONFIG_SYS_PLAT_LXSKI),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKIDIR)/Module.symvers
endif

ifeq ($(CONFIG_SYS_PROTO_ISG),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKI_PROTO_ISGDIR)/Module.symvers
endif
export KBUILD_EXTRA_SYMBOLS

ALL_OBJ := $(DAI_OBJ) ski_dai_lkm.o ski_dai_lkm_export.o ski_dai.o
TARGET = l2g_dai
obj-$(CONFIG_SYS_PROTO_DAI) += $(TARGET).o
$(TARGET)-objs	:= $(ALL_OBJ)

all:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules
	$(SYSINSTALL) $(TARGET).ko

romfs:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules_install

clean:
	@$(RM) $(ALL_OBJ)
	@find \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' -o -name '*.symvers' \
                -o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' -o -name '.tmp_versions' \) \
                -print | xargs rm -rf

.PHONY: clean romfs
