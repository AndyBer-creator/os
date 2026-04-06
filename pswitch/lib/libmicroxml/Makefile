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

CFLAGS  += -Werror -Wall -Wuninitialized -Wstrict-prototypes -Wno-strict-aliasing -Wno-format-zero-length -O3 -Os -g
#-Wformat-security
INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)
INCLUDE += -I$(LOADERDIR)/include/pswitch -I$(SYS_SSLINCLUDE) -I$(SYS_GLUEINCLUDE) -I$(FLASH_INCDIR)

# target
LIBNAME  = libmicroxml.a
TARGET  = obj

OBJS_y :=
OBJS_m :=

OBJS_y +=  $(TARGET)/mxml-attr.o \
           $(TARGET)/mxml-entity.o \
           $(TARGET)/mxml-file.o \
           $(TARGET)/mxml-get.o \
           $(TARGET)/mxml-index.o \
           $(TARGET)/mxml-node.o \
           $(TARGET)/mxml-private.o \
           $(TARGET)/mxml-search.o \
           $(TARGET)/mxml-set.o \
           $(TARGET)/mxml-string.o

OBJS := $(OBJS_y) $(OBJS_m)

.PHONY: clean romfs

all: $(LIBNAME)

$(LIBNAME):  $(OBJS)
	$(AR) rv $(LIBNAME) $(OBJS)
	$(STRIP) --strip-debug -R .note -R .comment $@
	$(SYSINSTALL) $(LIBNAME)

$(TARGET)/%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) $(CFLAGS_EXTRA) -fPIC -c -o $@ $<

romfs:

clean:
	rm -f $(shell for i in $(SRCS_SYS); do n=`basename $$i`; echo -n $${n%%.*}.c ""; done)
	rm -f $(LIBNAME)* $(TARGET)/*

