TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c


INCLUDEPATH += /usr/include/libusb-1.0
LIBS += -lusb-1.0

unix:!macx: LIBS += -L$$OUT_PWD/../../src/SJDMXU1/ -lSJDMXU1

INCLUDEPATH += $$PWD/../../src/SJDMXU1
DEPENDPATH += $$PWD/../../src/SJDMXU1
