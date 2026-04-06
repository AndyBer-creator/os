PSWITCH_DIR = $(shell pwd)/../../
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

#
# uClinux-dist makefile for openssh
# davidm@snapgear.com
#

CONFOPTS =
CONFOPTS += --host=msdk-4.8.5-3.18-linux
CONFOPTS += --prefix=
CONFOPTS += --sbindir=/bin
CONFOPTS += --disable-etc-default-login
CONFOPTS += --with-ssl-dir=$(SYS_LIB_SSLDIR)

-include $(ROOTDIR)/$(LIBCDIR)/.config
ifdef HAS_SHADOW
CONFOPTS += --with-shadow
else
CONFOPTS += --without-shadow
endif

ifeq ($(CONFIG_LINUXDIR), linux-3.18.24.x)
CONFOPTS += --with-zlib=$(ROOTDIR)/lib
endif

# CONFOPTS += --disable-shared
# CONFOPTS += --disable-static

ifdef CONFIG_USER_FLATFSD_FLATFSD
CONFOPTS += --sysconfdir=/etc/config
else
CONFOPTS += --sysconfdir=/etc
endif

# If we're building with libpam, we add this capability to ssh
ifdef CONFIG_LIB_LIBPAM
NEED_LIB_PAM := y
endif
ifdef CONFIG_LIB_LIBPAM_FORCE
NEED_LIB_PAM := y
endif
ifeq ($(NEED_LIB_PAM),y)
CONFOPTS += --with-pam
endif

FLTFLAGS += -s 65536
ifdef CONFIG_PROP_SSH_SECURITY_COUNT
CFLAGS += -DSECURITY_COUNTS
endif
ifeq ($(CONFIG_USER_SSH_SSHD),y)
CFLAGS += -DINCLUDE_SSHD
endif
ifeq ($(CONFIG_USER_SSH_SSH),y)
CFLAGS += -DINCLUDE_SSH
endif
ifeq ($(CONFIG_USER_CLI),y)
CFLAGS += -D__CLI__
endif
ifeq ($(CONFIG_RTL8389),y)
CFLAGS += -D__RTL8389__
endif
CFLAGS += -g -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE) -I$(SYS_GLUEINCLUDE)
CONFIGURE_OPTS += --disable-largefile --disable-lastlog --disable-utmp --disable-utmpx --disable-wtmp --disable-wtmpx --disable-pututline --disable-pututxline


all: build/build
	$(MAKE) -C build
	$(SYSINSTALL) build/sshd build/ssh-keygen sshd_config

build/build: makefile
	rm -rf build
	mkdir build
	(cd build; sh ../configure $(CONFIGURE_OPTS) $(CONFOPTS))
	touch build/build

clean:
	rm -rf build

romfs:
	$(SYSROMFSINST) -e CONFIG_USER_SSH_SSH build/ssh /bin/ssh
	if test \! -e $(ROMFSDIR)/etc/default; then mkdir $(ROMFSDIR)/etc/default; fi
	$(ROMFSINST) -e CONFIG_USER_SSH_SSH /etc/default/ssh_config
	: $(ROMFSINST) -e CONFIG_USER_SSH_SSH -p 700 -d /.ssh
	$(SYSROMFSINST) -e CONFIG_USER_SSH_SSHD build/sshd /bin/sshd
#	$(ROMFSINST) -e CONFIG_USER_SSH_SSHD /etc/default/sshd_config
	$(ROMFSINST) -e CONFIG_USER_SSH_SSHD /etc/sshd_config
	$(SYSROMFSINST) -e CONFIG_USER_SSH_SSHKEYGEN build/ssh-keygen /bin/ssh-keygen
	$(SYSROMFSINST) -e CONFIG_USER_SSH_SCP build/scp /bin/scp
	$(SYSROMFSINST) -e CONFIG_USER_SSH_SFTP build/sftp /bin/sftp
	if test \! -e $(ROMFSDIR)/etc/ssh; then mkdir $(ROMFSDIR)/etc/ssh; fi

ifndef CONFIG_PROP_CONFIG_LIBCONFIG
	##
	# Add the appropriate stuff to the start files.
	#
#	$(ROMFSINST) -e CONFIG_USER_SSH_SSHKEYGEN \
#		-a "gen-keys &" /etc/default/start
#	$(ROMFSINST) -e CONFIG_USER_SSH_SSHKEYGEN \
#		-a "gen-keys &" /etc/rc
#	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)$(CONFIG_USER_SSH_SSH)" \
#		-A "root:" -a "root:abTxyM1SdxNko:0:0:Root:/:/bin/sh" \
#		/etc/default/passwd
	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)$(CONFIG_USER_SSH_SSH)" \
		-A "root:" -a "root::0:0:root:/:/bin/cli" \
		/etc/passwd
#	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)$(CONFIG_USER_SSH_SSH)" \
#		-A "root:" -a "root:x:0:root" /etc/default/group
	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)$(CONFIG_USER_SSH_SSH)" \
		-A "root:" -a "root:x:0:root" /etc/group
#	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)" \
#		-A "sshd:" -a "sshd:x:100:65534::/var/run/sshd:/bin/false" \
#		/etc/default/passwd
	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)" \
		-A "sshd:" -a "sshd::100:65534::/var/run/sshd:/bin/false" \
		/etc/passwd
#	$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)" \
#		-A "nobody:" -a "nobody:x:65534:" /etc/default/group
	 $(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)" \
		-A "nobody:" -a "nobody:x:65534:" /etc/group
endif
ifeq ($(CONFIG_IPV6),y)
	$(ROMFSINST) -e CONFIG_USER_SSH_SSHD \
		-a "ssh     stream tcp6 nowait root /bin/sshd -i" /etc/inetd.conf
else
	$(ROMFSINST) -e CONFIG_USER_SSH_SSHD \
		-a "ssh     stream tcp nowait root /bin/sshd -i" /etc/inetd.conf
endif
	##
	# Optionally add the startup stuff to the /etc/default-static dir.
	#
	if [ -d $(ROMFSDIR)/etc/default-static ] ; then \
		$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)$(CONFIG_USER_SSH_SSH)" \
			-a "root:abTxyM1SdxNko:0:0:Root:/:/bin/sh" -A "root:"\
			/etc/default-static/passwd ; \
		$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)$(CONFIG_USER_SSH_SSH)" \
			-A "root:" -a "root:x:0:root" /etc/default-static/group ; \
		$(ROMFSINST) -e CONFIG_USER_SSH_SSH /etc/default-static/ssh_config ; \
		$(ROMFSINST) -e CONFIG_USER_SSH_SSHD /etc/default-static/sshd_config ; \
		$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)" \
			-A "sshd:" -a "sshd:x:100:65534::/var/run/sshd:/bin/false" \
			/etc/default-static/passwd ; \
		$(ROMFSINST) -o "$(CONFIG_USER_SSH_SSHD)" \
			-A "nobody:" -a "nobody:x:65534:" /etc/default-static/group ;\
	fi


