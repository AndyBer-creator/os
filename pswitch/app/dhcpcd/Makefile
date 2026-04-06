PSWITCH_DIR = $(shell pwd)/../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

EXEC = dhcpcd
OBJS = arp.o buildmsg.o cache.o client.o dhcpcd.o dhcpconfig.o peekfd.o signals.o udpipgen.o

CFLAGS += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE)
CFLAGS += -D_BSD_SOURCE=1 -g
ifeq ($(CONFIG_LINUXDIR), linux-3.18.24.x)
CFLAGS += -DCONFIG_SYS_KERNEL_LINUX_3_18_24_X
endif
LIB     += -lsal -lfds -lcfg -lcfgtbl -lcmd -lclicmd -lglue -lvty -lcrypt -L$(SYS_INSTDIR)
ifdef CONFIG_SYS_LIB_CUSTOM
LIB     += -lcustom
endif
ifdef CONFIG_SYS_LIB_TACPLUS
LIB     += -ltacplus
endif
ifdef CONFIG_SYS_LIB_RADIUS
LIB     += -lradius
endif
LIB += -lssl
LIB += -lcrypto

ifdef CONFIG_SYS_LIB_IPC
LIB     += -lipc
endif

ifdef CONFIG_SYS_LIB_UCI
LIB += -luci
endif

ifdef CONFIG_SYS_LIB_UBOX
LIB += -lubox
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(LIB) -o $@ $(OBJS) $(LDLIBS)
	$(SYSINSTALL) $@

romfs:
	$(SYSROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

