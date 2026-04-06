#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR = $(CURDIR)/../../../../..
include $(PSWITCH_DIR)/make.config.mk

# target

LIBNAME  = libcustom.so
OBJS_y   = $(TARGET)/cfg_custom.o \
           $(TARGET)/cmd_custom.o \
           $(TARGET)/sal_custom.o \
           $(TARGET)/fds_custom.o
LIB_INCLUDE = -I$(SYS_GLUEINCLUDE)

# Put after OBJS
include $(PSWITCH_DIR)/lib/Makefile.lib
