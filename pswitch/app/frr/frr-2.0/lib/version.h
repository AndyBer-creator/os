/* lib/version.h.  Generated from version.h.in by configure.
 *
 * Quagga version
 * Copyright (C) 1997, 1999 Kunihiro Ishiguro
 * 
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.  
 */

#ifndef _ZEBRA_VERSION_H
#define _ZEBRA_VERSION_H

#ifdef GIT_VERSION
#include "gitversion.h"
#endif

#ifndef GIT_SUFFIX
#define GIT_SUFFIX ""
#endif
#ifndef GIT_INFO
#define GIT_INFO ""
#endif

#define FRR_PAM_NAME    "frr"
#define FRR_SMUX_NAME   "frr"
#define FRR_PTM_NAME    "frr"

#define FRR_FULL_NAME   "FRRouting"
#define FRR_VERSION     "2.0" GIT_SUFFIX
#define FRR_VER_SHORT   "2.0"
#define FRR_BUG_ADDRESS "https://github.com/frrouting/frr/issues"
#define FRR_COPYRIGHT   "Copyright 1996-2005 Kunihiro Ishiguro, et al."
#define FRR_CONFIG_ARGS " '--host=mips-linux' '--disable-doc' '--disable-largefile' '--disable-shared' '--disable-ripngd' '--disable-bgpd' '--disable-irdp' '--disable-ospf6d' '--disable-ospfapi' '--disable-ospfclient' '--disable-bgp-announce' '--disable-backtrace' '--disable-time-check' '--disable-rusage' '--disable-pimd' '--disable-vtysh' '--enable-multipath=8' '--disable-ripd' '--disable-ospfd' '--enable-static' '--enable-user=root' '--enable-group=root' '--enable-fpm' 'host_alias=mips-linux' 'CC=rtk-ms-2.0.0-linux-gcc ' 'CFLAGS=-Os  -mips1 -msoft-float  -fPIC -Dlinux -D__linux__ -Dunix -DEMBED -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/include -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/lib/uClibc-0.9.33-485-200/include -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/lib/libcrypt  -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/lib/zlib -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux -idirafter /opt/toolchain/bin/../lib/gcc/mips-linux-uclibc/4.8.5/soft-float//include -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/linux-3.18.24.x/include -I/compile/realtek/pswitch/app/frr/../../plat/linux/ski -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/../../sdk/system/include -I/compile/realtek/pswitch/app/frr/../../../kernel/uClinux/../../sdk/include -I/compile/realtek/pswitch/app/frr/../../lib/ -g' 'LDFLAGS= -L/compile/realtek/pswitch/app/frr/../../lib/readline -L/compile/realtek/pswitch/app/frr/../../lib/ncurses/lib -L/compile/realtek/pswitch/app/frr/../../lib/json-c -ljson-c -L/compile/realtek/pswitch/app/frr/../../lib/libipc -lipc'"

#define FRR_DEFAULT_MOTD \
	"\r\n" \
	"Hello, this is " FRR_FULL_NAME " (version " FRR_VERSION ").\r\n" \
	FRR_COPYRIGHT "\r\n" \
	GIT_INFO "\r\n"

pid_t pid_output (const char *);

#endif /* _ZEBRA_VERSION_H */
