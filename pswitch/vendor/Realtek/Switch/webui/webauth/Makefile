#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************

#
# Current directory build
#
ifndef PSWITCH_DIR
PSWITCH_DIR = $(shell pwd)/../../../../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk
endif

#
# Include architecture specific build rules
#

#
# Define build variables
#
ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
PREFIX = $(ROOTDIR)/sqfs/home
else
PREFIX = $(ROOTDIR)/romfs/home
endif
WEBDIR = $(PREFIX)/webauth
CGIBINDIR = $(WEBDIR)/cgi

#
# Define build flags
#
FLTFLAGS += -s 262143
CFLAGS += -Werror -Wall -Wuninitialized -Wstrict-prototypes -Wstrict-prototypes
CFLAGS_y =

CFLAGS_EXTRA = $(CFLAGS_y) $(CFLAGS_m)

INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_APPINCLUDE) -I$(SYS_PROTOINCLUDE)
INCLUDE += -I$(LOADERDIR)/include
LIB += -lsal -lfds -lcfg -lcfgtbl -lcmd -lclicmd -lglue -lvty -lcrypt -L$(SYS_INSTDIR)
ifdef CONFIG_SYS_LIB_TACPLUS
LIB += -ltacplus
endif
ifdef CONFIG_SYS_LIB_RADIUS
LIB += -lradius
endif
LIB += -lssl
LIB += -lcrypto

ifdef CONFIG_SYS_LIB_CUSTOM
INCLUDE += -I$(SYS_VENDOR_LIBDIR) -I$(SYS_VENDOR_MODDIR)
LIB     += -lcustom
endif
ifdef CONFIG_SYS_LIB_IPC
LIB     += -lipc
endif

ifdef CONFIG_SYS_LIB_UCI
LIB += -luci
endif

ifdef CONFIG_SYS_LIB_UBOX
LIB += -lubox
endif

#
# Define build directories
#

#
# Define build sources
#

#UTIL_SRC = web_util.c
WEBAUTH_CGI_SRC = webauth.c webauth_util.c
WEBAUTH_LOGIN_CGI_SRC = webauth_login.c webauth_util.c

WEBAUTH_CGI_OBJ = $(WEBAUTH_CGI_SRC:.c=.o)
WEBAUTH_LOGIN_CGI_OBJ = $(WEBAUTH_LOGIN_CGI_SRC:.c=.o)

WEBAUTH_CGI_BIN = webauth.cgi
WEBAUTH_LOGIN_CGI_BIN = webauth_login.cgi

#
# Define build targets
#

all: $(WEBAUTH_CGI_BIN) $(WEBAUTH_LOGIN_CGI_BIN)

$(WEBAUTH_CGI_BIN): $(WEBAUTH_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(WEBAUTH_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@

$(WEBAUTH_LOGIN_CGI_BIN): $(WEBAUTH_LOGIN_CGI_OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(WEBAUTH_LOGIN_CGI_OBJ) $(LIB) -o $@
	$(SYSINSTALL) $@

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) -c $< $(INCLUDE) -o $@

installweb:
	if test \! -e $(PREFIX); then mkdir $(PREFIX); fi
	if test \! -e $(WEBDIR); then mkdir $(WEBDIR); fi
	rm -f $(WEBDIR)/tmp
	ln -sf /tmp $(WEBDIR)/tmp
	cp -rf web/* $(WEBDIR);

installcgi: $(CGI_BIN)
	if test \! -e $(PREFIX); then mkdir $(PREFIX); fi
	if test \! -e $(CGIBINDIR); then mkdir $(CGIBINDIR); fi
	chmod 755 $(CGIBINDIR); cp *.cgi $(CGIBINDIR)

install: installweb installcgi

romfs: install

clean:
	rm -f *.o *.cgi *.gdb *.elf *.elf2flt

tgz: clean
	cd ..; tar zcvf webcgi.tgz webcgi

