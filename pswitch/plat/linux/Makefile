PSWITCH_DIR = $(shell pwd)/../..
include $(PSWITCH_DIR)/make.path.mk
include $(SYS_ROOTDIR)/make.config.mk

dir_m += net/
dir_m += osal/
dir_$(CONFIG_SYS_PLAT_LXBOARD) += board_conf
dir_$(CONFIG_SYS_PLAT_LXBOARD) += board
dir_$(CONFIG_SYS_PLAT_LXKSI)   += ksi
dir_$(CONFIG_SYS_PLAT_LXDEFDB) += defdb
dir_$(CONFIG_SYS_PLAT_LXSKI)   += ski
dir_$(CONFIG_SYS_PLAT_CUSMOD)  += custom

dir_$(CONFIG_SYS_PROTO_AUTHMGR)         += ski/proto/authmgr
dir_$(CONFIG_SYS_PROTO_IGMP_SNOOPING)   += ski/proto/igmp
dir_$(CONFIG_SYS_PROTO_LACP)            += ski/proto/lacp
dir_$(CONFIG_SYS_VOICEVLAN)             += ski/proto/voice_vlan
dir_$(CONFIG_SYS_PROTO_LLDP)            += ski/proto/lldp
dir_$(CONFIG_SYS_PROTO_STP)             += ski/proto/stp
dir_$(CONFIG_SYS_PROTO_ISG)             += ski/proto/isg
dir_$(CONFIG_SYS_PROTO_DHCP)            += ski/proto/dhcp
dir_$(CONFIG_SYS_PROTO_DAI)             += ski/proto/dai
dir_$(CONFIG_SYS_PROTO_GVRP)            += ski/proto/gvrp
dir_$(CONFIG_SYS_PROTO_GARP)            += ski/proto/garp
dir_$(CONFIG_SYS_PROTO_UDLD)            += ski/proto/udld
dir_$(CONFIG_SYS_MACVLAN)               += ski/proto/macvlan
dir_$(CONFIG_SYS_SURVEILLANCEVLAN)      += ski/proto/surveillancevlan
dir_$(CONFIG_SYS_PROTO_ERPS)            += ski/proto/erps
dir_$(CONFIG_SYS_PROTO_LOOPBACK)        += ski/proto/loopback

all:
	for i in $(dir_m)  ; do \
		$(MAKE) -C $$i || exit $? ; \
	done

romfs:
	for i in $(dir_m)  ; do \
		$(MAKE) -C $$i romfs || exit $? ; \
	done

clean:
	for i in $(dir_m)  ; do \
		$(MAKE) -C $$i clean ; \
	done

.PHONY: romfs clean devclean all
