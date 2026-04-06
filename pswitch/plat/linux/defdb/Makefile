#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
ifndef PSWITCH_DIR
PSWITCH_DIR := $(CURDIR)/../../..
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
endif

EXTRA_CFLAGS += -Wall -Werror

ALL_OBJ := defdb_lkm.o defdb_data.o
TARGET = defdb
obj-$(CONFIG_SYS_PLAT_LXDEFDB) += $(TARGET).o
$(TARGET)-objs  := $(ALL_OBJ)
EXTRA_CFLAGS += -I$(SYS_LIBINCLUDE)

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

