QT         += core gui widgets

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III Qt OpenGL renderer $$VERSION ***")

SOURCES += \
	b3CameraItem.cpp \
	b3LightItem.cpp \
	b3OpenGLWidget.cpp \
	qrender.cpp \
	mainwindow.cpp

HEADERS += \
	b3CameraItem.h \
	b3LightItem.h \
	b3OpenGLWidget.h \
	mainwindow.h

FORMS += \
	mainwindow.ui

LIBS  += -lb3SystemUnix -lb3System -lb3Image -lb3Base -lb3Raytrace -ljpeg -ltiff -lGLU

RESOURCES += \
	icons.qrc
