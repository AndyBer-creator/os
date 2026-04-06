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
ifndef PSWITCH_DIR
PSWITCH_DIR = $(shell pwd)/../../../../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
endif

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
CGIBINDIR = $(WEBDIR)/cgi-bin
LIBDIR = lib

#
# Define build flags
#
FLTFLAGS += -s 262143
CFLAGS += -Werror -Wall -Wuninitialized -Wstrict-prototypes -Wstrict-prototypes -g
CFLAGS_y = -lm

CFLAGS_EXTRA = $(CFLAGS_y) $(CFLAGS_m)

INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_APPINCLUDE)/webui/cgi -I$(SYS_PROTOINCLUDE)
INCLUDE += -I$(LOADERDIR)/include
LIB += -lsal -lfds -lcfg -lcfgtbl -lcmd -lclicmd -lglue -lvty -lreadline -lcrypt -lcJSON -lcgiutil -L$(SYS_INSTDIR)
ifdef CONFIG_SYS_LIB_TACPLUS
LIB += -ltacplus
endif
ifdef CONFIG_SYS_LIB_RADIUS
LIB += -lradius
endif
ifneq ($(CONFIG_RRCP_MASTER),)
LIB += -L$(APP_LIB_INCLUDE)/rrcp -lrrcp
endif
LIB += -lssl
LIB += -lcrypto

ifdef CONFIG_SYS_LIB_CUSTOM
INCLUDE += -I$(SYS_VENDOR_LIBDIR) -I$(SYS_VENDOR_MODDIR)
LIB     += -lcustom
endif
ifdef CONFIG_SYS_LIB_IPC
LIB     += -lipc
endif

ifdef CONFIG_SYS_LIB_UCI
LIB += -luci
endif

ifdef CONFIG_SYS_LIB_UBOX
LIB += -lubox
endif

ifdef CONFIG_SYS_FW_SIGNATURE_CHECK
include $(LOADERDIR)/.config
CFLAGS += -DCONFIG_IH_MAGIC_NUMBER=0x$(CONFIG_IH_MAGIC_NUMBER)
endif

#
# Define build directories
#

#
# Define build sources
#
# SET CGI
SET_y = cgi_set.c cgi_error.c
SET_y += cgi_set_diag.c
SET_y += cgi_set_file.c
SET_y += cgi_set_home.c
SET_y += cgi_set_lag.c
SET_y += cgi_set_mac.c
SET_y += cgi_set_port.c
SET_y += cgi_set_qos.c
SET_y += cgi_set_sys.c
SET_y += cgi_set_time.c
SET_y += cgi_set_vlan.c
SET_$(CONFIG_SYS_AAA)                   += cgi_set_aaa.c
SET_$(CONFIG_SYS_USER_DEFINED_ACL)      += cgi_set_acl.c
SET_$(CONFIG_SYS_MGMT_ACL)              += cgi_set_acl_mgmt.c
SET_$(CONFIG_SYS_PROTO_AUTHMGR)       	+= cgi_set_authmgr.c
SET_$(CONFIG_SYS_PROTO_DAI)         	+= cgi_set_dai.c
SET_$(CONFIG_SYS_DOS_PROTECTION)        += cgi_set_dos.c
SET_$(CONFIG_SYS_PROTO_DHCPSNOOPING)    += cgi_set_ds.c
SET_$(CONFIG_SYS_PROTO_GVRP)            += cgi_set_gvrp.c
SET_$(CONFIG_SYS_PROTO_GARP)            += cgi_set_gmrp.c
SET_$(CONFIG_SYS_PROTO_ISG)         	+= cgi_set_ipsg.c
SET_$(CONFIG_SYS_L3_ROUTE)              += cgi_set_l3.c
SET_$(CONFIG_SYS_PROTO_LLDP)            += cgi_set_lldp.c
SET_$(CONFIG_SYS_PROTO_ERPS)            += cgi_set_erps.c
SET_$(CONFIG_SYS_SYSLOG) 	        += cgi_set_log.c
SET_$(CONFIG_SYS_PROTO_IGMP_SNOOPING)   += cgi_set_mcast.c
SET_$(CONFIG_SYS_PORT_SECURITY)         += cgi_set_psecure.c
SET_$(CONFIG_SYS_LIB_RADIUS)            += cgi_set_radius.c
SET_$(CONFIG_SYS_MIB_RMON_MGMT_UI)      += cgi_set_rmon.c
SET_$(CONFIG_SYS_UI_SNMP)		+= cgi_set_snmp.c
SET_$(CONFIG_SYS_STORM_CTRL)            += cgi_set_storm.c
SET_$(CONFIG_SYS_PROTO_STP)             += cgi_set_stp.c
SET_$(CONFIG_SYS_LIB_TACPLUS)           += cgi_set_tacplus.c
SET_$(CONFIG_SYS_PROTO_UDLD)            += cgi_set_udld.c
SET_$(CONFIG_SYS_MACVLAN)               += cgi_set_vlan_mac.c
SET_$(CONFIG_SYS_PROTOCOL_VLAN)         += cgi_set_vlan_protocol.c
SET_$(CONFIG_SYS_IP_SUBNET)             += cgi_set_vlan_ip_subnet.c
SET_$(CONFIG_SYS_SURVEILLANCEVLAN)      += cgi_set_vlan_surveillance.c
SET_$(CONFIG_SYS_VOICEVLAN)             += cgi_set_vlan_voice.c
SET_$(CONFIG_SYS_POE)                   += cgi_set_poe.c
SET_$(CONFIG_SYS_DHCP_SERVER)           += cgi_set_dr.c
SET_$(CONFIG_SYS_L3_ROUTE_RIP)          += cgi_set_rip.c
SET_$(CONFIG_SYS_L3_ROUTE_OSPF)         += cgi_set_ospf.c
SET_$(CONFIG_SYS_L3_VRRP)               += cgi_set_vrrp.c
SET_CGI_SRC = $(SET_y) $(SET_m)

