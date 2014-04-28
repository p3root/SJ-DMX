lib_LTLIBRARIES = libsjdmxu1.la

LIB_SRC = usbio.c

EXTRA_DIST = $(LIB_SRC)

AM_CPPFLAGS = -I/usr/local/include/libusb-1.0/
AM_LDFLAGS = -lusb-1.0

libsjdmxu1_la_CFLAGS = $(AM_CFLAGS)
libsjdmxu1_la_LDFLAGS = $(LTLDFLAGS)
libsjdmxu1_la_SOURCES = usbio.c

hdrdir = $(includedir)/libsjdmxu1-1.0
hdr_HEADERS = usbio.h
