#
# Current directory build
#
PSWITCH_DIR = $(shell pwd)/../..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(PSWITCH_DIR)/make.config.mk
include $(PSWITCH_DIR)/make.root.linux.mk

#
#RANLIB=ranlib

#SHELL = /bin/sh
srcdir = .

#INSTALL = /usr/bin/install -c
LIBNAME=libtacplus.so


#LIBS= $(USELIBS)

#CFLAGS=-O
#LDFLAGS=
INCLUDE += -I$(SYS_LIBINCLUDE) -I$(SYS_PLATINCLUDE) -I$(SYS_LXINCLUDE) -I$(SYS_PROTOINCLUDE)

#CC =

OBJS= tac_authen.o \
    tac_account.o \
    tac_author.o \
    tac_packet.o \
    tac_utils.o \
    tac_clnt.o \
    md5.o

all: $(LIBNAME)

#libtacplus.a: $(OBJS)
#	@rm -f $@
#	$(AR) rc $@ $(OBJS) $(LIBS)
#	$(RANLIB) $@
#	${CC} -o $@ ${OBJS} ${LDFLAGS} ${LIBS}


$(LIBNAME): $(OBJS)
	@rm -f $@
	$(LD) -shared -o $@.`cat VERSION` $(OBJS)
	ln -s $@.`cat VERSION` $@
	$(SYSINSTALL) $@*

.c.o:
	${CC} $(INCLUDE) -c ${CFLAGS} $< -o $@

clean:
	rm -f *.o $(LIBNAME)*

romfs:
	$(SYSROMFSINST) $(LIBNAME).`cat VERSION` /lib
	ln -sf $(LIBNAME).`cat VERSION` $(ROMFSDIR)/lib/$(LIBNAME)

