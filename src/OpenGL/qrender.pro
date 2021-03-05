QT       += core gui widgets

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III Qt OpenGL renderer $$VERSION ***")

SOURCES += \
	qrender.cpp \
	mainwindow.cpp

HEADERS += \
	mainwindow.h

FORMS += \
	mainwindow.ui

LIBS    += -lb3SystemUnix -lb3System -lb3Image -lb3Base -lb3Raytrace -ljpeg -ltiff -lGLU
