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

ERPSDIR := ../../../../../proto/erps
include $(SYS_PROTO_ERPSDIR)/Makefile

EXTRA_CFLAGS += -Wall -Werror
#-mlong-calls -fno-pic -mno-abicalls

ifeq ($(CONFIG_SYS_PLAT_LXSKI),m)
SYMBOLPATH += $(SYS_PLAT_LX_SKIDIR)/Module.symvers
endif

ALL_OBJ      := $(ERPS_OBJ) ski_erps_lkm.o ski_erps_lkm_export.o ski_erps.o
TARGET = l2g_erps
obj-$(CONFIG_SYS_PROTO_ERPS) += $(TARGET).o
$(TARGET)-objs	:= $(ALL_OBJ)

all:
	rm -f Module.symvers;
	cat $(SYMBOLPATH) >> Module.symvers
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules
	$(SYSINSTALL) $(TARGET).ko

romfs:
	make -C $(ROOTDIR) M=$(CURDIR) $(EXTRA_CFLAGS) modules_install

clean:
	@$(RM) $(ALL_OBJ)
	@find \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \) \
		-type f -print | xargs rm -f

