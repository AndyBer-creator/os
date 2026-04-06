#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(CURDIR)/../../../../..
include $(PSWITCH_DIR)/make.config.mk

# target
LIBNAME  = libclicmd.so

OBJS_$(CONFIG_SYS_PROTO_AUTHMGR)        += $(TARGET)/cmd_authmgr.o
OBJS_$(CONFIG_SYS_PLAT_DBGMOD)          += $(TARGET)/cmd_debug.o
OBJS_$(CONFIG_SYS_PORT_SECURITY)        += $(TARGET)/cmd_psecure.o
OBJS_$(CONFIG_SYS_APP_SSH)              += $(TARGET)/cmd_ssh.o
OBJS_$(CONFIG_SYS_USER_DEFINED_ACL)     += $(TARGET)/cmd_acl_usr.o
OBJS_$(CONFIG_SYS_EEE)                  += $(TARGET)/cmd_eee.o
OBJS_$(CONFIG_SYS_MIRROR)               += $(TARGET)/cmd_mirror.o
OBJS_$(CONFIG_SYS_SYSLOG)               += $(TARGET)/cmd_syslog.o
OBJS_$(CONFIG_SYS_UI_SNMP)              += $(TARGET)/cmd_snmp.o
OBJS_$(CONFIG_SYS_MIB_RMON_MGMT_UI)     += $(TARGET)/cmd_rmon.o
OBJS_$(CONFIG_SYS_PROTO_STP)            += $(TARGET)/cmd_stp.o
OBJS_$(CONFIG_SYS_PROTO_LACP)           += $(TARGET)/cmd_lacp.o
OBJS_$(CONFIG_SYS_PROTO_LLDP)           += $(TARGET)/cmd_lldp.o
OBJS_$(CONFIG_SYS_PROTO_LLDP_MED)       += $(TARGET)/cmd_lldp_med.o
OBJS_$(CONFIG_SYS_PROTO_IGMP_SNOOPING)	+= $(TARGET)/cmd_igmp.o
OBJS_$(CONFIG_SYS_PROTO_MLD_SNOOPING)	+= $(TARGET)/cmd_mld.o
OBJS_$(CONFIG_SYS_AAA)                  += $(TARGET)/cmd_aaa.o
OBJS_$(CONFIG_SYS_PROTO_ISG)            += $(TARGET)/cmd_isg.o
OBJS_$(CONFIG_SYS_PROTO_DHCPSNOOPING)	+= $(TARGET)/cmd_dhcp_snooping.o
OBJS_$(CONFIG_SYS_PROTO_DAI)	        += $(TARGET)/cmd_dai.o
OBJS_$(CONFIG_SYS_DOS_PROTECTION)   	+= $(TARGET)/cmd_dos.o
OBJS_$(CONFIG_SYS_LIB_RADIUS)           += $(TARGET)/cmd_radius.o
OBJS_$(CONFIG_SYS_LIB_TACPLUS)          += $(TARGET)/cmd_tacplus.o
OBJS_$(CONFIG_SYS_PROTO_GVRP)	        += $(TARGET)/cmd_gvrp.o
OBJS_$(CONFIG_SYS_PROTO_GARP)	        += $(TARGET)/cmd_garp.o
OBJS_$(CONFIG_SYS_MGMT_ACL)             += $(TARGET)/cmd_acl_mgmt.o
OBJS_$(CONFIG_SYS_CABLE_DIAG)           += $(TARGET)/cmd_cablediag.o
OBJS_$(CONFIG_SYS_VOICEVLAN)            += $(TARGET)/cmd_vlan_voice.o
OBJS_$(CONFIG_SYS_PROTOCOL_VLAN)        += $(TARGET)/cmd_vlan_proto.o
OBJS_$(CONFIG_SYS_PROTO_UDLD)           += $(TARGET)/cmd_udld.o
OBJS_$(CONFIG_SYS_MACVLAN)              += $(TARGET)/cmd_vlan_mac.o
OBJS_$(CONFIG_SYS_SURVEILLANCEVLAN)     += $(TARGET)/cmd_vlan_surveillance.o
OBJS_$(CONFIG_SYS_IP_SUBNET)            += $(TARGET)/cmd_vlan_ip_subnet.o
OBJS_$(CONFIG_SYS_PROTO_ERPS)           += $(TARGET)/cmd_erps.o
OBJS_$(CONFIG_SYS_PROTO_LOOPBACK)       += $(TARGET)/cmd_loopback.o
OBJS_$(CONFIG_SYS_TECH_SUPPORT)         += $(TARGET)/cmd_sys_tech.o
OBJS_$(CONFIG_SYS_POE)                  += $(TARGET)/cmd_poe.o
OBJS_$(CONFIG_SYS_L3_ROUTE)		        += $(TARGET)/cmd_l3.o
OBJS_$(CONFIG_SYS_DHCP_SERVER)	        += $(TARGET)/cmd_dhcp_server.o
OBJS_$(CONFIG_SYS_L3_ROUTE_RIP)	        += $(TARGET)/cmd_l3_rip.o
OBJS_$(CONFIG_SYS_L3_ROUTE_ZEBRA)	    += $(TARGET)/cmd_l3_zebra.o
OBJS_$(CONFIG_SYS_L3_ROUTE_OSPF)	    += $(TARGET)/cmd_l3_ospf.o
OBJS_$(CONFIG_SYS_L3_ROUTE_BGP)	        += $(TARGET)/cmd_l3_bgp.o
OBJS_$(CONFIG_SYS_APP_EASYCWMP)	        += $(TARGET)/cmd_easycwmp.o
OBJS_$(CONFIG_SYS_L3_VRRP)	            += $(TARGET)/cmd_l3_vrrp.o

OBJS_y += $(TARGET)/cmd_util.o \
          $(TARGET)/cmd_tables.o \
          $(TARGET)/cmd_ext.o \
          $(TARGET)/cmd_ext_hidden.o\
          $(TARGET)/cmd_common.o \
          $(TARGET)/cmd_sys.o \
          $(TARGET)/cmd_sys_fs.o \
          $(TARGET)/cmd_sys_auth.o \
          $(TARGET)/cmd_sys_line.o \
          $(TARGET)/cmd_vlan.o \
          $(TARGET)/cmd_port.o \
          $(TARGET)/cmd_switch.o \
          $(TARGET)/cmd_l2.o \
          $(TARGET)/cmd_trunk.o \
          $(TARGET)/cmd_qos.o \
          $(TARGET)/cmd_rate.o \
          $(TARGET)/cmd_time.o \

# Put after OBJS
include $(PSWITCH_DIR)/lib/Makefile.lib
