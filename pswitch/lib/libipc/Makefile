#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(CURDIR)/../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

ifdef CONFIG_SYS_FW_SIGNATURE_CHECK
include $(LOADERDIR)/.config
CFLAGS += -DCONFIG_IH_MAGIC_NUMBER=0x$(CONFIG_IH_MAGIC_NUMBER)
endif

CFLAGS  += -Werror -Wall -Wuninitialized -Wstrict-prototypes -Wno-strict-aliasing -Wno-format-zero-length  -Werror=old-style-declaration -Werror=ignored-qualifiers -Wno-unused-function -O3 -Os -g
#-Wformat-security
INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)
INCLUDE += -I$(LOADERDIR)/include/pswitch -I$(SYS_SSLINCLUDE) -I$(SYS_GLUEINCLUDE) -I$(FLASH_INCDIR) -I$(SYS_SALINCLUDE) 

# target
LIBNAME  = libipc.so
TARGET  = obj

OBJS_y :=
OBJS_m :=

OBJS_y +=  $(TARGET)/vosMsg.o \
		   $(TARGET)/smdMsg.o \
		   $(TARGET)/smdMsgRequest.o \
		   $(TARGET)/smdCmdWrapApp.o \
		   $(TARGET)/lstLib.o \
		   $(TARGET)/vosMemoryUs.o \
		   #$(TARGET)/hash.o \
		   #$(TARGET)/pqueue.o \
		   #$(TARGET)/thread.o \
		   #$(TARGET)/log.o \
		   
#OBJS_y  += $(TARGET)/sal_init.o \

#OBJS_$(CONFIG_SYS_PORT_SECURITY)        += $(TARGET)/sal_psecure.o

OBJS := $(OBJS_y) $(OBJS_m)

.PHONY: clean romfs prerequisite

all: prerequisite

$(LIBNAME):  $(OBJS)
	$(CC) $(LIB) -shared -Wl,-soname,$(LIBNAME).0 -o $(LIBNAME).0.0 $(OBJS)
	ln -sf $(LIBNAME).0.0 $(LIBNAME).0
	ln -sf $(LIBNAME).0 $(LIBNAME)
	$(SYSINSTALL) $(LIBNAME)*

$(TARGET)/%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) $(CFLAGS_EXTRA) -fPIC -c -o $@ $<

romfs:
	$(SYSROMFSINST) $(LIBNAME).0.0 /lib
	ln -sf $(LIBNAME).0.0 $(ROMFSDIR)/lib/$(LIBNAME).0
	ln -sf $(LIBNAME).0 $(ROMFSDIR)/lib/$(LIBNAME)

clean:
	rm -f $(shell for i in $(SRCS_SYS); do n=`basename $$i`; echo -n $${n%%.*}.c ""; done)
	rm -f $(LIBNAME)* $(TARGET)/*

#SRCS_SYS = $(SYS_PLAT_COMMONDIR)/sys_error.c \

prerequisite:
	if [ ! -e $(LIBNAME) ]; then \
		for i in $(SRCS_SYS); do n=`basename $$i`; ln -sf $$i $${n%%.*}.c; done; \
	fi
	rm -f $(LIBNAME)*
	make $(LIBNAME)
