cmd_libbb/perror_nomsg_and_die.o := rtk-ms-2.0.0-linux-gcc  -Wp,-MD,libbb/.perror_nomsg_and_die.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -D"BB_VER=KBUILD_STR(1.20.2)" -DBB_BT=AUTOCONF_TIMESTAMP -Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/kernel/uClinux/include -I/compile/realtek/kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/kernel/uClinux/lib/libcrypt  -I/compile/realtek/kernel/uClinux/lib/zlib -I/compile/realtek/kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/kernel/uClinux/../../pswitch/plat/linux/ski -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/include -DINET6 -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -I/compile/realtek/kernel/uClinux/../../pswitch/plat -DCONFIG_RTK_PATCH    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(perror_nomsg_and_die)"  -D"KBUILD_MODNAME=KBUILD_STR(perror_nomsg_and_die)" -c -o libbb/.tmp_perror_nomsg_and_die.o libbb/perror_nomsg_and_die.c

deps_libbb/perror_nomsg_and_die.o := \
  libbb/perror_nomsg_and_die.c \
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

libbb/perror_nomsg_and_die.o: $(deps_libbb/perror_nomsg_and_die.o)

$(deps_libbb/perror_nomsg_and_die.o):
