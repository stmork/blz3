# Blizzard III  raytracing of scenes.

QT         -= gui core
TEMPLATE    = app

include(../common.pri)

target.path =   $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III image halfener $$VERSION ***")

SOURCES += half.cc

LIBS += -lb3System -lb3SystemUnix -lb3Image -lb3Base
