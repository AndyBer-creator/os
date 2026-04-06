cmd_arch/mips/lib/csum_partial.o := rtk-ms-2.0.0-linux-gcc -Wp,-MD,arch/mips/lib/.csum_partial.o.d  -nostdinc -isystem /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/include -I./arch/mips/include -Iarch/mips/include/generated  -Iinclude -I./arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0  -D__ASSEMBLY__  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips -Iarch/mips/bsp/ -DRTL9300 -DRTK_UNIVERSAL_BSP -DRTK_LKM -I./arch/mips/include/asm/mach-generic -Wa,-gdwarf-2         -c -o arch/mips/lib/csum_partial.o arch/mips/lib/csum_partial.S

source_arch/mips/lib/csum_partial.o := arch/mips/lib/csum_partial.S

deps_arch/mips/lib/csum_partial.o := \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/loongson3.h) \
    $(wildcard include/config/cpu/little/endian.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
    $(wildcard include/config/cpu/sb1.h) \
    $(wildcard include/config/eva.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/mips/include/asm/errno.h \
  arch/mips/include/uapi/asm/errno.h \
  include/uapi/asm-generic/errno-base.h \
  arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/cpu/rlx.h) \
    $(wildcard include/config/sgi/ip28.h) \
  arch/mips/include/uapi/asm/sgidefs.h \
  arch/mips/include/asm/asm-eva.h \
    $(wildcard include/config/32bit.h) \
  arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  arch/mips/include/asm/regdef.h \

arch/mips/lib/csum_partial.o: $(deps_arch/mips/lib/csum_partial.o)

$(deps_arch/mips/lib/csum_partial.o):
