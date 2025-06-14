# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#
# Blizzard III OpenGL Qt based rendering of scenes.

QT         += core gui widgets

greaterThan(QT_MAJOR_VERSION, 5) {
	QT += openglwidgets
}

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III Qt OpenGL renderer $$VERSION ***")

HEADERS += model/point.h \
	src-gen/MouseSelect.h \
	src-lib/sc_eventdriven.h \
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

FORMS += \
	mainwindow.ui

RESOURCES += \
	icons.qrc

INCLUDEPATH += $$PWD/src-lib $$PWD/model
LIBS        += -lb3SystemUnix -lb3System -lb3Image -lb3Base -lb3Raytrace
QMAKE_CLEAN += qrender

#####################################################################
#
# Extra YAKINDU SCT generate target
#
#####################################################################

generate.commands = ant
generate.depends  = MouseSelect.ysc qrender.sgen

QMAKE_EXTRA_TARGETS += generate