# GET CGI
GET_y = cgi_get.c
GET_y += cgi_get_diag.c
GET_y += cgi_get_file.c
GET_y += cgi_get_home.c
GET_y += cgi_get_lag.c
GET_y += cgi_get_mac.c
GET_y += cgi_get_panel.c
GET_y += cgi_get_port.c
GET_y += cgi_get_qos.c
GET_y += cgi_get_sys.c
GET_y += cgi_get_time.c
GET_y += cgi_get_vlan.c
GET_$(CONFIG_SYS_AAA)                   += cgi_get_aaa.c
GET_$(CONFIG_SYS_USER_DEFINED_ACL)      += cgi_get_acl.c
GET_$(CONFIG_SYS_MGMT_ACL)              += cgi_get_acl_mgmt.c
GET_$(CONFIG_SYS_PROTO_AUTHMGR)         += cgi_get_authmgr.c
GET_$(CONFIG_SYS_PROTO_DAI) 		+= cgi_get_dai.c
GET_$(CONFIG_SYS_DOS_PROTECTION)        += cgi_get_dos.c
GET_$(CONFIG_SYS_PROTO_DHCPSNOOPING)    += cgi_get_ds.c
GET_$(CONFIG_SYS_PROTO_GVRP)            += cgi_get_gvrp.c
GET_$(CONFIG_SYS_PROTO_GARP)            += cgi_get_gmrp.c
GET_$(CONFIG_SYS_PROTO_ISG)     	+= cgi_get_ipsg.c
GET_$(CONFIG_SYS_L3_ROUTE)              += cgi_get_l3.c
GET_$(CONFIG_SYS_PROTO_LLDP)            += cgi_get_lldp.c
GET_$(CONFIG_SYS_SYSLOG)                += cgi_get_log.c
GET_$(CONFIG_SYS_PROTO_ERPS)        	+= cgi_get_erps.c
GET_$(CONFIG_SYS_PROTO_IGMP_SNOOPING)   += cgi_get_mcast.c
GET_$(CONFIG_SYS_PORT_SECURITY)         += cgi_get_psecure.c
GET_$(CONFIG_SYS_LIB_RADIUS)            += cgi_get_radius.c
GET_$(CONFIG_SYS_MIB_RMON_MGMT_UI)      += cgi_get_rmon.c
GET_$(CONFIG_SYS_UI_SNMP)               += cgi_get_snmp.c
GET_$(CONFIG_SYS_PROTO_STP)             += cgi_get_stp.c
GET_$(CONFIG_SYS_STORM_CTRL)            += cgi_get_storm.c
GET_$(CONFIG_SYS_LIB_TACPLUS)           += cgi_get_tacplus.c
GET_$(CONFIG_SYS_PROTO_UDLD)            += cgi_get_udld.c
GET_$(CONFIG_SYS_MACVLAN)               += cgi_get_vlan_mac.c
GET_$(CONFIG_SYS_PROTOCOL_VLAN)         += cgi_get_vlan_protocol.c
GET_$(CONFIG_SYS_IP_SUBNET)             += cgi_get_vlan_ip_subnet.c
GET_$(CONFIG_SYS_SURVEILLANCEVLAN)      += cgi_get_vlan_surveillance.c
GET_$(CONFIG_SYS_VOICEVLAN)             += cgi_get_vlan_voice.c
GET_$(CONFIG_SYS_POE)                   += cgi_get_poe.c
GET_$(CONFIG_SYS_DHCP_SERVER)           += cgi_get_dr.c
GET_$(CONFIG_SYS_L3_ROUTE_RIP)          += cgi_get_rip.c
GET_$(CONFIG_SYS_L3_ROUTE_OSPF)         += cgi_get_ospf.c
GET_$(CONFIG_SYS_L3_VRRP)               += cgi_get_vrrp.c

GET_CGI_SRC = $(GET_y) $(GET_m)

