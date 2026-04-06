cmd_drivers/net/switch/rtk/rtk/sds.o := rtk-ms-2.0.0-linux-gcc -Wp,-MD,drivers/net/switch/rtk/rtk/.sds.o.d  -nostdinc -isystem /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/include -I./arch/mips/include -Iarch/mips/include/generated  -Iinclude -I./arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips -Iarch/mips/bsp/ -DRTL9300 -DRTK_UNIVERSAL_BSP -DRTK_LKM -I./arch/mips/include/asm/mach-generic -fno-delete-null-pointer-checks -I/compile/realtek/kernel/uClinux/../../sdk/include -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/system/linux -I/compile/realtek/kernel/uClinux/../../pswitch/plat -I/compile/realtek/kernel/uClinux/../../pswitch/plat/linux -I/compile/realtek/kernel/uClinux/../../pswitch/proto -O2 --param=allow-store-data-races=0 -Wframe-larger-than=4096 -fstack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -fno-var-tracking-assignments -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO -Wall -Werror -Wuninitialized -Wstrict-prototypes -Wno-array-bounds -I/compile/realtek/kernel/uClinux/../../sdk/include -I/compile/realtek/kernel/uClinux/../../sdk/system/include -I/compile/realtek/kernel/uClinux/../../sdk/system/linux -mlong-calls -fno-pic -DRTK_LKM  -DMODULE -mlong-calls  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sds)"  -D"KBUILD_MODNAME=KBUILD_STR(rtk)" -c -o drivers/net/switch/rtk/rtk/.tmp_sds.o drivers/net/switch/rtk/rtk/sds.c

source_drivers/net/switch/rtk/rtk/sds.o := drivers/net/switch/rtk/rtk/sds.c

deps_drivers/net/switch/rtk/rtk/sds.o := \
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
  /compile/realtek/kernel/uClinux/../../sdk/include/common/rt_error.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/common/error.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/common/util/rt_util.h \
    $(wildcard include/config/sdk/rtksim.h) \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/common/util.h \
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
  /opt/toolchain/lib/gcc/mips-linux-uclibc/4.8.5/include/stdarg.h \
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
  /compile/realtek/kernel/uClinux/../../sdk/include/dal/dal_mgmt.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/dal/dal_mapper.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/dal/dal_common.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/vlan.h \
    $(wildcard include/config/sdk/rtl931x.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/port.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/chipdef.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/chip.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/sds.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/phy.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/time.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/l2.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/mcast.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/l3.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/tunnel.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/trunk.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/stp.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/rate.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/qos.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/trap.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/stat.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/switch.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/phy/phydef.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/diag.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/allreg.h \
    $(wildcard include/config/sdk/reg/dflt/val.h) \
    $(wildcard include/config/sdk/reg/field/type.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/hal/chipdef/allmem.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/mirror.h \
    $(wildcard include/config/sflow/pg/thread.h) \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/flowctrl.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/sec.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/oam.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/pie.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/eee.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/led.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/acl.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/mpls.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/stack.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/vxlan.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/openflow.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/ipmcast.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/bpe.h \
  /compile/realtek/kernel/uClinux/../../sdk/system/include/osal/sem.h \
  /compile/realtek/kernel/uClinux/../../sdk/include/rtk/default.h \

drivers/net/switch/rtk/rtk/sds.o: $(deps_drivers/net/switch/rtk/rtk/sds.o)

$(deps_drivers/net/switch/rtk/rtk/sds.o):
