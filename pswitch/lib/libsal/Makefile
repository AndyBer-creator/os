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
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

ifdef CONFIG_SYS_FW_SIGNATURE_CHECK
include $(LOADERDIR)/.config
CFLAGS += -DCONFIG_IH_MAGIC_NUMBER=0x$(CONFIG_IH_MAGIC_NUMBER)
endif

CFLAGS  += -Werror -Wall -Wuninitialized -Wstrict-prototypes -Wno-strict-aliasing -Wno-format-zero-length  -Werror=old-style-declaration -Werror=ignored-qualifiers -O3 -Os -g
#-Wformat-security
INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)
INCLUDE += -I$(LOADERDIR)/include/pswitch -I$(SYS_SSLINCLUDE) -I$(SYS_GLUEINCLUDE) -I$(FLASH_INCDIR)

# target
LIBNAME  = libsal.so
TARGET  = obj

OBJS_y :=
OBJS_m :=

OBJS_y +=  $(TARGET)/sys_error.o \
           $(TARGET)/sys_list.o \
           $(TARGET)/sys_def.o \
           $(TARGET)/sys_l2.o \
           $(TARGET)/sys_l3.o \
           $(TARGET)/sys_log.o \
           $(TARGET)/sys_log_desc.o \
           $(TARGET)/sys_dbg.o	\
           $(TARGET)/sys_dbg_desc.o \
           $(TARGET)/sys_util.o \
           $(TARGET)/sys_vlan.o \
           $(TARGET)/sys_portview.o \
           $(TARGET)/time.o \

OBJS_y  += $(TARGET)/sal_init.o \
           $(TARGET)/sal_sys.o \
           $(TARGET)/sal_sys_fw.o \
           $(TARGET)/sal_sys_auth.o \
           $(TARGET)/sal_sys_line.o\
           $(TARGET)/sal_util.o \
           $(TARGET)/sal_util_crc32.o \
           $(TARGET)/sal_util_crypt.o \
           $(TARGET)/sal_util_lock.o \
           $(TARGET)/sal_vlan.o \
           $(TARGET)/sal_port.o \
           $(TARGET)/sal_trunk.o \
           $(TARGET)/sal_trunk_chk.o \
           $(TARGET)/sal_qos.o \
           $(TARGET)/sal_mib.o \
           $(TARGET)/sal_switch.o \
           $(TARGET)/sal_l2.o \
           $(TARGET)/sal_l3.o \
           $(TARGET)/sal_l3_intf.o \
           $(TARGET)/sal_l3_route.o \
           $(TARGET)/sal_tftp.o \
           $(TARGET)/sal_rate.o \
           $(TARGET)/sal_led.o \
           $(TARGET)/sal_optical.o \
           $(TARGET)/sal_time.o \
		   $(TARGET)/sal_mib_private.o \
		   $(TARGET)/sal_socket.o \

