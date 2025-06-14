# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#
# Blizzard III OpenGL rendering of scenes.

TEMPLATE  = app

include(../common.pri)

target.path = $$BLZ3_BIN
LIBS       += -L$$BLZ3_LIB
INSTALLS   += target

message("*** Blizzard III console OpenGL renderer $$VERSION ***")

SOURCES += render.cc

LIBS    += -lb3System -lb3SystemUnix -lb3Image -lb3Base -lb3Raytrace
LIBS    += $$LIBS_GLUT
