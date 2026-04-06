# $Id: Makefile.in,v 1.50 2013/02/15 01:13:02 dtucker Exp $

sysconfdir=/etc
piddir=/var/run
srcdir=../../openbsd-compat
top_srcdir=../..

VPATH=../../openbsd-compat
CC=rtk-ms-2.0.0-linux-gcc 
LD=rtk-ms-2.0.0-linux-ld 
CFLAGS=-Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/libcrypt  -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/zlib -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/pswitch/app/ssh/../..//plat/linux/ski -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/../../sdk/system/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/../../sdk/include -DINCLUDE_SSHD -g -I/compile/realtek/pswitch/app/ssh/../..//lib -I/compile/realtek/pswitch/app/ssh/../..//plat -I/compile/realtek/pswitch/app/ssh/../..//plat/linux -I/compile/realtek/pswitch/app/ssh/../..//proto -I/compile/realtek/pswitch/app/ssh/../..//lib/libglue -Wall -Wpointer-arith -Wuninitialized -Wsign-compare -Wformat-security -Wno-pointer-sign -Wno-unused-result -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -fno-builtin-memset -fstack-protector-all 
CPPFLAGS=-I. -I.. -I$(srcdir) -I$(srcdir)/.. -I/compile/realtek/pswitch/app/ssh/../..//lib/libssl/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib  -DHAVE_CONFIG_H
LIBS=-lcrypto -ldl -lz  -lcrypt
AR=/usr/bin/ar
RANLIB=rtk-ms-2.0.0-linux-ranlib
INSTALL=/usr/bin/install -c
LDFLAGS=-L. -L/compile/realtek/pswitch/app/ssh/../..//lib/libssl -L/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib  -fstack-protector-all

OPENBSD=base64.o basename.o bindresvport.o daemon.o dirname.o fmt_scaled.o getcwd.o getgrouplist.o getopt.o getrrsetbyname.o glob.o inet_aton.o inet_ntoa.o inet_ntop.o mktemp.o pwcache.o readpassphrase.o realpath.o rresvport.o setenv.o setproctitle.o sha2.o sigact.o strlcat.o strlcpy.o strmode.o strnlen.o strptime.o strsep.o strtonum.o strtoll.o strtoul.o strtoull.o timingsafe_bcmp.o vis.o

COMPAT=bsd-arc4random.o bsd-asprintf.o bsd-closefrom.o bsd-cray.o bsd-cygwin_util.o bsd-getpeereid.o getrrsetbyname-ldns.o bsd-misc.o bsd-nextstep.o bsd-openpty.o bsd-poll.o bsd-setres_id.o bsd-snprintf.o bsd-statvfs.o bsd-waitpid.o fake-rfc2553.o openssl-compat.o xmmap.o xcrypt.o

PORTS=port-aix.o port-irix.o port-linux.o port-solaris.o port-tun.o port-uw.o

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

all: libopenbsd-compat.a

$(COMPAT): ../config.h
$(OPENBSD): ../config.h
$(PORTS): ../config.h

libopenbsd-compat.a:  $(COMPAT) $(OPENBSD) $(PORTS)
	$(AR) rv $@ $(COMPAT) $(OPENBSD) $(PORTS)
	$(RANLIB) $@

clean:
	rm -f *.o *.a core 

distclean: clean
	rm -f Makefile *~
