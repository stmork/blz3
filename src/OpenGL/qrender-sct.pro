# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#
# Blizzard III Statechart unit test.

QT         -= core gui widgets

include(../common.pri)

QMAKE_CXXFLAGS  -= -Wsuggest-override
LIBS            += -L$$BLZ3_LIB

message("*** Blizzard III Qt OpenGL renderer statechart unittests $$VERSION ***")

HEADERS += \
	src-gen/MouseSelect.h \
	src-lib/sc_eventdriven.h \
	src-lib/sc_rxcpp.h \
	src-lib/sc_statemachine.h \
	src-lib/sc_types.h \
	src-lib/sc_runner.h

SOURCES += \
	src-gen/MouseSelect.cpp \
	src-test/MouseSelectTest.cpp \
	src-lib/sc_runner.cpp

INCLUDEPATH += $$PWD/src-gen
INCLUDEPATH += $$PWD/src-lib $$PWD/model
LIBS        += -lgtest -lgtest_main
QMAKE_CLEAN += test*.xml qrender-sct
