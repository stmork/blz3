# Blizzard III  image display tool

QT         -= gui core
TEMPLATE    = app

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III console image viewer $$VERSION ***")

SOURCES += bimg3.cc

LIBS += -lb3System -lb3SystemUnix -lb3Image -lb3Base -lb3Raytrace
