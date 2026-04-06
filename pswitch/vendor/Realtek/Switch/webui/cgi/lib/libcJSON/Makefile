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
PSWITCH_DIR = $(shell pwd)/../../../../../../..
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
LIBNAME  = libcJSON.so
SRCENV   = cJSON.sys
ENVCLEAN = envclean
LIBCLEAN = libclean
TARGET   = obj

#
# Define build flags
#
CFLAGS += -Werror -Wall -Wuninitialized -Wstrict-prototypes -Wstrict-prototypes
CFLAGS_EXTRA = $(CFLAGS_y) $(CFLAGS_m)

LIB     += -L$(SYS_INSTDIR)
INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)
INCLUDE += -I$(SYS_VENDOR_LIBDIR) -I$(SYS_VENDOR_MODDIR)

#
# Define build directories
#

#
# Define build sources
#
SRCS_y   = cJSON.c
SRCS_m   =

SRCS     = $(SRCS_y) $(SRCS_m)
OBJS     = $(shell for i in $(SRCS); do n=`basename $$i`; echo -n $(TARGET)/$${n%%.*}.o ""; done)

#
# Define build targets
#
all: $(LIBCLEAN) $(SRCENV)

ifdef CONFIG_SDK_PACKAGE
$(LIBNAME): $(OBJS)
	$(AR) rv $(LIBNAME) $(OBJS)
else
$(LIBNAME): $(OBJS)
	$(CC) $(LIB) -shared -Wl,-soname,$(LIBNAME).0 -o $(LIBNAME).0.0 $(OBJS)
	ln -sf $(LIBNAME).0.0 $(LIBNAME).0
	ln -sf $(LIBNAME).0 $(LIBNAME)
	$(SYSINSTALL) $@*
endif

$(SRCENV):
	@make $(LIBNAME)

$(LIBCLEAN):
	@rm -f $(LIBNAME)

$(TARGET)/%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS_EXTRA) $(CFLAGS) -fPIC -c -o $@ $<

romfs:
	$(SYSROMFSINST) $(LIBNAME).0.0 /lib
	ln -sf $(LIBNAME).0.0 $(ROMFSDIR)/lib/$(LIBNAME).0
	ln -sf $(LIBNAME).0 $(ROMFSDIR)/lib/$(LIBNAME)

clean:
	rm -f $(LIBNAME)* $(TARGET)/*

