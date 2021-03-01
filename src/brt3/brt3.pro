# Blizzard III  raytracing of scenes.

TEMPLATE = app
BLZ3_HOME = ../..
include(../common.pri)

target.path = $$BLZ3_LIB
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III console raytracer $$VERSION ***")

SOURCES += brt3.cc

LIBS += -lb3System -lb3SystemUnix -lb3Image -lb3Base -lb3Raytrace -lcppunit -ljpeg -ltiff -lGLU
