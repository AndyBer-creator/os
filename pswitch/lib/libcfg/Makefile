#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR := $(CURDIR)/../..
include $(PSWITCH_DIR)/make.config.mk

# target
LIBNAME = libcfg.so

#OBJS_y or OBJS_m
OBJS_$(CONFIG_SYS_PORT_SECURITY)        += $(TARGET)/cfg_psecure.o
OBJS_$(CONFIG_SYS_UI_SNMP)              += $(TARGET)/cfg_snmp.o
OBJS_$(CONFIG_SYS_MIB_RMON_MGMT_UI)     += $(TARGET)/cfg_rmon.o
OBJS_$(CONFIG_SYS_PROTO_AUTHMGR)        += $(TARGET)/cfg_authmgr.o
OBJS_$(CONFIG_SYS_PROTO_STP)            += $(TARGET)/cfg_stp.o
OBJS_$(CONFIG_SYS_MIRROR)               += $(TARGET)/cfg_mirror.o
OBJS_$(CONFIG_SYS_PROTO_LACP)           += $(TARGET)/cfg_lacp.o
OBJS_$(CONFIG_SYS_PROTO_LLDP)           += $(TARGET)/cfg_lldp.o
OBJS_$(CONFIG_SYS_PROTO_LLDP_MED)       += $(TARGET)/cfg_lldp_med.o
OBJS_$(CONFIG_SYS_PROTO_IGMP_SNOOPING)  += $(TARGET)/cfg_igmp.o
OBJS_$(CONFIG_SYS_PROTO_MLD_SNOOPING)   += $(TARGET)/cfg_mld.o
OBJS_$(CONFIG_SYS_DOS_PROTECTION)       += $(TARGET)/cfg_dos.o
OBJS_$(CONFIG_SYS_AAA)                  += $(TARGET)/cfg_aaa.o
OBJS_$(CONFIG_SYS_PROTO_ISG)            += $(TARGET)/cfg_isg.o
OBJS_$(CONFIG_SYS_PROTO_DHCPSNOOPING)   += $(TARGET)/cfg_dhcp_snooping.o
OBJS_$(CONFIG_SYS_PROTO_DAI)            += $(TARGET)/cfg_dai.o
OBJS_$(CONFIG_SYS_LIB_RADIUS)           += $(TARGET)/cfg_radius.o
OBJS_$(CONFIG_SYS_LIB_TACPLUS)          += $(TARGET)/cfg_tacplus.o
OBJS_$(CONFIG_SYS_PROTO_GVRP)           += $(TARGET)/cfg_gvrp.o
OBJS_$(CONFIG_SYS_PROTO_GARP)           += $(TARGET)/cfg_garp.o
OBJS_$(CONFIG_SYS_VOICEVLAN)            += $(TARGET)/cfg_vlan_voice.o
OBJS_$(CONFIG_SYS_PROTOCOL_VLAN)        += $(TARGET)/cfg_vlan_proto.o
OBJS_$(CONFIG_SYS_IP_SUBNET)            += $(TARGET)/cfg_vlan_ip_subnet.o
OBJS_$(CONFIG_SYS_PROTO_UDLD)           += $(TARGET)/cfg_udld.o
OBJS_$(CONFIG_SYS_SURVEILLANCEVLAN)     += $(TARGET)/cfg_vlan_surveillance.o
OBJS_$(CONFIG_SYS_RLDP)                 += $(TARGET)/cfg_rldp.o
OBJS_$(CONFIG_SYS_PROTO_ERPS)           += $(TARGET)/cfg_erps.o
OBJS_$(CONFIG_SYS_PROTO_LOOPBACK)       += $(TARGET)/cfg_loopback.o
OBJS_$(CONFIG_SYS_POE)                  += $(TARGET)/cfg_poe.o
OBJS_$(CONFIG_SYS_L3_ROUTE)             += $(TARGET)/cfg_l3.o
OBJS_$(CONFIG_SYS_DHCP_SERVER)          += $(TARGET)/cfg_dhcp_server.o
OBJS_$(CONFIG_SYS_APP_EASYCWMP)         += $(TARGET)/cfg_easycwmp.o

OBJS_y += $(TARGET)/cfg.o \
          $(TARGET)/cfg_sys.o \
          $(TARGET)/cfg_vlan.o \
          $(TARGET)/cfg_port.o \
          $(TARGET)/cfg_switch.o \
          $(TARGET)/cfg_l2.o \
          $(TARGET)/cfg_trunk.o \
          $(TARGET)/cfg_qos.o \
          $(TARGET)/cfg_rate.o \
          $(TARGET)/cfg_init.o \
          $(TARGET)/cfg_time.o \
          $(TARGET)/cfg_syslog.o

# Put after OBJS
include ../Makefile.lib
