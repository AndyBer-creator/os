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
PSWITCH_DIR = $(shell pwd)/../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

#
# Include architecture specific build rules
#

#
# Define build variables
#
LIBNAME  = libvty.so
TARGET   = obj

#
# Define build flags
#
OPTIM = -O3 -Os
CFLAGS_y += $(OPTIM) -Wall -Wformat-security -Wno-format-zero-length
CFLAGS_y += -Werror -Wimplicit -Wstrict-prototypes -Wno-strict-aliasing -g -Wno-unused-function
CFLAGS_EXTRA = $(CFLAGS_y) $(CFLAGS_m)

INCLUDE += -I. -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE)
INCLUDE += -I$(SYS_PLATINCLUDE) -I$(SYS_PROTOINCLUDE)

#
# Define build directories
#

#
# Define build sources
#
SRCS    += vector.c linklist.c command.c vty.c buffer.c memory.c hash.c thread.c

OBJS  = $(shell for i in $(SRCS); do n=`basename $$i`; echo -n $(TARGET)/$${n%%.*}.o ""; done)

#
# Define build targets
#
all: $(LIBNAME)

ifdef CONFIG_SDK_PACKAGE
$(LIBNAME): $(OBJS)
	$(AR) rv $(LIBNAME) $(OBJS)
else
$(LIBNAME): $(OBJS)
	$(CC) $(LIBS) -shared -Wl,-soname,$(LIBNAME).0 -o $(LIBNAME).0.0 $(OBJS)
	ln -sf $(LIBNAME).0.0 $(LIBNAME).0
	ln -sf $(LIBNAME).0 $(LIBNAME)
	$(SYSINSTALL) $@*
endif

$(TARGET)/%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS_EXTRA) $(CFLAGS) -fPIC -c -o $@ $<

romfs:
	$(SYSROMFSINST) $(LIBNAME).0.0 /lib
	ln -sf $(LIBNAME).0.0 $(ROMFSDIR)/lib/$(LIBNAME).0
	ln -sf $(LIBNAME).0 $(ROMFSDIR)/lib/$(LIBNAME)

clean:
	rm -f $(LIBNAME)* $(OBJS)

