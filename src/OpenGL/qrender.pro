QT         += core gui widgets

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III Qt OpenGL renderer $$VERSION ***")

SOURCES += \
	src-gen/MouseSelect.cpp \
	b3BBoxItem.cpp \
	b3CameraItem.cpp \
	b3CameraVolume.cpp \
	b3LightItem.cpp \
	b3OpenGLScrollArea.cpp \
	b3OpenGLWidget.cpp \
	b3SceneItem.cpp \
	qrender.cpp \
	mainwindow.cpp

HEADERS += point.h \
	src-gen/MouseSelect.h \
	src-lib/sc_statemachine.h \
	src-lib/sc_types.h \
	src-lib/sc_rxcpp.h \
	b3AbstractItem.h \
	b3BBoxItem.h \
	b3CameraItem.h \
	b3CameraVolume.h \
	b3LightItem.h \
	b3OpenGLScrollArea.h \
	b3OpenGLWidget.h \
	b3SceneItem.h \
	mainwindow.h

FORMS += \
	mainwindow.ui

LIBS  += -lb3SystemUnix -lb3System -lb3Image -lb3Base -lb3Raytrace -ljpeg -ltiff -lGLU

RESOURCES += \
	icons.qrc
