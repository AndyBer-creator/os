#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR := $(shell pwd)

include $(PSWITCH_DIR)/make.root.linux.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.path.mk

ifeq ($(PRODUCT_VERSION),)
include $(PSWITCH_DIR)/../version.mk
endif

TARGET_BUILD := lkm lib app
TARGET_PHONY := $(TARGET_BUILD)
ROMFS_MODULE_EXTRA       = $(SYSROMFSDIR)/lib/modules/3.18.24/extra/

.PHONY : all menuconfig clean distclean $(TARGET_PHONY)

all: $(TARGET_BUILD) romfs

lkm:
	make -C $(SYS_PLAT_LXDIR)

lib:
	make -C $(SYS_LIBDIR)

app:
	make -C $(SYS_APPDIR)

menuconfig:
	make -C $(SYS_CFGDIR) menuconfig

romfs:
	make -C $(SYS_PLAT_LXDIR) romfs
	make -C $(SYS_LIBDIR) romfs
	make -C $(SYS_APPDIR) romfs
	@mkdir -p $(SYSROMFSDIR)/lib/modules/3.18.24/extra/
	@cp -f $(shell ls $(SYS_INSTDIR)/*.ko | grep -Ev 'board|custom|defdb') $(SYSROMFSDIR)/lib/modules/3.18.24/extra/;
	@if egrep "^CONFIG_MODULES=y" $(ROOTDIR)/$(LINUXDIR)/.config > /dev/null; then \
		find $(SYSROMFSDIR)/$(ROMFS_LX) -type f -name "*o" | xargs -r $(STRIP) -R .comment -R .note -g --strip-unneeded; \
        fi
	@if [ -e $(SYS_INSTDIR)/libcfg.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libcfg.so.0.0 /lib; \
		ln -sf libcfg.so.0.0 $(ROMFSDIR)/lib/libcfg.so.0; \
		ln -sf libcfg.so.0 $(ROMFSDIR)/lib/libcfg.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libcfgtbl.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libcfgtbl.so.0.0 /lib; \
		ln -sf libcfgtbl.so.0.0 $(ROMFSDIR)/lib/libcfgtbl.so.0; \
		ln -sf libcfgtbl.so.0 $(ROMFSDIR)/lib/libcfgtbl.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libclicmd.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libclicmd.so.0.0 /lib; \
		ln -sf libclicmd.so.0.0 $(ROMFSDIR)/lib/libclicmd.so.0; \
		ln -sf libclicmd.so.0 $(ROMFSDIR)/lib/libclicmd.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libcmd.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libcmd.so.0.0 /lib; \
		ln -sf libcmd.so.0.0 $(ROMFSDIR)/lib/libcmd.so.0; \
		ln -sf libcmd.so.0 $(ROMFSDIR)/lib/libcmd.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libsal.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libsal.so.0.0 /lib; \
		ln -sf libsal.so.0.0 $(ROMFSDIR)/lib/libsal.so.0; \
		ln -sf libsal.so.0 $(ROMFSDIR)/lib/libsal.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libfds.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libfds.so.0.0 /lib; \
		ln -sf libfds.so.0.0 $(ROMFSDIR)/lib/libfds.so.0; \
		ln -sf libfds.so.0 $(ROMFSDIR)/lib/libfds.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libglue.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libglue.so.0.0 /lib; \
		ln -sf libglue.so.0.0 $(ROMFSDIR)/lib/libglue.so.0; \
		ln -sf libglue.so.0 $(ROMFSDIR)/lib/libglue.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libipc.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libipc.so.0.0 /lib; \
		ln -sf libipc.so.0.0 $(ROMFSDIR)/lib/libipc.so.0; \
		ln -sf libipc.so.0 $(ROMFSDIR)/lib/libipc.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libuci.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libuci.so.0.0 /lib; \
		ln -sf libuci.so.0.0 $(ROMFSDIR)/lib/libuci.so.0; \
		ln -sf libuci.so.0 $(ROMFSDIR)/lib/libuci.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libubox.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libubox.so.0.0 /lib; \
		ln -sf libubox.so.0.0 $(ROMFSDIR)/lib/libubox.so.0; \
		ln -sf libubox.so.0 $(ROMFSDIR)/lib/libubox.so; \
	fi
	@if [ -e $(SYS_INSTDIR)/libubus.so.0.0 ]; then \
		$(SYSROMFSINST) $(SYS_INSTDIR)/libubus.so.0.0 /lib; \
		ln -sf libubus.so.0.0 $(ROMFSDIR)/lib/libubus.so.0; \
		ln -sf libubus.so.0 $(ROMFSDIR)/lib/libubus.so; \
	fi
setup:
	if [ ! -L $(SYS_LIB_CLICMDDIR) ]; then \
                ln -snf $(SYS_VENDOR_CLICMDDIR) $(SYS_LIB_CLICMDDIR); fi
	if [ ! -L $(SYS_LIB_CUSDIR) ]; then \
                ln -snf $(SYS_VENDOR_LIBDIR) $(SYS_LIB_CUSDIR); fi
	if [ ! -L $(SYS_PLAT_LX_BOARDDIR)/vendor ]; then \
                ln -snf $(SYS_VENDOR_BOARDDIR) $(SYS_PLAT_LX_BOARDDIR)/vendor; fi
	if [ ! -L $(SYS_PLAT_LX_BOARDCONFDIR)/vendor ]; then \
                ln -snf $(SYS_VENDOR_BOARDDIR) $(SYS_PLAT_LX_BOARDCONFDIR)/vendor; fi
	if [ ! -L $(SYS_PLAT_LX_DEFDBDIR)/defdb_data.c ]; then \
                ln -snf $(SYS_VENDOR_DEFDBDIR)/defdb_data.c $(SYS_PLAT_LX_DEFDBDIR)/defdb_data.c; fi
	if [ ! -L $(SYS_PLAT_LX_DEFDBDIR)/defdb_data.h ]; then \
                ln -snf $(SYS_VENDOR_DEFDBDIR)/defdb_data.h $(SYS_PLAT_LX_DEFDBDIR)/defdb_data.h; fi
	if [ ! -L $(SYS_PLAT_LX_CUSDIR) ]; then \
        	ln -snf $(SYS_VENDOR_MODDIR) $(SYS_PLAT_LX_CUSDIR); fi
	if [ ! -d $(SYS_INSTDIR) ]; then \
		mkdir -p $(SYS_INSTDIR); fi
	if [ ! -L $(SYS_APP_CLIDIR) ]; then \
		ln -snf $(SYS_VENDOR_CLIDIR) $(SYS_APP_CLIDIR); fi
	if [ ! -L $(SYS_APP_WEBUIDIR) ]; then \
		ln -snf $(SYS_VENDOR_WEBUIDIR) $(SYS_APP_WEBUIDIR); fi
	if [ -e $(SYS_VENDOR_CUSTOMAPPDIR) ]; then \
		ln -snf $(SYS_VENDOR_CUSTOMAPPDIR) $(SYS_APP_CUSTOMDIR); fi
	if [ ! -L $(SYS_APP_SNMPVENDORDIR) ]; then \
		ln -snf $(SYS_VENDOR_SNMPDIR) $(SYS_APP_SNMPVENDORDIR); fi

clean:
	make -C $(SYS_APPDIR) clean
	make -C $(SYS_PLAT_LXDIR) clean
	make -C $(SYS_LIBDIR) clean
	

distclean: clean
	make -C $(SYS_CFGDIR) clean
	find \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' -o -name '*.symvers' -o -name '*.order' \
        	-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' -o -name '.tmp_versions' \) \
                -print | xargs rm -rf;
	rm -rf install
	rm -rf $(SYS_APP_CLIDIR) $(SYS_APP_WEBUIDIR) $(SYS_APP_CUSTOMDIR) $(SYS_LIB_CUSDIR) $(SYS_LIB_CLICMDDIR)
	rm -rf $(SYS_PLAT_LX_DEFDBDIR)/defdb_data.c $(SYS_PLAT_LX_DEFDBDIR)/defdb_data.h
	find $(SYS_APPDIR) $(SYS_PLATDIR) $(SYS_PROTODIR) $(SYS_VENDORDIR) -type l -print | xargs rm -f
	rm -rf app/httpd/src/boa_lexer.c
	rm -rf app/iproute2/ip/static-syms.h
	rm -rf app/openssl/progs.h
	rm -rf app/openssl/progs.pl
	rm -rf lib/libssl/apps/md4.c
	rm -rf lib/libssl/include/openssl
	rm -rf lib/libssl/test/bftest.c
	rm -rf lib/libssl/test/bntest.c
	rm -rf lib/libssl/test/casttest.c
	rm -rf lib/libssl/test/clienthellotest.c
	rm -rf lib/libssl/test/constant_time_test.c
	rm -rf lib/libssl/test/destest.c
	rm -rf lib/libssl/test/dhtest.c
	rm -rf lib/libssl/test/dsatest.c
	rm -rf lib/libssl/test/ecdhtest.c
	rm -rf lib/libssl/test/ecdsatest.c
	rm -rf lib/libssl/test/ectest.c
	rm -rf lib/libssl/test/enginetest.c
	rm -rf lib/libssl/test/evp_extra_test.c
	rm -rf lib/libssl/test/evp_test.c
	rm -rf lib/libssl/test/exptest.c
	rm -rf lib/libssl/test/heartbeat_test.c
	rm -rf lib/libssl/test/hmactest.c
	rm -rf lib/libssl/test/ideatest.c
	rm -rf lib/libssl/test/md4test.c
	rm -rf lib/libssl/test/md5test.c
	rm -rf lib/libssl/test/mdc2test.c
	rm -rf lib/libssl/test/randtest.c
	rm -rf lib/libssl/test/rc2test.c
	rm -rf lib/libssl/test/rc4test.c
	rm -rf lib/libssl/test/rmdtest.c
	rm -rf lib/libssl/test/rsa_test.c
	rm -rf lib/libssl/test/sha1test.c
	rm -rf lib/libssl/test/sha256t.c
	rm -rf lib/libssl/test/sha512t.c
	rm -rf lib/libssl/test/shatest.c
	rm -rf lib/libssl/test/srptest.c
	rm -rf lib/libssl/test/ssltest.c
	rm -rf lib/libssl/test/sslv2conftest.c
	rm -rf lib/libssl/test/v3nametest.c
	rm -rf lib/libssl/test/verify_extra_test.c
	rm -rf lib/libssl/test/wp_test.c

