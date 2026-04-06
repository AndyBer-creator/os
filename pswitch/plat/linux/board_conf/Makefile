#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
ifndef PSWITCH_DIR
PSWITCH_DIR := $(CURDIR)/../../..
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
include $(PSWITCH_DIR)/make.path.mk
endif

include $(SYS_VENDOR_BOARDDIR)/Makefile

EXTRA_CFLAGS += -Wall -Werror

KBUILD_EXTRA_SYMBOLS += $(SDK_RTKDIR)/Module.symvers
export KBUILD_EXTRA_SYMBOLS

ALL_OBJ := board_conf_lkm.o board_conf_lkm_export.o board_conf_common.o $(VENDOR_BOARD_CONF_OBJ)
TARGET  := board_conf
obj-$(CONFIG_SYS_PLAT_LXBOARD) += $(TARGET).o
$(TARGET)-objs  := $(ALL_OBJ)

EXTRA_CFLAGS += -I$(LOADERDIR)/include

all:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules
	$(SYSINSTALL) $(TARGET).ko

romfs:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules_install

clean:
	@$(RM) $(ALL_OBJ)
	@find \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' -o -name '*.symvers' -o -name '*.order' \
                -o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' -o -name '.tmp_versions' \) \
                -print | xargs rm -rf

.PHONY: clean romfs

