#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(CURDIR)/../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(SYS_ROOTDIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

ifdef CONFIG_SYS_RTL8380
LIB_FILE=libg_8382
endif

ifdef CONFIG_SYS_RTL9300
LIB_FILE=libg_9301
endif

ifdef CONFIG_SYS_RTL9310
ifdef CONFIG_SYS_L3_ROUTE_OSPF
LIB_FILE=libg_9311_l3
else
LIB_FILE=libg_9311_l2
endif
endif

LIBNAME   = libglue.so

all: 
	cp -f $(LIB_FILE) $(LIBNAME).0.0
	ln -sf $(LIBNAME).0.0 $(LIBNAME).0
	ln -sf $(LIBNAME).0 $(LIBNAME)
	$(SYSINSTALL) $(LIBNAME)*

romfs:
	$(SYSROMFSINST) $(LIBNAME).0.0 /lib
	ln -sf $(LIBNAME).0.0 $(ROMFSDIR)/lib/$(LIBNAME).0
	ln -sf $(LIBNAME).0 $(ROMFSDIR)/lib/$(LIBNAME)

clean:
	rm -f $(LIBNAME)
	rm -f $(LIBNAME).0.0 $(LIBNAME).0

