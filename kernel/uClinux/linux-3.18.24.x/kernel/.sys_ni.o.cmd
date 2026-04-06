cmd_kernel/sys_ni.o := rtk-ms-2.0.0-linux-gcc -Wp,-MD,kernel/.sys_ni.o.d  -nostdinc -isystem /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/include -I./arch/mips/include -Iarch/mips/include/generated  -Iinclude -I./arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips -Iarch/mips/bsp/ -DRTL9300 -DRTK_UNIVERSAL_BSP -DRTK_LKM -I./arch/mips/include/asm/mach-generic -fno-delete-null-pointer-checks -I/compile/realtek/kernel/uClinux/../../sdk/include -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/system/linux -I/compile/realtek/kernel/uClinux/../../pswitch/plat -I/compile/realtek/kernel/uClinux/../../pswitch/plat/linux -I/compile/realtek/kernel/uClinux/../../pswitch/proto -O2 --param=allow-store-data-races=0 -Wframe-larger-than=4096 -fstack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -fno-var-tracking-assignments -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sys_ni)"  -D"KBUILD_MODNAME=KBUILD_STR(sys_ni)" -c -o kernel/.tmp_sys_ni.o kernel/sys_ni.c

source_kernel/sys_ni.o := kernel/sys_ni.c

deps_kernel/sys_ni.o := \
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
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/mips/include/asm/errno.h \
  arch/mips/include/uapi/asm/errno.h \
  include/uapi/asm-generic/errno-base.h \
  arch/mips/include/asm/unistd.h \
    $(wildcard include/config/mips32/n32.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/mips32/o32.h) \
  arch/mips/include/uapi/asm/unistd.h \

kernel/sys_ni.o: $(deps_kernel/sys_ni.o)

$(deps_kernel/sys_ni.o):