OBJS_$(CONFIG_SYS_PORT_SECURITY)        += $(TARGET)/sal_psecure.o
OBJS_$(CONFIG_SYS_FIBER)                += $(TARGET)/sal_fiber.o
OBJS_$(CONFIG_SYS_USER_DEFINED_ACL)     += $(TARGET)/sal_acl_usr.o
OBJS_$(CONFIG_SYS_PROTO_AUTHMGR)        += $(TARGET)/sal_authmgr.o
OBJS_$(CONFIG_SYS_PROTO_AUTHMGR)        += $(TARGET)/sal_authmgr_dot1x.o
OBJS_$(CONFIG_SYS_PROTO_AUTHMGR)        += $(TARGET)/sal_authmgr_macauth.o
OBJS_$(CONFIG_SYS_PROTO_AUTHMGR)        += $(TARGET)/sal_authmgr_webauth.o
OBJS_$(CONFIG_SYS_EEE)                  += $(TARGET)/sal_eee.o
OBJS_$(CONFIG_SYS_MIRROR)               += $(TARGET)/sal_mirror.o
OBJS_$(CONFIG_SYS_SYSLOG)               += $(TARGET)/sal_syslog.o
OBJS_$(CONFIG_SYS_UI_SNMP)              += $(TARGET)/sal_snmp.o
OBJS_$(CONFIG_SYS_MIB_RMON_MGMT_UI)     += $(TARGET)/sal_rmon.o
OBJS_$(CONFIG_SYS_PROTO_LACP)           += $(TARGET)/sal_lacp.o
OBJS_$(CONFIG_SYS_PROTO_STP)            += $(TARGET)/sal_stp.o
OBJS_$(CONFIG_SYS_PROTO_LLDP)           += $(TARGET)/sal_lldp.o
OBJS_$(CONFIG_SYS_PROTO_LLDP_MED)       += $(TARGET)/sal_lldp_med.o
OBJS_$(CONFIG_SYS_PROTO_IGMP_SNOOPING)	+= $(TARGET)/sal_igmp.o $(TARGET)/sal_ipmcast.o
OBJS_$(CONFIG_SYS_PROTO_MLD_SNOOPING)	+= $(TARGET)/sal_mld.o
OBJS_$(CONFIG_SYS_DOS_PROTECTION)   	+= $(TARGET)/sal_dos.o
OBJS_$(CONFIG_SYS_APP_SSH)              += $(TARGET)/sal_ssh.o
OBJS_$(CONFIG_SYS_AAA)                  += $(TARGET)/sal_aaa.o
OBJS_$(CONFIG_SYS_PROTO_ISG)            += $(TARGET)/sal_isg.o
OBJS_$(CONFIG_SYS_PROTO_DHCPSNOOPING)   += $(TARGET)/sal_dhcp_snooping.o
OBJS_$(CONFIG_SYS_PROTO_DAI)            += $(TARGET)/sal_dai.o
OBJS_$(CONFIG_SYS_LIB_RADIUS)           += $(TARGET)/sal_radius.o
OBJS_$(CONFIG_SYS_LIB_TACPLUS)          += $(TARGET)/sal_tacplus.o
OBJS_$(CONFIG_SYS_PROTO_GVRP)           += $(TARGET)/sal_gvrp.o
OBJS_$(CONFIG_SYS_PROTO_GARP)           += $(TARGET)/sal_garp.o
OBJS_$(CONFIG_SYS_CABLE_DIAG)        	+= $(TARGET)/sal_cablediag.o
OBJS_$(CONFIG_SYS_MGMT_ACL)             += $(TARGET)/sal_acl_mgmt.o
OBJS_$(CONFIG_SYS_VOICEVLAN)            += $(TARGET)/sal_vlan_voice.o
OBJS_$(CONFIG_SYS_PROTOCOL_VLAN)        += $(TARGET)/sal_vlan_proto.o
OBJS_$(CONFIG_SYS_IP_SUBNET)            += $(TARGET)/sal_vlan_ip_subnet.o
OBJS_$(CONFIG_SYS_PROTO_UDLD)           += $(TARGET)/sal_udld.o
OBJS_$(CONFIG_SYS_MACVLAN)              += $(TARGET)/sal_vlan_mac.o
OBJS_$(CONFIG_SYS_SURVEILLANCEVLAN)     += $(TARGET)/sal_vlan_surveillance.o
OBJS_$(CONFIG_SYS_PROTO_ERPS)           += $(TARGET)/sal_erps.o
OBJS_$(CONFIG_SYS_PROTO_LOOPBACK)       += $(TARGET)/sal_loopback.o
OBJS_$(CONFIG_SYS_TECH_SUPPORT)         += $(TARGET)/sal_sys_tech.o
OBJS_$(CONFIG_SYS_POE)                  += $(TARGET)/sal_poe.o
OBJS_$(CONFIG_SYS_L3_ROUTE)             += $(TARGET)/sal_l3_neigh.o
OBJS_$(CONFIG_SYS_DHCP_SERVER)          += $(TARGET)/sal_dhcp_server.o
OBJS_$(CONFIG_SYS_L3_ROUTE_RIP)         += $(TARGET)/sal_l3_route_rip.o
OBJS_$(CONFIG_SYS_L3_ROUTE_ZEBRA)       += $(TARGET)/sal_l3_route_zebra.o
OBJS_$(CONFIG_SYS_L3_ROUTE_OSPF)        += $(TARGET)/sal_l3_route_ospf.o
OBJS_$(CONFIG_SYS_L3_ROUTE_BGP)         += $(TARGET)/sal_l3_route_bgp.o
OBJS_$(CONFIG_SYS_APP_EASYCWMP)         += $(TARGET)/sal_easycwmp.o
OBJS_$(CONFIG_SYS_L3_VRRP)              += $(TARGET)/sal_l3_vrrp.o

OBJS := $(OBJS_y) $(OBJS_m)

.PHONY: clean romfs prerequisite

all: prerequisite

$(LIBNAME):  $(OBJS)
	$(CC) $(LIB) -shared -Wl,-soname,$(LIBNAME).0 -o $(LIBNAME).0.0 $(OBJS)
	ln -sf $(LIBNAME).0.0 $(LIBNAME).0
	ln -sf $(LIBNAME).0 $(LIBNAME)
	$(SYSINSTALL) $(LIBNAME)*

$(TARGET)/%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) $(CFLAGS_EXTRA) -fPIC -c -o $@ $<

romfs:
	$(SYSROMFSINST) $(LIBNAME).0.0 /lib
	ln -sf $(LIBNAME).0.0 $(ROMFSDIR)/lib/$(LIBNAME).0
	ln -sf $(LIBNAME).0 $(ROMFSDIR)/lib/$(LIBNAME)

clean:
	rm -f $(shell for i in $(SRCS_SYS); do n=`basename $$i`; echo -n $${n%%.*}.c ""; done)
	rm -f $(LIBNAME)* $(TARGET)/*

SRCS_SYS = $(SYS_PLAT_COMMONDIR)/sys_error.c \
           $(SYS_PLAT_COMMONDIR)/sys_list.c \
           $(SYS_PLAT_COMMONDIR)/sys_def.c \
           $(SYS_PLAT_COMMONDIR)/sys_l2.c \
           $(SYS_PLAT_COMMONDIR)/sys_l3.c \
           $(SYS_PLAT_COMMONDIR)/sys_log.c \
           $(SYS_PLAT_COMMONDIR)/sys_log_desc.c \
           $(SYS_PLAT_COMMONDIR)/sys_dbg.c \
           $(SYS_PLAT_COMMONDIR)/sys_dbg_desc.c \
           $(SYS_PLAT_COMMONDIR)/sys_util.c \
           $(SYS_PLAT_COMMONDIR)/sys_vlan.c \
           $(SYS_PLAT_COMMONDIR)/sys_portview.c \
           $(SYS_PLAT_LX_OSALDIR)/time.c

prerequisite:
	if [ ! -e $(LIBNAME) ]; then \
		for i in $(SRCS_SYS); do n=`basename $$i`; ln -sf $$i $${n%%.*}.c; done; \
	fi
	rm -f $(LIBNAME)*
	make $(LIBNAME)
