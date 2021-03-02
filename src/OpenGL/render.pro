# Blizzard III  OpenGL rendering of scenes.

TEMPLATE  = app

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III console OpenGL renderer $$VERSION ***")

SOURCES += render.cc

LIBS += -lb3System -lb3SystemUnix -lb3Image -lb3Base -lb3Raytrace -lb3Raytrace -lcppunit -ljpeg -ltiff -lGLU -lglut
