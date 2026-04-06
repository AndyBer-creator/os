#***********************************************************
#
# Copyright (C) 2010 Realtek Semiconductor Corp.
#
# Makefile -- Build instructions for switch global setting
#
#***********************************************************
# target
EXEC := msntp
OBJS := main.o unix.o internet.o socket.o timing.o

#app lib
LIB += -lm -lc

PSWITCH_DIR := $(shell pwd)/../..
include $(PSWITCH_DIR)/app/Makefile.app
