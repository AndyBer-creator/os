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
PSWITCH_DIR = $(shell pwd)/../../../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

#
# Include architecture specific build rules
#

#
# Define build variables
#
ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
PREFIX = $(ROOTDIR)/sqfs/home
else
PREFIX = $(ROOTDIR)/romfs/home
endif
WEBDIR = $(PREFIX)/web
CGIBINDIR = $(WEBDIR)/cgi

#
# Define build flags
#

#
# Define build directories
#

#
# Define build sources
#

#
# Define build targets
#

all:
	make -C cgi all
ifdef CONFIG_SYS_PROTO_WEBAUTH
	$(MAKE) -C webauth
endif

rmdevfiles:
	rm -rf $(WEBDIR)/data
	rm -rf $(WEBDIR)/js/url_mockup.js

installweb:
	if test \! -e $(PREFIX); then mkdir $(PREFIX); fi
	if test \! -e $(WEBDIR); then mkdir $(WEBDIR); fi
	if test \! -e $(WEBDIR)/tmp; then ln -sf /webtmp $(WEBDIR)/tmp; fi
	if test \! -e $(WEBDIR)/images; then ln -sf /webtmp/web/images $(WEBDIR)/images; fi
	if test \! -e $(WEBDIR)/css; then ln -sf /webtmp/web/css $(WEBDIR)/css; fi
	if test \! -e $(WEBDIR)/html; then ln -sf /webtmp/web/html $(WEBDIR)/html; fi
	if test \! -e $(WEBDIR)/js; then ln -sf /webtmp/web/js $(WEBDIR)/js; fi
	if test \! -e $(WEBDIR)/lang; then ln -sf /webtmp/web/lang $(WEBDIR)/lang; fi
	if test \! -e $(WEBDIR)/home.html; then ln -sf /webtmp/web/home.html $(WEBDIR)/home.html; fi
	if test \! -e $(WEBDIR)/index.html; then ln -sf /webtmp/web/index.html $(WEBDIR)/index.html; fi
	if test \! -e $(WEBDIR)/login.html; then ln -sf /webtmp/web/login.html $(WEBDIR)/login.html; fi
	if test \! -e $(WEBDIR)/system-media-access-control-address-set.html; then ln -sf /webtmp/web/system-media-access-control-address-set.html $(WEBDIR)/system-media-access-control-address-set.html; fi
	if test \! -e $(WEBDIR)/system-device-and-sn-set.html; then ln -sf /webtmp/web/system-device-and-sn-set.html $(WEBDIR)/system-device-and-sn-set.html; fi
	if test \! -e $(WEBDIR)/orig-equip-manufact-info-set.html; then ln -sf /webtmp/web/orig-equip-manufact-info-set.html $(WEBDIR)/orig-equip-manufact-info-set.html; fi
	cp -rf web_org $(WEBDIR)

installcgi:
	if test \! -e $(PREFIX); then mkdir $(PREFIX); fi
	if test \! -e $(CGIBINDIR); then mkdir $(CGIBINDIR); fi
	if test \! -e $(WEBDIR)/cgi-bin; then mkdir $(WEBDIR)/cgi-bin; fi
	$(STRIP) --strip-unneeded cgi/*.cgi
	chmod 755 $(CGIBINDIR); cp cgi/*.cgi $(CGIBINDIR)
	mv $(CGIBINDIR)/httpupload.cgi $(WEBDIR)/cgi-bin/

install: installweb installcgi rmdevfiles

romfs: install
	make -C cgi romfs
ifdef CONFIG_SYS_PROTO_WEBAUTH
	$(MAKE) -C webauth romfs
endif

clean:
	make -C cgi clean
	rm -f *.o *.cgi *.gdb *.elf *.elf2flt
ifdef CONFIG_SYS_PROTO_WEBAUTH
	$(MAKE) -C webauth clean
endif

tgz: clean
	cd ..; tar zcvf webcgi.tgz webcgi

