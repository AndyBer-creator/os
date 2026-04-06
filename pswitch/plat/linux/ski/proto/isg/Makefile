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

IPSGDIR := ../../../../../proto/isg
include $(SYS_PROTO_IPSGDIR)/Makefile

ifeq ($(CONFIG_SYS_PLAT_LXSKI),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_SKIDIR)/Module.symvers
endif

ifeq ($(CONFIG_SYS_PLAT_LXKSI),m)
EXTRA_CFLAGS += -I$(SYS_APP_KSIDDIR)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_KSIDIR)/Module.symvers
endif

ifeq ($(CONFIG_SYS_PLAT_LXDEFDB),m)
KBUILD_EXTRA_SYMBOLS += $(SYS_PLAT_LX_DEFDBDIR)/Module.symvers
endif
export KBUILD_EXTRA_SYMBOLS

ALL_OBJ      := $(IPSG_OBJ) ski_isg_lkm.o ski_isg_lkm_export.o ski_isg.o
TARGET = l2g_isg
obj-$(CONFIG_SYS_PROTO_ISG) += $(TARGET).o
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

