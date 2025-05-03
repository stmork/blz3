# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#

TEMPLATE = subdirs
SUBDIRS += system_unix system base image raytrace
SUBDIRS += brt3 benc3 bimg3 half bexif3 render qrender qrender-sct
SUBDIRS += unittest longtest
CONFIG  += ordered

include(common.pri)

message("*** Blizzard III Qt version $$VERSION ***")

system_unix.file = system_unix/system_unix.pro
system.file      = system/system.pro
base.file        = base/base.pro
image.file       = image/image.pro
raytrace.file    = raytrace/raytrace.pro
brt3.file        = brt3/brt3.pro
benc3.file       = brt3/benc3.pro
bimg3.file       = brt3/bimg3.pro
half.file        = tools/half.pro
bexif3.file      = tools/bexif3.pro
render.file      = OpenGL/render.pro
qrender.file     = OpenGL/qrender.pro
qrender-sct.file = OpenGL/qrender-sct.pro
unittest.file    = unittest/unittest.pro
longtest.file    = unittest/longtest.pro

system.depends   = system_unix
base.depends     = system
image.depends    = system base
raytrace.depends = system base image

benc3.depends       = image
bimg3.depends       = image
half.depends        = image
bexif3.depends      = image
brt3.depends        = raytrace
render.depends      = raytrace
qrender.depends     = raytrace render
qrender-sct.depends = raytrace qrender

unittest.depends = system base image
longtest.depends = system base image

#####################################################################
#
# Extra target cppcheck.
#
#####################################################################

cppcheck.commands = cppcheck\
	--enable=style,warning,performance,portability\
	--inline-suppr\
	--suppress=preprocessorErrorDirective\
	--suppress=syntaxError\
	--suppress=duplInheritedMember\
	-I$$[QT_INSTALL_HEADERS]\
	-I$$BLZ3_INCLUDE -I$$BLZ3_HOME/include_unix\
	--language=c++ --std=c++14\
	--xml-version=2 --force -q -j `nproc`\
	--library=qt\
	system/*.cc system_unix/*.cc base/*.cc image/*.cc raytrace/*.cc\
	OpenGL/*.c* OpenGL/*.h brt3/*.cc 2>cppcheck.xml

QMAKE_EXTRA_TARGETS += cppcheck
QMAKE_CLEAN         += cppcheck.xml test-results*.xml *.wav *.dot *.mp4 *.deb *.qch

#####################################################################
#
# Extra target lcov
#
#####################################################################

lcov.commands += lcov --no-external -c --directory $$BLZ3_HOME -o coverage.info;
lcov.commands += genhtml coverage.info --legend --title=\"Blizzard III base libraries\" --output-directory $$BLZ3_LCOV --rc genhtml_med_limit=50

QMAKE_EXTRA_TARGETS += lcov
QMAKE_CLEAN         += $$BLZ3_LCOV
