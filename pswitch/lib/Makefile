#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(CURDIR)/..
PSWITCH_USER_MODE = y
include $(PSWITCH_DIR)/make.path.mk
include $(SYS_ROOTDIR)/make.config.mk

dir_y 					:=
dir_$(CONFIG_SYS_LIB_NCURSES)		+= ncurses
dir_$(CONFIG_SYS_LIB_READLINE)		+= readline
dir_$(CONFIG_SYS_LIB_SSL)		+= libssl
dir_$(CONFIG_SYS_LIB_GLUE)		+= libglue
dir_$(CONFIG_SYS_LIB_FDS)		+= libfds
dir_$(CONFIG_SYS_LIB_SAL)		+= libsal
dir_$(CONFIG_SYS_LIB_VTY)		+= libvty
dir_$(CONFIG_SYS_LIB_CMD)		+= libcmd
dir_$(CONFIG_SYS_LIB_CMD)		+= libclicmd
dir_$(CONFIG_SYS_LIB_CFG)		+= libcfg
dir_$(CONFIG_SYS_LIB_CFG)		+= libcfgtbl
dir_$(CONFIG_SYS_LIB_TACPLUS)		+= libtacplus
dir_$(CONFIG_SYS_LIB_CUSTOM)            += libcustom
dir_$(CONFIG_SYS_LIB_RADIUS)	        += libradius
dir_$(CONFIG_SYS_L3_ROUTE)	        += json-c
dir_$(CONFIG_SYS_LIB_IPC)		+= libipc
dir_$(CONFIG_SYS_LIB_UCI)		+= libuci
dir_$(CONFIG_SYS_LIB_UBOX)		+= libubox
dir_$(CONFIG_SYS_LIB_UBUS)		+= libubus
dir_$(CONFIG_SYS_LIB_MICROXML)	+= libmicroxml 
dir_$(CONFIG_SYS_LIB_CURL)      += libcurl
#
# Define build targets
#
all:
	for i in $(dir_y)  ; do \
		$(MAKE) -C $$i || exit $? ; \
	done

romfs:
	for i in $(dir_y)  ; do \
		$(MAKE) -C $$i romfs || exit $? ; \
	done

clean:
	for i in $(dir_y)  ; do \
		$(MAKE) -C $$i clean ; \
	done

devclean:
	for i in $(dir_dev)  ; do \
		$(MAKE) -C $$i clean ; \
	done

