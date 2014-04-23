TARGET = SJDMXU1

TEMPLATE = lib

CONFIG += sharedlib

SOURCES += \
    usbio.c

HEADERS += \
    usbio.h


INCLUDEPATH += /usr/include/libusb-1.0
LIBS += -lusb-1.0

OTHER_FILES +=


