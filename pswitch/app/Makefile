#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(shell pwd)/..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(SYS_ROOTDIR)/make.config.mk

dir_y   = mountfs bootlogd handler custom timed iproute2
dir_$(CONFIG_SYS_UI_CLI)		+= cli
dir_$(CONFIG_SYS_UI_SNMP)		+= snmp
dir_$(CONFIG_SYS_UI_WEB)		+= webui
dir_$(CONFIG_SYS_APP_HTTPD)		+= httpd
dir_$(CONFIG_SYS_APP_INETD)		+= inetd
dir_$(CONFIG_SYS_APP_INITD)		+= initd
dir_$(CONFIG_SYS_APP_OPENSSL)		+= openssl
dir_$(CONFIG_SYS_APP_POLLD)		+= polld
dir_$(CONFIG_SYS_APP_SSH)		+= ssh
dir_$(CONFIG_SYS_APP_TELNETD)		+= telnetd
dir_$(CONFIG_SYS_APP_MSNTP)		+= msntp
dir_$(CONFIG_SYS_APP_DHCP6C)		+= wide-dhcpv6
dir_$(CONFIG_SYS_APP_DHCPC)		+= dhcpcd
dir_$(CONFIG_SYS_APP_KSID)		+= ksid
dir_$(CONFIG_SYS_L3_ROUTE)		+= frr
dir_$(CONFIG_SYS_DHCP_SERVER)	+= dhcpserver
dir_$(CONFIG_SYS_APP_EASYCWMP)	+= easycwmp
dir_$(CONFIG_SYS_L3_VRRP) += vrrpd
dir_y += fpmd   monitor 

#
# Define build targets
#

all:
	for i in $(dir_y)  ; do \
		$(MAKE) -C $$i || exit $? ; \
	done

romfs:
	for i in $(dir_y)  ; do \
		$(MAKE) -C $$i romfs || exit $? ; \
	done

clean:
	for i in $(dir_y)  ; do \
		$(MAKE) -C $$i clean ; \
	done
