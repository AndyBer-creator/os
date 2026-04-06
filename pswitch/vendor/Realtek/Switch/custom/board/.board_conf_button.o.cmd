cmd_/compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.o := rtk-ms-2.0.0-linux-gcc -Wp,-MD,/compile/realtek/pswitch/plat/linux/board_conf/vendor/.board_conf_button.o.d  -nostdinc -isystem /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/include -I./arch/mips/include -Iarch/mips/include/generated  -Iinclude -I./arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips -Iarch/mips/bsp/ -DRTL9300 -DRTK_UNIVERSAL_BSP -DRTK_LKM -I./arch/mips/include/asm/mach-generic -fno-delete-null-pointer-checks -I/compile/realtek/kernel/uClinux/../../sdk/include -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/system/linux -I/compile/realtek/kernel/uClinux/../../pswitch/plat -I/compile/realtek/kernel/uClinux/../../pswitch/plat/linux -I/compile/realtek/kernel/uClinux/../../pswitch/proto -O2 --param=allow-store-data-races=0 -Wframe-larger-than=4096 -fstack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -fno-var-tracking-assignments -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO -Wall -Werror -I/compile/realtek/kernel/uClinux/../../loader/u-boot-2011.12/include  -DMODULE -mlong-calls  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(board_conf_button)"  -D"KBUILD_MODNAME=KBUILD_STR(board_conf)" -c -o /compile/realtek/pswitch/plat/linux/board_conf/vendor/.tmp_board_conf_button.o /compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.c

source_/compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.o := /compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.c

