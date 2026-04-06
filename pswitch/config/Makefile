ifndef PSWITCH_DIR
include ../../kernel/uClinux/.config
endif

include ../../version.mk

############################################################################

all:

clean:
	$(MAKE) -C $(SYS_SCRIPTSDIR) clean
	$(MAKE) -C $(SYS_SCRIPTSDIR)/lxdialog clean
	@rm -f config.tk autoconf.h .config .config.old $(SYS_COMMONDIR)/sys_autoconf.h .menuconfig.log

############################################################################
#
# The config stuff
#

ifeq ("x$(CONFIG_SHELL)","x")
CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
		else if [ -x /bin/bash ]; then echo /bin/bash; \
		else echo sh; fi ; fi)
endif
ifeq ($(CONFIG_LINUXDIR), linux-2.6.x)
SYS_KERNEL_VER := CONFIG_SYS_KERNEL_LINUX_2_6_X
else
SYS_KERNEL_VER := CONFIG_SYS_KERNEL_LINUX_3_18_24_X
endif
ifeq ($(CONFIG_LOADER_PSWITCH_UBOOT), y)
SYS_LOADER_VER := CONFIG_SYS_LOADER_UBOOT
else
SYS_LOADER_VER := CONFIG_SYS_LOADER_UBOOT2011
endif

SYS_COMMONDIR   := ../plat/common
SYS_SCRIPTSDIR  := scripts

ifeq ($(wildcard $(SYS_VENDOR_ROOTDIR)/config.in), $(SYS_VENDOR_ROOTDIR)/config.in)
SYS_CONFIG_IN   := $(SYS_VENDOR_ROOTDIR)/config.in
else
SYS_CONFIG_IN   := config.in
endif

config.tk: Makefile config.in $(SYS_SCRIPTSDIR)/header.tk $(SYS_SCRIPTSDIR)/tail.tk \
		$(wildcard $(ROOTDIR)/prop/config.in)
	$(MAKE) -C $(SYS_SCRIPTSDIR) tkparse
	ARCH=m68knommu $(SYS_SCRIPTSDIR)/tkparse < config.in > config.tmp
	@if [ -f /usr/local/bin/wish ];	then \
		echo '#!'"/usr/local/bin/wish -f" > config.tk; \
	else \
		echo '#!'"/usr/bin/wish -f" > config.tk; \
	fi
	cat $(SYS_SCRIPTSDIR)/header.tk >> ./config.tk
	cat config.tmp >> config.tk
	rm -f config.tmp
	echo "set defaults \"defconfig\"" >> config.tk
	echo "set autoconf_file \"autoconf.h\"" >> config.tk
	echo "set help_file \"Configure.help\"" >> config.tk
	cat $(SYS_SCRIPTSDIR)/tail.tk >> config.tk
	chmod 755 config.tk

xconfig: config.tk
	@$(CONFIG_SHELL) -n $(SYS_CONFIG_IN)
	wish -f config.tk

oldconfig:
	@$(CONFIG_SHELL) -n $(SYS_CONFIG_IN)
	@HELP_FILE=Configure.help \
		AUTOCONF_FILE=autoconf.h \
		$(CONFIG_SHELL) $(SYS_SCRIPTSDIR)/Configure -d $(SYS_CONFIG_IN)

config:
	@$(CONFIG_SHELL) -n $(SYS_CONFIG_IN)
	@HELP_FILE=Configure.help \
		AUTOCONF_FILE=autoconf.h \
		$(CONFIG_SHELL) $(SYS_SCRIPTSDIR)/Configure $(SYS_CONFIG_IN)
	@cp autoconf.h $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define $(SYS_KERNEL_VER) 1" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define $(SYS_LOADER_VER) 1" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define PRODUCT_VERSION \"$(PRODUCT_VERSION)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define PLATFORM_NAME \"$(PLATFORM_NAME)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define COPYRIGHT \"$(COPYRIGHT)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define VENDOR_URL \"$(VENDOR_URL)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h

menuconfig:
	@$(CONFIG_SHELL) -n $(SYS_CONFIG_IN)
	make -C $(SYS_SCRIPTSDIR)/lxdialog all
	@HELP_FILE=Configure.help \
		AUTOCONF_FILE=autoconf.h \
			$(CONFIG_SHELL) $(SYS_SCRIPTSDIR)/Menuconfig $(SYS_CONFIG_IN)
	@cp autoconf.h $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define $(SYS_KERNEL_VER) 1" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define $(SYS_LOADER_VER) 1" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define PRODUCT_VERSION \"$(PRODUCT_VERSION)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define PLATFORM_NAME \"$(PLATFORM_NAME)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define COPYRIGHT \"$(COPYRIGHT)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h
	echo "#define VENDOR_URL \"$(VENDOR_URL)\"" >> $(SYS_COMMONDIR)/sys_autoconf.h

############################################################################

