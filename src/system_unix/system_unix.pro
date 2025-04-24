# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#
# Blizzard III Unix System abstraction library.

QT         -= gui core
TEMPLATE    = lib

include(../common.pri)

target.path = $$BLZ3_LIB

TARGET      = b3SystemUnix
INSTALLS   += target

message("*** Blizzard III system abstraction library $$VERSION ***")

SOURCES += \
	b3CPU.cc \
	b3Dir.cc \
	b3DisplayView.cc \
	b3Error.cc \
	b3File.cc \
	b3Log.cc \
	b3Plugin.cc \
	b3Runtime.cc \
	b3Thread.cc \
	b3Time.cc

HEADERS += \
	../../include_unix/blz3/b3Config.h \
	../../include_unix/blz3/system/b3Assert.h \
	../../include_unix/blz3/system/b3Dir.h \
	../../include_unix/blz3/system/b3DisplayView.h \
	../../include_unix/blz3/system/b3Error.h \
	../../include_unix/blz3/system/b3File.h \
	../../include_unix/blz3/system/b3Log.h \
	../../include_unix/blz3/system/b3Mem.h \
	../../include_unix/blz3/system/b3Plugin.h \
	../../include_unix/blz3/system/b3Runtime.h \
	../../include_unix/blz3/system/b3Thread.h \
	../../include_unix/blz3/system/b3Time.h
