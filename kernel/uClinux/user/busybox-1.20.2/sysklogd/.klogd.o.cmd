cmd_sysklogd/klogd.o := rtk-ms-2.0.0-linux-gcc  -Wp,-MD,sysklogd/.klogd.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -D"BB_VER=KBUILD_STR(1.20.2)" -DBB_BT=AUTOCONF_TIMESTAMP -Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/kernel/uClinux/include -I/compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/kernel/uClinux/lib/libcrypt  -I/compile/realtek/kernel/uClinux/lib/zlib -I/compile/realtek/kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/kernel/uClinux/../../pswitch/plat/linux/ski -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/include -DINET6 -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -I/compile/realtek/kernel/uClinux/../../pswitch/plat -DCONFIG_RTK_PATCH    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(klogd)"  -D"KBUILD_MODNAME=KBUILD_STR(klogd)" -c -o sysklogd/.tmp_klogd.o sysklogd/klogd.c

deps_sysklogd/klogd.o := \
  sysklogd/klogd.c \
    $(wildcard include/config/rtk/patch.h) \
    $(wildcard include/config/feature/klogd/klogctl.h) \
    $(wildcard include/config/feature/clean/up.h) \
  include/libbb.h \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/xz.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/editing/save/on/exit.h) \
    $(wildcard include/config/pmap.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/unicode/support.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/limits.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/features.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uClibc_config.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/cdefs.h \
  /opt/toolchain/lib/gcc/mips-linux-uclibc/4.8.5/include-fixed/limits.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/posix1_lim.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/local_lim.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/linux/limits.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uClibc_local_lim.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/posix2_lim.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/xopen_lim.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/stdio_lim.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/byteswap.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/byteswap.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/byteswap-common.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/endian.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/endian.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/stdint.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/wchar.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/wordsize.h \
  /opt/toolchain/lib/gcc/mips-linux-uclibc/4.8.5/include/stdbool.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/unistd.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/posix_opt.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uClibc_posix_opt.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/environments.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/types.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/typesizes.h \
  /opt/toolchain/lib/gcc/mips-linux-uclibc/4.8.5/include/stddef.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/confname.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/getopt.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/ctype.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uClibc_touplow.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/dirent.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/dirent.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/errno.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/errno.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/linux/errno.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm/errno.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm-generic/errno-base.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/syscall.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sysnum.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/fcntl.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/fcntl.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sgidefs.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/types.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/time.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/select.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/select.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sigset.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/time.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/sysmacros.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/pthreadtypes.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uio.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/stat.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/stat.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/inttypes.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/netdb.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/netinet/in.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/socket.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/uio.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/socket.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/socket_type.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sockaddr.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm/socket.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm/sockios.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm/ioctl.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm-generic/ioctl.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/in.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/rpc/netdb.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/siginfo.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/netdb.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/setjmp.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/setjmp.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/signal.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/signum.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sigaction.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sigcontext.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sigstack.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/ucontext.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/sigthread.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/stdio.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uClibc_stdio.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/wchar.h \
  /opt/toolchain/lib/gcc/mips-linux-uclibc/4.8.5/include/stdarg.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/stdlib.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/waitflags.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/waitstatus.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/alloca.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/string.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/libgen.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/poll.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/poll.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/ioctl.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/ioctls.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm/ioctls.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/ioctl-types.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/ttydefaults.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/mman.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/mman.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/time.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/wait.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/resource.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/resource.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/termios.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/termios.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/uClibc_clk_tck.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/param.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/linux/param.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm/param.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/asm-generic/param.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/pwd.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/grp.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/mntent.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/paths.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/statfs.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/bits/statfs.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/arpa/inet.h \
  include/xatonum.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/syslog.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/syslog.h \
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
    $(wildcard include/config/sys/snmp.h) \
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
    $(wildcard include/config/sys/rtl8380.h) \
    $(wildcard include/config/sys/rtl8390.h) \
    $(wildcard include/config/sys/rtl9300.h) \
    $(wildcard include/config/sys/rtl9310.h) \
    $(wildcard include/config/sys/button.h) \
    $(wildcard include/config/sys/button/reset/interrupt.h) \
    $(wildcard include/config/sys/button/reset/polling.h) \
    $(wildcard include/config/sys/button/reset.h) \
    $(wildcard include/config/sys/dying/gasp.h) \
    $(wildcard include/config/sys/fiber.h) \
    $(wildcard include/config/sys/fiber/preference/interrupt.h) \
    $(wildcard include/config/sys/fiber/preference/polling.h) \
    $(wildcard include/config/sys/fiber/preference.h) \
    $(wildcard include/config/sys/fiber/txdisable.h) \
    $(wildcard include/config/sys/led.h) \
    $(wildcard include/config/sys/led/blinking/continue.h) \
    $(wildcard include/config/sys/led/blinking/32ms.h) \
    $(wildcard include/config/sys/led/blinking/128ms.h) \
    $(wildcard include/config/sys/led/blinking/512ms.h) \
    $(wildcard include/config/sys/led/mode/off.h) \
    $(wildcard include/config/sys/reset.h) \
    $(wildcard include/config/sys/watchdog.h) \
    $(wildcard include/config/sys/test/sw/check.h) \
    $(wildcard include/config/sys/uart1.h) \
    $(wildcard include/config/sys/hw/usb.h) \
    $(wildcard include/config/sys/encrypt.h) \
    $(wildcard include/config/sys/poe.h) \
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
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_log.h \
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
    $(wildcard include/config/sdk/driver/rtcore/module.h) \
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
    $(wildcard include/config/sdk/driver/rtk/module.h) \
    $(wildcard include/config/sdk/driver/rtk/legacy/api.h) \
    $(wildcard include/config/sdk/driver/rtk/customer.h) \
    $(wildcard include/config/sdk/driver/mx.h) \
    $(wildcard include/config/sdk/driver/mx/module.h) \
    $(wildcard include/config/sdk/driver/rtdrv.h) \
    $(wildcard include/config/sdk/driver/rtdrv/module.h) \
    $(wildcard include/config/sdk/driver/rtnic.h) \
    $(wildcard include/config/sdk/driver/compile.h) \
    $(wildcard include/config/sdk/driver/compile/module.h) \
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
    $(wildcard include/config/chip/type/1.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/sds.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/phy.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/time.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/led.h \
  /compile/realtek/kernel/uClinux/../../pswitch/plat/common/sys_log_desc.h \
  /compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include/sys/klog.h \

sysklogd/klogd.o: $(deps_sysklogd/klogd.o)

$(deps_sysklogd/klogd.o):
