
PSWITCH_DIR = $(shell pwd)/../../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
PREFIX = $(ROOTDIR)/sqfs/home
else
PREFIX = $(ROOTDIR)/romfs/home
endif
WEBDIR = $(PREFIX)/web
CGIBINDIR = $(WEBDIR)/cgi-bin
ifdef CONFIG_SYS_PROTO_AUTHMGR
WEBAUTHDIR = $(PREFIX)/webauth
WEBAUTHCGIDIR = $(WEBAUTHDIR)/cgi
endif
ifdef CONFIG_SYS_UI_WEB_LOGIN_PAGE
BOACONF = login/boa.conf
else
BOACONF = boa.conf
endif

EXEC = boa
#OBJS = alias.o auth.o boa.o cgi.o cgi_header.o config.o get.o hash.o \
	globals.o \
	log.o nls.o pipe.o queue.o read.o request.o response.o signals.o \
	timestamp.o util.o crc32.o
OBJS = alias.o boa.o buffer.o cgi.o cgi_header.o config.o escape.o \
	get.o hash.o ip.o log.o mmap_cache.o pipe.o queue.o read.o \
	request.o response.o select.o signals.o util.o sublog.o ssl.o \
	socket.o virthost.o index.o timestamp.o boa_grammar.o boa_lexer.o\
	strutil.o cgi_ssl.o poll.o access.o action_cgi.o crc32.o

CFLAGS += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)

CFLAGS += -I$(LOADERDIR)/include
FLTFLAGS += -s 8192

EXTRALIBS = $(LIBCRYPT)

EXTRALIBS += -lpthread -lsal -lfds -lcfg -lcfgtbl -lcmd -lclicmd -lglue -lvty -lcrypt -lcustom -L$(SYS_INSTDIR)
ifdef CONFIG_SYS_AAA
ifdef CONFIG_SYS_LIB_TACPLUS
EXTRALIBS += -ltacplus
endif
endif
ifdef CONFIG_SYS_LIB_RADIUS
EXTRALIBS += -lradius
EXTRALIBS += -lssl
EXTRALIBS += -lcrypto
endif

ifdef CONFIG_SYS_LIB_IPC
EXTRALIBS += -lipc
endif

ifdef CONFIG_SYS_LIB_UCI
EXTRALIBS += -luci
endif

ifdef CONFIG_SYS_LIB_UBOX
EXTRALIBS += -lubox
endif

ifdef CONFIG_PROP_BOA_SECURITY_COUNT
CFLAGS += -DSECURITY_COUNTS
endif

ifdef CONFIG_USER_BOA_WITH_SSL
CFLAGS += -DSERVER_SSL=1 -I$(SYS_SSLINCLUDE)
EXTRALIBS += $(LIBSSL) $(LIBCRYPTO) -L$(SYS_INSTDIR)
endif

ifdef CONFIG_SYS_FW_SIGNATURE_CHECK
include $(LOADERDIR)/.config
CFLAGS += -DCONFIG_IH_MAGIC_NUMBER=0x$(CONFIG_IH_MAGIC_NUMBER)
endif

ifdef CONFIG_USER_TINYLOGIN_SHADOWPASSWDS
CFLAGS += -DSHADOW_AUTH
endif

ifdef CONFIG_USER_FLATFSD_FLATFSD
CFLAGS += -DCONFIG_USER_FLATFSD_FLATFSD=1
endif

ifdef CONFIG_PROP_AMAZON_BOA_LDAP
CFLAGS += -DLDAP_HACK
EXTRALIBS += -lldap -llber -static $(LIBSNAPGEAR) $(LIBCONFIG)
endif

ifdef CONFIG_USER_BOA_EMERG
CFLAGS += -DSYSLOGS_AT_EMERG
endif

ifdef CONFIG_USER_BOA_LOGGING
CFLAGS += -DBOA_TIME_LOG
endif

ifdef CONFIG_SYS_IPV6
CFLAGS += -DINET6
endif

all: touchit $(EXEC)

touchit:
	touch boa_grammar.c

$(EXEC): $(OBJS)
	$(CC) $(INCLUDE) $(LDFLAGS) -o $@ $(OBJS) $(EXTRALIBS) $(LDLIBS)
	$(SYSINSTALL) $@
	$(SYSINSTALL) $(BOACONF)
	$(SYSINSTALL) mime.types

romfs:
	$(SYSROMFSINST) /bin/$(EXEC)
	if test \! -e $(PREFIX); then mkdir $(PREFIX); fi
	if test \! -e $(WEBDIR); then mkdir $(WEBDIR); fi
	if test \! -e $(CGIBINDIR); then mkdir $(CGIBINDIR); fi
	if test \! -e $(ROMFSDIR)/etc/boa.conf; then cp $(BOACONF) $(ROMFSDIR)/etc; fi
	if test \! -e $(WEBDIR)/mime.types; then cp mime.types $(WEBDIR); fi
	chmod 755 $(CGIBINDIR);
ifdef CONFIG_SYS_PROTO_AUTHMGR
	if test \! -e $(WEBAUTHDIR); then mkdir $(WEBAUTHDIR); fi
	if test \! -e $(WEBAUTHCGIDIR); then mkdir $(WEBAUTHCGIDIR); fi
	if test \! -e $(ROMFSDIR)/etc/boa_webauth.conf; then cp boa_webauth.conf $(ROMFSDIR)/etc; fi
	if test \! -e $(WEBAUTHDIR)/mime.types; then cp mime.types $(WEBAUTHDIR); fi
endif
ifdef CONFIG_USER_BOA_WITH_SSL
	if test \! -e $(ROMFSDIR)/etc/group; then touch $(ROMFSDIR)/etc/group ; fi
endif
clean:
	-rm -f $(EXEC) *.gdb *.elf *.o

test:
	@echo $(INCLUDE)