UPLOAD_CGI_SRC = httpupload.c
RESTORE_CFG_CGI_SRC = httprestorecfg.c
ifdef CONFIG_SYS_BACKUP_CONFIGURATION
UPLOAD_BAKCFG_CGI_SRC = httpuploadbakcfg.c
endif
UPLOAD_RUNCFG_CGI_SRC = httpuploadruncfg.c
LOGIN_CGI_SRC = login.c
UPLOAD_LOGO_CGI_SRC = httpuploadlogo.c
UPLOAD_USRCFG_CGI_SRC = httpuploadusrcfg.c

SET_CGI_OBJ = $(SET_CGI_SRC:.c=.o)
GET_CGI_OBJ = $(GET_CGI_SRC:.c=.o)
UPLOAD_CGI_OBJ = $(UPLOAD_CGI_SRC:.c=.o)
RESTORE_CFG_CGI_OBJ = $(RESTORE_CFG_CGI_SRC:.c=.o)
ifdef CONFIG_SYS_BACKUP_CONFIGURATION
UPLOAD_BAKCFG_CGI_OBJ = $(UPLOAD_BAKCFG_CGI_SRC:.c=.o)
endif
UPLOAD_RUNCFG_CGI_OBJ = $(UPLOAD_RUNCFG_CGI_SRC:.c=.o)
LOGIN_CGI_OBJ = $(LOGIN_CGI_SRC:.c=.o)
UPLOAD_LOGO_CGI_OBJ = $(UPLOAD_LOGO_CGI_SRC:.c=.o)
UPLOAD_USRCFG_CGI_OBJ = $(UPLOAD_USRCFG_CGI_SRC:.c=.o)

SET_CGI_BIN = set.cgi
GET_CGI_BIN = get.cgi
UPLOAD_CGI_BIN = httpupload.cgi
RESTORE_CFG_CGI_BIN = httprestorecfg.cgi
ifdef CONFIG_SYS_BACKUP_CONFIGURATION
UPLOAD_BAKCFG_CGI_BIN = httpuploadbakcfg.cgi
endif
UPLOAD_RUNCFG_CGI_BIN = httpuploadruncfg.cgi
LOGIN_CGI_BIN = login.cgi
UPLOAD_LOGO_CGI_BIN = httpuploadlogo.cgi
UPLOAD_USRCFG_CGI_BIN = httpuploadusrcfg.cgi

#
# Define build targets
#

cgilib:
	for i in $(LIBDIR) ; do make -C $$i all || exit $?; done

ifdef CONFIG_SYS_BACKUP_CONFIGURATION
all: cgilib $(SET_CGI_BIN) $(GET_CGI_BIN) $(UPLOAD_CGI_BIN) $(RESTORE_CFG_CGI_BIN) $(UPLOAD_BAKCFG_CGI_BIN) $(UPLOAD_RUNCFG_CGI_BIN) $(LOGIN_CGI_BIN) $(UPLOAD_LOGO_CGI_BIN)  $(UPLOAD_USRCFG_CGI_BIN) 
else
all: cgilib $(SET_CGI_BIN) $(GET_CGI_BIN) $(UPLOAD_CGI_BIN) $(RESTORE_CFG_CGI_BIN) $(UPLOAD_RUNCFG_CGI_BIN) $(LOGIN_CGI_BIN) $(UPLOAD_LOGO_CGI_BIN) $(UPLOAD_USRCFG_CGI_BIN) 
endif

$(SET_CGI_BIN): $(SET_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(SET_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@

$(GET_CGI_BIN): $(GET_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(GET_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@

$(UPLOAD_CGI_BIN): $(UPLOAD_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(UPLOAD_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@

$(RESTORE_CFG_CGI_BIN): $(RESTORE_CFG_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(RESTORE_CFG_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@
ifdef CONFIG_SYS_BACKUP_CONFIGURATION
$(UPLOAD_BAKCFG_CGI_BIN): $(UPLOAD_BAKCFG_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(UPLOAD_BAKCFG_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@
endif
$(UPLOAD_RUNCFG_CGI_BIN): $(UPLOAD_RUNCFG_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(UPLOAD_RUNCFG_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@
$(LOGIN_CGI_BIN): $(LOGIN_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(LOGIN_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@
$(UPLOAD_LOGO_CGI_BIN): $(UPLOAD_LOGO_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(UPLOAD_LOGO_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@
$(UPLOAD_USRCFG_CGI_BIN): $(UPLOAD_USRCFG_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(UPLOAD_USRCFG_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@
%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) -c $< $(INCLUDE) -o $@

romfs:
	for i in $(LIBDIR) ; do make -C $$i $@ || exit $?; done

clean:
	for i in $(LIBDIR) ; do make -C $$i clean || exit $?; done
	rm -f *.o *.cgi *.gdb *.elf *.elf2flt

tgz: clean
	cd ..; tar zcvf webcgi.tgz webcgi

