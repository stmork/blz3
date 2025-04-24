# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#
# Blizzard III Encoding animation images into mp4 video format.

QT         -= gui core
TEMPLATE    = app

include(../common.pri)

target.path =   $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III video stream builder $$VERSION ***")

SOURCES     += benc3.cc

LIBS        += -lb3System -lb3SystemUnix -lb3Base -lb3Image

QMAKE_CLEAN +=  *.mp4 *.mov *.hevc

#####################################################################
#
# Extra target valgrind
#
#####################################################################

valgrind.commands = valgrind\
	--tool=memcheck --leak-check=full\
	--show-reachable=no --undef-value-errors=no --track-origins=no\
	--child-silent-after-fork=no --trace-children=no --gen-suppressions=no\
	--xml=yes --xml-file=valgrind.xml ./benc3

QMAKE_EXTRA_TARGETS += valgrind
QMAKE_CLEAN         += valgrind.xml
