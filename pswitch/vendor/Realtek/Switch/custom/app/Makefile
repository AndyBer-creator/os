#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
PSWITCH_DIR := $(shell pwd)/../../../../..
include $(PSWITCH_DIR)/make.config.mk

# target
EXEC :=
OBJS :=

#app special include
APP_INCLUDE +=

#app lib
LIB     += -L$(SYS_INSTDIR)

include $(PSWITCH_DIR)/app/Makefile.app
