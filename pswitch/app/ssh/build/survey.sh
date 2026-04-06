#!/bin/sh
#
# Copyright (c) 2004, 2005 Darren Tucker
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

host="msdk-4.8.5-3.18-unknown-linux-gnu"
AWK="gawk"
CC="rtk-ms-2.0.0-linux-gcc "
CPP="rtk-ms-2.0.0-linux-gcc  -E"
CFLAGS="-Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/libcrypt  -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib/zlib -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/pswitch/app/ssh/../..//plat/linux/ski -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/../../sdk/system/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/../../sdk/include -DINCLUDE_SSHD -g -I/compile/realtek/pswitch/app/ssh/../..//lib -I/compile/realtek/pswitch/app/ssh/../..//plat -I/compile/realtek/pswitch/app/ssh/../..//plat/linux -I/compile/realtek/pswitch/app/ssh/../..//proto -I/compile/realtek/pswitch/app/ssh/../..//lib/libglue -Wall -Wpointer-arith -Wuninitialized -Wsign-compare -Wformat-security -Wno-pointer-sign -Wno-unused-result -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -fno-builtin-memset -fstack-protector-all "
CPPFLAGS="-I/compile/realtek/pswitch/app/ssh/../..//lib/libssl/include -I/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib "
LDFLAGS="-L/compile/realtek/pswitch/app/ssh/../..//lib/libssl -L/compile/realtek/pswitch/app/ssh/../..//../kernel/uClinux/lib  -fstack-protector-all"
LIBS="-lcrypto -ldl -lz  -lcrypt"

# Note format:
# identifier: [data] CRCR

echo "openssh-survey-version: 1"
echo
echo "openssh-version: `./ssh -V 2>&1`"
echo
configinv=`$AWK '/^  \\\$.*configure/' config.log | sed 's/^  \\\$ //g'`
echo "configure-invocation: $configinv"
echo
echo "host: $host"
echo
echo "uname: `uname`"
echo
echo "uname-r: `uname -r`"
echo
echo "uname-m: `uname -m`"
echo
echo "uname-p: `uname -p`"
echo
echo "oslevel: `oslevel 2>/dev/null`"
echo
echo "oslevel-r: `oslevel -r 2>/dev/null`"
echo
echo "cc: $CC"
echo
echo "cflags: $CFLAGS"
echo
echo "cppflags: $CPPFLAGS"
echo
echo "ldflags: $LDFLAGS"
echo
echo "libs: $LIBS"
echo
echo "ccver-v: `$CC -v 2>&1 | sed '/^[ \t]*$/d'`"
echo
echo "ccver-V: `$CC -V 2>&1 | sed '/^[ \t]*$/d'`"
echo
echo "cppdefines:"
${CPP} -dM - </dev/null
echo
echo "config.h:"
egrep '#define|#undef' config.h
echo
