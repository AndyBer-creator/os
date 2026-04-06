#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

CFLAGS  += -O3 -Os -Werror -Wall -Wuninitialized -Wformat-security -Wno-format-zero-length -Wimplicit -Wno-strict-aliasing -Werror=old-style-declaration -Werror=ignored-qualifiers \
			-Wno-pointer-sign  -Wno-unused-variable -g -Wno-unused-function
INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)
INCLUDE += $(APP_INCLUDE)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) $(INCLUDE) $(OPTIMIZE) $(OBJS) $(LIB) -o $@
	$(SYSINSTALL) $@

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) -c $< $(INCLUDE) -o $@

romfs: $(EXEC) $(POSTREQUISITE)
	if [ ! -z $(EXEC) ]; then $(SYSROMFSINST) /bin/$(EXEC); fi

clean:
	rm -f *.o $(EXEC) $(OBJS) *.gdb *.elf *.elf2flt

.PHONY: clean romfs