deps_/compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.o := \
    $(wildcard include/config/sys/rtl8380.h) \
    $(wildcard include/config/sys/db/board.h) \
    $(wildcard include/config/sys/rtl8390.h) \
    $(wildcard include/config/sys/rtl9310.h) \
    $(wildcard include/config/sys/rtl9300.h) \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/board.h \
    $(wildcard include/config/sys/poe.h) \
    $(wildcard include/config/sys/uart1.h) \
    $(wildcard include/config/sys/board/rtc.h) \
    $(wildcard include/config/sys/board/fan.h) \
    $(wildcard include/config/sys/board/temp.h) \
    $(wildcard include/config/sys/snmp.h) \
    $(wildcard include/config/sys/watchdog.h) \
    $(wildcard include/config/sys/fiber.h) \
    $(wildcard include/config/sys/led.h) \
    $(wildcard include/config/sys/button.h) \
    $(wildcard include/config/sys/encrypt.h) \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_autoconf.h \
    $(wildcard include/config/sys/jumbo/frame.h) \
    $(wildcard include/config/sys/proto/lacp.h) \
    $(wildcard include/config/sys/proto/stp.h) \
    $(wildcard include/config/sys/proto/stp/mstp.h) \
    $(wildcard include/config/sys/proto/stp/define/open.h) \
    $(wildcard include/config/sys/stp/bpdu/filter.h) \
    $(wildcard include/config/sys/stp/selfloop/prevent.h) \
    $(wildcard include/config/sys/proto/loopback.h) \
    $(wildcard include/config/sys/proto/erps.h) \
    $(wildcard include/config/sys/proto/garp.h) \
    $(wildcard include/config/sys/rtl838x/l3/entry/num.h) \
    $(wildcard include/config/sys/rtl839x/l3/entry/num.h) \
    $(wildcard include/config/sys/rtl9300/l3/entry/num.h) \
    $(wildcard include/config/sys/rtl9310/l3/entry/num.h) \
    $(wildcard include/config/sys/l3/intflo/num.h) \
    $(wildcard include/config/sys/l3/intfvlan/num.h) \
    $(wildcard include/config/sys/l3/intfvlan/ip/sub/num.h) \
    $(wildcard include/config/sys/l3/prefix/ipv4/num.h) \
    $(wildcard include/config/sys/l3/prefix/ipv6/num.h) \
    $(wildcard include/config/sys/l3/host/route/num.h) \
    $(wildcard include/config/sys/l3/route.h) \
    $(wildcard include/config/sys/app/iproute2.h) \
    $(wildcard include/config/sys/l3/architecture.h) \
    $(wildcard include/config/sys/l3/route/rip.h) \
    $(wildcard include/config/sys/l3/route/zebra.h) \
    $(wildcard include/config/sys/l3/route/ospf.h) \
    $(wildcard include/config/sys/l3/route/bgp.h) \
    $(wildcard include/config/sys/l3/vrrp.h) \
    $(wildcard include/config/sys/dhcp/server.h) \
    $(wildcard include/config/sys/vlan/max/num.h) \
    $(wildcard include/config/sys/proto/gvrp.h) \
    $(wildcard include/config/sys/macvlan.h) \
    $(wildcard include/config/sys/protocol/vlan.h) \
    $(wildcard include/config/sys/vlan/qinq.h) \
    $(wildcard include/config/sys/surveillancevlan.h) \
    $(wildcard include/config/sys/voicevlan.h) \
    $(wildcard include/config/sys/ip/subnet.h) \
    $(wildcard include/config/sys/vlan/port/mode/access.h) \
    $(wildcard include/config/sys/vlan/port/mode/hybrid.h) \
    $(wildcard include/config/sys/vlan/port/mode/trunk.h) \
    $(wildcard include/config/sys/mcast/group/num.h) \
    $(wildcard include/config/sys/proto/igmp/snooping.h) \
    $(wildcard include/config/sys/proto/igmpv3/basic.h) \
    $(wildcard include/config/sys/proto/igmp/snooping/querier.h) \
    $(wildcard include/config/sys/proto/igmp/static/router.h) \
    $(wildcard include/config/sys/proto/igmp/static/forward.h) \
    $(wildcard include/config/sys/proto/igmp/filtering.h) \
    $(wildcard include/config/sys/proto/igmp/throttling.h) \
    $(wildcard include/config/sys/proto/mld/snooping.h) \
    $(wildcard include/config/sys/proto/mvr.h) \
    $(wildcard include/config/sys/bw/ctrl.h) \
    $(wildcard include/config/sys/user/defined/acl.h) \
    $(wildcard include/config/sys/user/defined/acl/deny/all.h) \
    $(wildcard include/config/sys/aaa.h) \
    $(wildcard include/config/sys/aaa/authentication.h) \
    $(wildcard include/config/sys/aaa/enable/authentication.h) \
    $(wildcard include/config/sys/proto/authmgr.h) \
    $(wildcard include/config/sys/proto/dot1x.h) \
    $(wildcard include/config/sys/proto/macauth.h) \
    $(wildcard include/config/sys/proto/webauth.h) \
    $(wildcard include/config/sys/del/def/username.h) \
    $(wildcard include/config/sys/user/ps/mix/length.h) \
    $(wildcard include/config/sys/console/no/auth.h) \
    $(wildcard include/config/sys/black/hole/mac.h) \
    $(wildcard include/config/sys/flapping/mac.h) \
    $(wildcard include/config/sys/stp/bpdu/guard.h) \
    $(wildcard include/config/sys/defence/engine.h) \
    $(wildcard include/config/sys/cpu/queue/utilization.h) \
    $(wildcard include/config/sys/proto/dhcpsnooping.h) \
    $(wildcard include/config/sys/proto/dhcp.h) \
    $(wildcard include/config/sys/proto/dhcpsnooping/userdefine/cid.h) \
    $(wildcard include/config/sys/dos/protection.h) \
    $(wildcard include/config/sys/proto/dai.h) \
    $(wildcard include/config/user/boa/with/ssl.h) \
    $(wildcard include/config/sys/proto/isg.h) \
    $(wildcard include/config/sys/proto/isg/netmask.h) \
    $(wildcard include/config/sys/management/vlan.h) \
    $(wildcard include/config/sys/mgmt/acl.h) \
    $(wildcard include/config/sys/port/security.h) \
    $(wildcard include/config/sys/port/isolation.h) \
    $(wildcard include/config/sys/lib/radius.h) \
    $(wildcard include/config/user/ssh/ssh.h) \
    $(wildcard include/config/sys/app/ssh.h) \
    $(wildcard include/config/user/ssh/sshkeygen.h) \
    $(wildcard include/config/user/ssh/sshd.h) \
    $(wildcard include/config/sys/lib/ssl.h) \
    $(wildcard include/config/sys/app/openssl.h) \
    $(wildcard include/config/sys/storm/ctrl.h) \
    $(wildcard include/config/sys/lib/tacplus.h) \
    $(wildcard include/config/sys/backup/configuration.h) \
    $(wildcard include/config/sys/ui/cli.h) \
    $(wildcard include/config/sys/ui/cli/session.h) \
    $(wildcard include/config/sys/app/dhcpc.h) \
    $(wildcard include/config/sys/advance/dns.h) \
    $(wildcard include/config/sys/dual/image.h) \
    $(wildcard include/config/sys/eee.h) \
    $(wildcard include/config/sys/port/err/disabled.h) \
    $(wildcard include/config/sys/port/description.h) \
    $(wildcard include/config/sys/ui/cli/line/mgmt.h) \
    $(wildcard include/config/sys/ui/cli/session/timeout.h) \
    $(wildcard include/config/sys/ui/cli/passretry/silent.h) \
    $(wildcard include/config/sys/user/access/meth/value.h) \
    $(wildcard include/config/sys/mib/rmon/mgmt/ui.h) \
    $(wildcard include/config/sys/ui/snmp.h) \
    $(wildcard include/config/sys/snmpv3.h) \
    $(wildcard include/config/sys/app/msntp.h) \
    $(wildcard include/config/sys/app/telnetd.h) \
    $(wildcard include/config/user/telnetd/does/not/use/openpty.h) \
    $(wildcard include/config/sys/app/telnet.h) \
    $(wildcard include/config/sys/ui/web.h) \
    $(wildcard include/config/sys/app/httpd.h) \
    $(wildcard include/config/sys/ui/web/login/page.h) \
    $(wildcard include/config/sys/ui/web/session/timeout.h) \
    $(wildcard include/config/sys/ui/web/three/level/tree.h) \
    $(wildcard include/config/sys/app/easycwmp.h) \
    $(wildcard include/config/sys/proto/lldp.h) \
    $(wildcard include/config/sys/proto/lldp/med.h) \
    $(wildcard include/config/sys/bw/utilization.h) \
    $(wildcard include/config/sys/cable/diag.h) \
    $(wildcard include/config/sys/cpu/utilization.h) \
    $(wildcard include/config/sys/opticalmod/status.h) \
    $(wildcard include/config/sys/syslog.h) \
    $(wildcard include/config/user/syslog/ram/size/256k.h) \
    $(wildcard include/config/user/syslog/flash/size/256k.h) \
    $(wildcard include/config/user/syslog/ram/size.h) \
    $(wildcard include/config/user/syslog/flash/size.h) \
    $(wildcard include/config/sys/log/severity/level.h) \
    $(wildcard include/config/sys/mem/utilization.h) \
    $(wildcard include/config/sys/ping/test.h) \
    $(wildcard include/config/sys/mirror.h) \
    $(wildcard include/config/sys/trace/route.h) \
    $(wildcard include/config/sys/tech/support.h) \
    $(wildcard include/config/sys/proto/udld.h) \
    $(wildcard include/config/sys/mibii.h) \
    $(wildcard include/config/sys/mib/trap.h) \
    $(wildcard include/config/sys/mib/bridge.h) \
    $(wildcard include/config/sys/mib/ext/bridge.h) \
    $(wildcard include/config/sys/mib/entity.h) \
    $(wildcard include/config/sys/mib/rmon.h) \
    $(wildcard include/config/sys/mib/interface/group.h) \
    $(wildcard include/config/sys/mib/etherlike.h) \
    $(wildcard include/config/sys/mib/community.h) \
    $(wildcard include/config/sys/mib/snmpv3/conf.h) \
    $(wildcard include/config/sys/mib/snmp/lldp/conf.h) \
    $(wildcard include/config/private/mib.h) \
    $(wildcard include/config/sys/mib/poe/conf.h) \
    $(wildcard include/config/sys/mib/hardwareinfo/conf.h) \
    $(wildcard include/config/sys/mib/portctrl/conf.h) \
    $(wildcard include/config/sys/mib/dhcpsnooping/conf.h) \
    $(wildcard include/config/sys/button/reset/interrupt.h) \
    $(wildcard include/config/sys/button/reset/polling.h) \
    $(wildcard include/config/sys/button/reset.h) \
    $(wildcard include/config/sys/dying/gasp.h) \
    $(wildcard include/config/sys/fiber/preference/interrupt.h) \
    $(wildcard include/config/sys/fiber/preference/polling.h) \
    $(wildcard include/config/sys/fiber/preference.h) \
    $(wildcard include/config/sys/fiber/txdisable.h) \
    $(wildcard include/config/sys/led/blinking/continue.h) \
    $(wildcard include/config/sys/led/blinking/32ms.h) \
    $(wildcard include/config/sys/led/blinking/128ms.h) \
    $(wildcard include/config/sys/led/blinking/512ms.h) \
    $(wildcard include/config/sys/led/mode/off.h) \
    $(wildcard include/config/sys/reset.h) \
    $(wildcard include/config/sys/test/sw/check.h) \
    $(wildcard include/config/sys/hw/usb.h) \
    $(wildcard include/config/sys/poe/swdog.h) \
    $(wildcard include/config/sys/poe/led.h) \
    $(wildcard include/config/sys/poe/chip/tps23861.h) \
    $(wildcard include/config/sys/poe/chip/tps23880.h) \
    $(wildcard include/config/sys/poe/chip/tps23882.h) \
    $(wildcard include/config/sys/poe/chip/rtl8238.h) \
    $(wildcard include/config/sys/poe/chip/rtl8238b.h) \
    $(wildcard include/config/sys/poe/chip/rtl8239.h) \
    $(wildcard include/config/sys/poe/chip/mp3924.h) \
    $(wildcard include/config/sys/poe/chip/ip808.h) \
    $(wildcard include/config/sys/board/debug/info.h) \
    $(wildcard include/config/sys/ho/board.h) \
    $(wildcard include/config/sys/build/devel.h) \
    $(wildcard include/config/sys/build/release.h) \
    $(wildcard include/config/sys/kernel/linux.h) \
    $(wildcard include/config/sys/kernel/linux/module/loadable.h) \
    $(wildcard include/config/sys/plat/lxboard.h) \
    $(wildcard include/config/sys/plat/lxdefdb.h) \
    $(wildcard include/config/sys/plat/lxksi.h) \
    $(wildcard include/config/sys/plat/lxski.h) \
    $(wildcard include/config/sys/plat/lxvlan.h) \
    $(wildcard include/config/sys/plat/fw.h) \
    $(wildcard include/config/sys/app.h) \
    $(wildcard include/config/sys/lib.h) \
    $(wildcard include/config/sys/plat/autoreboot.h) \
    $(wildcard include/config/sys/kernel/linux/compressed/fs.h) \
    $(wildcard include/config/sys/plat/cusmod.h) \
    $(wildcard include/config/sys/plat/dbgmod.h) \
    $(wildcard include/config/sys/ipv6.h) \
    $(wildcard include/config/sys/app/dhcp6c.h) \
    $(wildcard include/config/sys/app/dhcp6c/stateless.h) \
    $(wildcard include/config/sys/fw/signature/check.h) \
    $(wildcard include/config/sys/fw/upgrade/checkmem.h) \
    $(wildcard include/config/sys/fw/upgrade/savemem.h) \
    $(wildcard include/config/sys/fw/upgrade/savemem/size.h) \
    $(wildcard include/config/sys/lib/cfg.h) \
    $(wildcard include/config/sys/lib/cmd.h) \
    $(wildcard include/config/sys/lib/custom.h) \
    $(wildcard include/config/sys/lib/fds.h) \
    $(wildcard include/config/sys/lib/gcc.h) \
    $(wildcard include/config/sys/lib/glue.h) \
    $(wildcard include/config/sys/lib/ncurses.h) \
    $(wildcard include/config/sys/lib/readline.h) \
    $(wildcard include/config/sys/lib/sal.h) \
    $(wildcard include/config/sys/lib/vty.h) \
    $(wildcard include/config/sys/lib/ipc.h) \
    $(wildcard include/config/sys/app/inetd.h) \
    $(wildcard include/config/sys/app/initd.h) \
    $(wildcard include/config/sys/app/ksid.h) \
    $(wildcard include/config/sys/app/polld.h) \
    $(wildcard include/config/sys/kernel/linux/3/18/24/x.h) \
    $(wildcard include/config/sys/loader/uboot2011.h) \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_type.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/common/rt_type.h \
    $(wildcard include/config/sdk/rtl9310.h) \
    $(wildcard include/config/sdk/rtl9300.h) \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/common/rt_autoconf.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/common/sdk_autoconf.h \
    $(wildcard include/config/sdk/rtl8390.h) \
    $(wildcard include/config/sdk/rtl8380.h) \
    $(wildcard include/config/sdk/rtl8208d.h) \
    $(wildcard include/config/sdk/rtl8208g.h) \
    $(wildcard include/config/sdk/rtl8208l.h) \
    $(wildcard include/config/sdk/rtl8212b.h) \
    $(wildcard include/config/sdk/rtl8214b.h) \
    $(wildcard include/config/sdk/rtl8214fb.h) \
    $(wildcard include/config/sdk/rtl8214fc.h) \
    $(wildcard include/config/sdk/rtl8214c.h) \
    $(wildcard include/config/sdk/rtl8214qf.h) \
    $(wildcard include/config/sdk/rtl8218b.h) \
    $(wildcard include/config/sdk/rtl8218d.h) \
    $(wildcard include/config/sdk/rtl8218e.h) \
    $(wildcard include/config/sdk/rtl8218fb.h) \
    $(wildcard include/config/sdk/rtl8224qf.h) \
    $(wildcard include/config/sdk/rtl8226.h) \
    $(wildcard include/config/sdk/rtl8295r.h) \
    $(wildcard include/config/sdk/rtl826x.h) \
    $(wildcard include/config/sdk/rtl826xi.h) \
    $(wildcard include/config/sdk/rtl825x.h) \
    $(wildcard include/config/sdk/rtl825xi.h) \
    $(wildcard include/config/sdk/rtl825xl.h) \
    $(wildcard include/config/sdk/rtl8224.h) \
    $(wildcard include/config/sdk/rtl826xb.h) \
    $(wildcard include/config/sdk/rtl8231.h) \
    $(wildcard include/config/sdk/phy/cust1.h) \
    $(wildcard include/config/sdk/phy/cust2.h) \
    $(wildcard include/config/sdk/phy/cust3.h) \
    $(wildcard include/config/sdk/phy/cust4.h) \
    $(wildcard include/config/sdk/phy/cust5.h) \
    $(wildcard include/config/sdk/experimental/rtl8211fs.h) \
    $(wildcard include/config/sdk/external/cpu.h) \
    $(wildcard include/config/sdk/endian/little.h) \
    $(wildcard include/config/sdk/kernel/linux.h) \
    $(wildcard include/config/sdk/os/kernel/other.h) \
    $(wildcard include/config/sdk/kernel/linux/kernel/mode.h) \
    $(wildcard include/config/sdk/kernel/linux/user/mode.h) \
    $(wildcard include/config/sdk/kernel/linux/module/loadable.h) \
    $(wildcard include/config/sdk/driver/rtcore.h) \
    $(wildcard include/config/sdk/driver/rtcore/in/linux/kernel.h) \
    $(wildcard include/config/sdk/driver/nic.h) \
    $(wildcard include/config/sdk/driver/extc/nic.h) \
    $(wildcard include/config/sdk/driver/extc/pci.h) \
    $(wildcard include/config/sdk/driver/gpio.h) \
    $(wildcard include/config/sdk/driver/spi.h) \
    $(wildcard include/config/sdk/driver/i2c.h) \
    $(wildcard include/config/sdk/tc/drv.h) \
    $(wildcard include/config/sdk/driver/watchdog.h) \
    $(wildcard include/config/sdk/tc/tc1/time.h) \
    $(wildcard include/config/sdk/uart1.h) \
    $(wildcard include/config/sys/driver/jtag/intf/gpio.h) \
    $(wildcard include/config/sdk/driver/nic/kernel/mode.h) \
    $(wildcard include/config/sdk/driver/nic/user/mode.h) \
    $(wildcard include/config/sdk/driver/nic/r9310.h) \
    $(wildcard include/config/sdk/driver/nic/r9300.h) \
    $(wildcard include/config/sdk/driver/nic/r8390.h) \
    $(wildcard include/config/sdk/driver/nic/r8380.h) \
    $(wildcard include/config/sdk/driver/l2ntfy.h) \
    $(wildcard include/config/sdk/driver/l2ntfy/r9310.h) \
    $(wildcard include/config/sdk/driver/l2ntfy/r9300.h) \
    $(wildcard include/config/sdk/driver/l2ntfy/r8390.h) \
    $(wildcard include/config/sdk/driver/i2c/r9310.h) \
    $(wildcard include/config/sdk/driver/i2c/r9300.h) \
    $(wildcard include/config/sdk/driver/i2c/r8390.h) \
    $(wildcard include/config/sdk/driver/i2c/r8380.h) \
    $(wildcard include/config/sdk/driver/rtk.h) \
    $(wildcard include/config/sdk/driver/rtk/legacy/api.h) \
    $(wildcard include/config/sdk/driver/rtk/customer.h) \
    $(wildcard include/config/sdk/driver/mx.h) \
    $(wildcard include/config/sdk/driver/rtdrv.h) \
    $(wildcard include/config/sdk/driver/rtnic.h) \
    $(wildcard include/config/sdk/driver/compile.h) \
    $(wildcard include/config/sdk/app/diag.h) \
    $(wildcard include/config/sdk/app/diag/dir/diag/v2.h) \
    $(wildcard include/config/sdk/app/diag/ext.h) \
    $(wildcard include/config/sdk/squt/cmd/opt.h) \
    $(wildcard include/config/sdk/app/compile.h) \
    $(wildcard include/config/sdk/debug.h) \
    $(wildcard include/config/sdk/debug/log/type/level.h) \
    $(wildcard include/config/sdk/debug/log/type/level/mask.h) \
    $(wildcard include/config/sdk/debug/log/level.h) \
    $(wildcard include/config/sdk/debug/log/mod/mask.h) \
    $(wildcard include/config/sdk/dump/reg/with/name.h) \
    $(wildcard include/config/sdk/dump/table/with/name.h) \
    $(wildcard include/config/sdk/bsp/flash.h) \
    $(wildcard include/config/flash/layout/type5.h) \
    $(wildcard include/config/pswitch/sysinfo.h) \
    $(wildcard include/config/bdinfo/mtd/index.h) \
    $(wildcard include/config/dual/image.h) \
    $(wildcard include/config/sdk/bsp/mtd.h) \
    $(wildcard include/config/sdk/bsp/mtd/spi.h) \
    $(wildcard include/config/sdk/bsp/mtd/nor.h) \
    $(wildcard include/config/sdk/bsp/mtd/spim.h) \
    $(wildcard include/config/sdk/bsp/mtd/norsf/g3.h) \
    $(wildcard include/config/sdk/bsp/mtd/common.h) \
    $(wildcard include/config/sdk/spi/dev/none.h) \
    $(wildcard include/config/sdk/spi/dev/1.h) \
    $(wildcard include/config/sdk/spi/dev/2.h) \
    $(wildcard include/config/sdk/spi/dev/3.h) \
    $(wildcard include/config/sdk/spi/dev/4.h) \
    $(wildcard include/config/sdk/spi/dev/5.h) \
    $(wildcard include/config/sdk/bsp/spi/nor/boot.h) \
    $(wildcard include/config/sdk/bsp/spi/nand/boot.h) \
    $(wildcard include/config/sdk/experimental.h) \
    $(wildcard include/config/sdk/fpga/platform.h) \
    $(wildcard include/config/sdk/driver/test.h) \
    $(wildcard include/config/sdk/model/mode.h) \
    $(wildcard include/config/sdk/linked/kmod.h) \
    $(wildcard include/config/trunk/failover/handling.h) \
    $(wildcard include/config/sdk/rx/thread.h) \
    $(wildcard include/config/sdk/linkmon/polling/mode.h) \
    $(wildcard include/config/sdk/linkmon/isr/mode.h) \
    $(wildcard include/config/sdk/linkmon/mixed/mode.h) \
    $(wildcard include/config/rise.h) \
    $(wildcard include/config/sdk/hw/profile/probe/gpio.h) \
    $(wildcard include/config/sdk/hw/profile/probe/uboot/env.h) \
    $(wildcard include/config/sdk/os/version.h) \
    $(wildcard include/config/sdk/compiler/version.h) \
    $(wildcard include/config/sdk/libc/version.h) \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/common/type.h \
    $(wildcard include/config/sdk/wordsize/64.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/common/util/rt_bitop.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_aaa.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_acl.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_authmgr.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_common.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_cli.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_dai.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_dhcp.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_dos.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_isg.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_l2.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_l3.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_lldp.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_mcast.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_mirror.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_port.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_qos.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_rate.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_mibrmon.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_security.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_snmp.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_stp.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_sys.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_syslog.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_time.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_lag.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_udld.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_vlan.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_erps.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/swdef/sys_swdef_poe.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/trap.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/port.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/chipdef.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/chip.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/sds.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/phy.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/time.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/led.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_assert.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_bitmap.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_error.h \
    $(wildcard include/config/phy.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/common/rt_error.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/common/error.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_list.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_log.h \
  /opt/toolchain/lib/gcc/mips-linux-uclibc/4.8.5/include/stdarg.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_log_desc.h \
    $(wildcard include/config/start.h) \
    $(wildcard include/config/stop.h) \
    $(wildcard include/config/fail.h) \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_log_desc_old.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_util.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_notifier.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/cache.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/inet.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/isr.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/lib.h \
  include/linux/ctype.h \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  include/uapi/linux/types.h \
  arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/mips/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  arch/mips/include/uapi/asm/types.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  arch/mips/include/uapi/asm/posix_types.h \
  arch/mips/include/uapi/asm/sgidefs.h \
  include/uapi/asm-generic/posix_types.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/mips/include/asm/linkage.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  include/linux/bitops.h \
  arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/has/cls.h) \
  arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/kvm/guest.h) \
    $(wildcard include/config/dma/noncoherent.h) \
    $(wildcard include/config/cpu/rlx.h) \
  include/uapi/linux/const.h \
  arch/mips/include/uapi/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/uapi/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  arch/mips/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  arch/mips/include/asm/compiler.h \
    $(wildcard include/config/cpu/micromips.h) \
    $(wildcard include/config/cpu/mipsr6.h) \
  arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/sys/supports/micromips.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
    $(wildcard include/config/cpu/has/msa.h) \
  arch/mips/include/asm/cpu.h \
  arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  arch/mips/include/asm/mach-generic/cpu-feature-overrides.h \
    $(wildcard include/config/cpu/mipsia.h) \
    $(wildcard include/config/cpu/mipspa.h) \
    $(wildcard include/config/cpu/has/sleep.h) \
    $(wildcard include/config/cpu/has/radiax.h) \
    $(wildcard include/config/cpu/has/ar7.h) \
    $(wildcard include/config/cpu/has/fpu.h) \
    $(wildcard include/config/cpu/has/dsp.h) \
    $(wildcard include/config/cpu/has/dsp2.h) \
    $(wildcard include/config/mips/mt.h) \
    $(wildcard include/config/cpu/has/tls.h) \
    $(wildcard include/config/hardware/watchpoints.h) \
    $(wildcard include/config/cpu/mips14k.h) \
    $(wildcard include/config/cpu/mips74k.h) \
  arch/mips/bsp/bspcpu.h \
    $(wildcard include/config/rtk/cache/define.h) \
    $(wildcard include/config/addr.h) \
  arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  arch/mips/include/asm/mach-generic/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  arch/mips/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/lto.h) \
  include/linux/kern_levels.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/uapi/linux/kernel.h \
  include/uapi/linux/sysinfo.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/rlx4181.h) \
    $(wildcard include/config/cpu/rlx5181.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/mips/include/asm/errno.h \
  arch/mips/include/uapi/asm/errno.h \
  include/uapi/asm-generic/errno-base.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/memory.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/print.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/sem.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/spl.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/thread.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/osal/time.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/time.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/osal/timer.h \
  include/linux/jiffies.h \
  include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/seqlock.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  arch/mips/include/generated/asm/preempt.h \
  include/asm-generic/preempt.h \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  arch/mips/include/asm/break.h \
  arch/mips/include/uapi/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/mips/pgd/c0/context.h) \
  arch/mips/include/asm/processor.h \
    $(wildcard include/config/cpu/little/endian.h) \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/cpu/xlp.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  arch/mips/include/uapi/asm/cachectl.h \
  arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/mips/huge/tlb/support.h) \
    $(wildcard include/config/sys/supports/mips16.h) \
    $(wildcard include/config/cpu/has/micromips.h) \
  arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/bmips.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/xlr.h) \
    $(wildcard include/config/cpu/sb1.h) \
  arch/mips/include/asm/prefetch.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/irq/cpu.h) \
  include/linux/bottom_half.h \
  include/linux/preempt_mask.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  arch/mips/include/asm/atomic.h \
  arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/time64.h \
  include/uapi/linux/time.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  include/uapi/linux/param.h \
  arch/mips/include/uapi/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  arch/mips/include/asm/timex.h \
  arch/mips/include/asm/cpu-type.h \
    $(wildcard include/config/sys/has/cpu/loongson2e.h) \
    $(wildcard include/config/sys/has/cpu/loongson2f.h) \
    $(wildcard include/config/sys/has/cpu/loongson3.h) \
    $(wildcard include/config/sys/has/cpu/loongson1b.h) \
    $(wildcard include/config/sys/has/cpu/mips32/r1.h) \
    $(wildcard include/config/sys/has/cpu/mips32/r2.h) \
    $(wildcard include/config/sys/has/cpu/mips64/r1.h) \
    $(wildcard include/config/sys/has/cpu/mips64/r2.h) \
    $(wildcard include/config/sys/has/cpu/r3000.h) \
    $(wildcard include/config/sys/has/cpu/tx39xx.h) \
    $(wildcard include/config/sys/has/cpu/vr41xx.h) \
    $(wildcard include/config/sys/has/cpu/r4300.h) \
    $(wildcard include/config/sys/has/cpu/r4x00.h) \
    $(wildcard include/config/sys/has/cpu/tx49xx.h) \
    $(wildcard include/config/sys/has/cpu/r5000.h) \
    $(wildcard include/config/sys/has/cpu/r5432.h) \
    $(wildcard include/config/sys/has/cpu/r5500.h) \
    $(wildcard include/config/sys/has/cpu/r6000.h) \
    $(wildcard include/config/sys/has/cpu/nevada.h) \
    $(wildcard include/config/sys/has/cpu/r8000.h) \
    $(wildcard include/config/sys/has/cpu/r10000.h) \
    $(wildcard include/config/sys/has/cpu/rm7000.h) \
    $(wildcard include/config/sys/has/cpu/sb1.h) \
    $(wildcard include/config/sys/has/cpu/cavium/octeon.h) \
    $(wildcard include/config/sys/has/cpu/bmips32/3300.h) \
    $(wildcard include/config/sys/has/cpu/bmips4350.h) \
    $(wildcard include/config/sys/has/cpu/bmips4380.h) \
    $(wildcard include/config/sys/has/cpu/bmips5000.h) \
    $(wildcard include/config/sys/has/cpu/xlp.h) \
    $(wildcard include/config/sys/has/cpu/xlr.h) \
  include/linux/smp.h \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
  include/linux/timekeeping.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/osal/routine.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/wait.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/atomic.h \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/stack/growsup.h) \
  include/uapi/linux/sched.h \
  include/linux/sched/prio.h \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/rbtree.h \
  include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/movable/node.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/compaction.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  arch/mips/include/generated/uapi/asm/auxvec.h \
  include/uapi/asm-generic/auxvec.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/completion.h \
  include/linux/wait.h \
  arch/mips/include/generated/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/uprobes.h \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/generated/bounds.h \
  arch/mips/include/asm/page.h \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
  include/linux/pfn.h \
  arch/mips/include/asm/io.h \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/dma/maybe/coherent.h) \
  include/asm-generic/iomap.h \
    $(wildcard include/config/has/ioport/map.h) \
    $(wildcard include/config/generic/iomap.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
    $(wildcard include/config/cpu/loongson3.h) \
    $(wildcard include/config/mach/jz4740.h) \
  arch/mips/include/asm/mach-generic/ioremap.h \
  arch/mips/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
  include/asm-generic/getorder.h \
  arch/mips/include/asm/mmu.h \
  arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
  arch/mips/include/asm/isadep.h \
  arch/mips/include/uapi/asm/ptrace.h \
  include/linux/cputime.h \
  arch/mips/include/generated/asm/cputime.h \
  include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime_jiffies.h \
  include/linux/sem.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/rcu/nocb/cpu/all.h) \
    $(wildcard include/config/no/hz/full/sysidle.h) \
  include/linux/rcutiny.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/linux/uidgid.h \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/uapi/linux/ipc.h \
  arch/mips/include/generated/uapi/asm/ipcbuf.h \
  include/uapi/asm-generic/ipcbuf.h \
  arch/mips/include/uapi/asm/sembuf.h \
  include/linux/shm.h \
  include/uapi/linux/shm.h \
  arch/mips/include/uapi/asm/shmbuf.h \
  arch/mips/include/asm/shmparam.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  arch/mips/include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
  arch/mips/include/uapi/asm/signal.h \
  include/uapi/asm-generic/signal-defs.h \
  arch/mips/include/asm/sigcontext.h \
  arch/mips/include/uapi/asm/sigcontext.h \
  arch/mips/include/uapi/asm/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/pid.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  arch/mips/include/generated/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  include/linux/osq_lock.h \
  include/linux/mutex-debug.h \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/srcu.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/sysfs.h) \
  arch/mips/include/asm/topology.h \
  arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/gfp.h \
    $(wildcard include/config/pm/sleep.h) \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/have/arch/seccomp/filter.h) \
    $(wildcard include/config/seccomp/filter.h) \
  include/uapi/linux/seccomp.h \
  include/linux/rculist.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  arch/mips/include/uapi/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/sysctl.h \
  include/uapi/linux/sysctl.h \
  include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/uapi/linux/magic.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/vendor/board_vendor.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/hwp/hw_profile.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/phy/phydef.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/diag.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/allreg.h \
    $(wildcard include/config/sdk/reg/dflt/val.h) \
    $(wildcard include/config/sdk/reg/field/type.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/allmem.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/vendor/board_gpio.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/gpio.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/drv/gpio/generalCtrl_gpio.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/drv/gpio/gpio.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/drv/gpio/ext_gpio.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/port.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/fiber.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/intf.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/i2c.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/drv/i2c/i2c.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/button.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/led.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/watchdog.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/drv/watchdog/watchdog.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/reset.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/reg.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/dyinggasp.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/board/include/poe.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/rsd/rsd_sys.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/switch.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/default.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/trunk.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/vlan.h \
    $(wildcard include/config/sdk/rtl931x.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/l2.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/mcast.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/stp.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/qos.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/pie.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/rate.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/l3.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_def.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_dbg.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_dbg_desc.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_portmask.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_portview.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_vlanmask.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_queuemask.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_l2.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_l3.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/defdb/defdb.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/linux/defdb/defdb_data.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/rsd/rsd_acl_rsv.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/rsd/rsd_acl.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/acl.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/rsd/rsd_acl_msw.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/rsd/rsd_trap.h \

/compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.o: $(deps_/compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.o)

$(deps_/compile/realtek/pswitch/plat/linux/board_conf/vendor/board_conf_button.o):
