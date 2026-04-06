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

EXTRA_CFLAGS += -Wall -Werror
#-mlong-calls -fno-pic -mno-abicalls

SYMBOLPATH += $(SYS_INSTDIR)/Module.symvers

KRN_OBJ_y += mod_custom_example.o
KRN_OBJ_y += mod_custom_lldp.o
ALL_OBJ	:= ski_custom.o mod_custom.o $(KRN_OBJ_y)
TARGET = custom
obj-$(CONFIG_SYS_PLAT_CUSMOD) += $(TARGET).o
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
	@find \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' -o -name '*.order' \
		-o -name '*.symvers' -o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \) \
		-type f -print | xargs rm -f

