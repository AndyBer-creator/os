cmd_coreutils/yes.o := rtk-ms-2.0.0-linux-gcc  -Wp,-MD,coreutils/.yes.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -D"BB_VER=KBUILD_STR(1.20.2)" -DBB_BT=AUTOCONF_TIMESTAMP -Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/kernel/uClinux/include -I/compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/kernel/uClinux/lib/libcrypt  -I/compile/realtek/kernel/uClinux/lib/zlib -I/compile/realtek/kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/kernel/uClinux/../../pswitch/plat/linux/ski -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/include -DINET6 -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -I/compile/realtek/kernel/uClinux/../../pswitch/plat -DCONFIG_RTK_PATCH    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(yes)"  -D"KBUILD_MODNAME=KBUILD_STR(yes)" -c -o coreutils/.tmp_yes.o coreutils/yes.c

deps_coreutils/yes.o := \
  coreutils/yes.c \
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
    $(wildcard include/config/feature/clean/up.h) \
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

coreutils/yes.o: $(deps_coreutils/yes.o)

$(deps_coreutils/yes.o):
