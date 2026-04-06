#
#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************

#
# Current directory build
#
PSWITCH_DIR = $(shell pwd)/../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

#
# Define build variables
#
TARGET_DIR=frr-2.0
TARGET_MAKEFILE=$(TARGET_DIR)/Makefile
INSTALL_DIR=install

CFLAGS+=-I$(PSWITCH_DIR)/lib/ -g
LDFLAGS+=-L$(PSWITCH_DIR)/lib/readline
LDFLAGS+=-L$(PSWITCH_DIR)/lib/ncurses/lib
LDFLAGS+=-L$(PSWITCH_DIR)/lib/json-c -ljson-c
LDFLAGS+=-L$(PSWITCH_DIR)/lib/libipc -lipc

HOST_TYPE=--host=mips-linux
DISABLE_MOD=--disable-doc --disable-largefile --disable-shared \
            --disable-ripngd --disable-bgpd --disable-irdp \
            --disable-ospf6d --disable-ospfapi --disable-ospfclient \
            --disable-bgp-announce --disable-backtrace --disable-time-check --disable-rusage \
            --disable-pimd --disable-vtysh --enable-multipath=8 
ENABLE_MOD=--enable-static --enable-user=root --enable-group=root --enable-fpm
ifdef CONFIG_SYS_L3_ROUTE_RIP
ENABLE_MOD+=--enable-ripd 
else
DISABLE_MOD+=--disable-ripd
endif
ifdef CONFIG_SYS_L3_ROUTE_OSPF
ENABLE_MOD+=--enable-ospfd 
else
DISABLE_MOD+=--disable-ospfd
endif

CONFIG_OPTS=$(HOST_TYPE) $(DISABLE_MOD) $(ENABLE_MOD)

#
# Define build flags
#

#
# Define build directories
#

#
# Define build targets
#

all:
	@if [ ! -e $(TARGET_MAKEFILE) ]; then \
		cd $(TARGET_DIR); ./configure $(CONFIG_OPTS); fi
	$(MAKE) -C $(TARGET_DIR)
romfs:
	cp $(TARGET_DIR)/zebra/zebra .
	$(SYSINSTALL) zebra
	$(SYSROMFSINST) /bin/zebra
	$(ROMFSINST) /usr/local/etc/zebra.conf
ifdef CONFIG_SYS_L3_ROUTE_RIP
	cp $(TARGET_DIR)/ripd/ripd .
	$(SYSINSTALL) ripd
	$(SYSROMFSINST) /bin/ripd
endif
ifdef CONFIG_SYS_L3_ROUTE_OSPF
	cp $(TARGET_DIR)/ospfd/ospfd .
	$(SYSINSTALL) ospfd
	$(SYSROMFSINST) /bin/ospfd
endif	

distclean:
	$(MAKE) -C $(TARGET_DIR) distclean
	
clean: 
	rm -f zebra
ifdef CONFIG_SYS_L3_ROUTE_RIP
	rm -f ripd
endif
ifdef CONFIG_SYS_L3_ROUTE_OSPF
	rm -f ospfd
endif
	$(MAKE) -C $(TARGET_DIR) distclean

devclean:
	$(MAKE) -C $(TARGET_DIR) clean

