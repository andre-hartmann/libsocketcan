# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

CONFIG -= qt

TARGET = libsocketcan-test

TEMPLATE = app

HEADERS = \
   $$PWD/include/can_netlink.h \
   $$PWD/include/libsocketcan.h

SOURCES = \
   $$PWD/src/libsocketcan.c \
   $$PWD/main.c

INCLUDEPATH = \
    $$PWD/include

#DEFINES =

