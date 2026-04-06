# $Id: Makefile.in,v 1.4 2006/08/19 09:12:14 dtucker Exp $

sysconfdir=/etc
piddir=/var/run
srcdir=../../../openbsd-compat/regress
top_srcdir=../../..

VPATH=../../../openbsd-compat/regress
CC=rtk-ms-2.0.0-linux-gcc 
LD=rtk-ms-2.0.0-linux-ld 
CFLAGS=-Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/libcrypt  -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/zlib -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/pswitch/app/ssh/../..//plat/linux/ski -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/../../sdk/system/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/../../sdk/include -DINCLUDE_SSHD -g -I/compile/realtek/pswitch/app/ssh/../..//lib -I/compile/realtek/pswitch/app/ssh/../..//plat -I/compile/realtek/pswitch/app/ssh/../..//plat/linux -I/compile/realtek/pswitch/app/ssh/../..//proto -I/compile/realtek/pswitch/app/ssh/../..//lib/libglue -Wall -Wpointer-arith -Wuninitialized -Wsign-compare -Wformat-security -Wno-pointer-sign -Wno-unused-result -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -fno-builtin-memset -fstack-protector-all 
CPPFLAGS=-I. -I.. -I$(srcdir) -I$(srcdir)/.. -I/compile/realtek/pswitch/app/ssh/../..//lib/libssl/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib  -DHAVE_CONFIG_H
EXEEXT=
LIBCOMPAT=../libopenbsd-compat.a
LIBS=-lcrypto -ldl -lz  -lcrypt
LDFLAGS=-L/compile/realtek/pswitch/app/ssh/../..//lib/libssl -L/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib  -fstack-protector-all $(LIBCOMPAT)

TESTPROGS=closefromtest$(EXEEXT) snprintftest$(EXEEXT) strduptest$(EXEEXT) \
	strtonumtest$(EXEEXT)

all:	t-exec ${OTHERTESTS}

%$(EXEEXT):	%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< $(LIBCOMPAT) $(LIBS)

t-exec:	$(TESTPROGS)
	@echo running compat regress tests
	@for TEST in ""$?; do \
		echo "run test $${TEST}" ... 1>&2; \
		./$${TEST}$(EXEEXT) || exit $$? ; \
	done
	@echo finished compat regress tests

clean:
	rm -f *.o *.a core $(TESTPROGS) valid.out

distclean: clean
	rm -f Makefile *~
