# Blizzard III Unix System abstraction library

CONFIG   += c++14
TEMPLATE = lib
BLZ3_HOME = ../..
BLZ3_INCLUDE = $$BLZ3_HOME/include
BLZ3_LIB     = $$BLZ3_HOME/lib

unix
{
	INCLUDEPATH += $$BLZ3_HOME/include_unix
}

INCLUDEPATH += $$BLZ3_INCLUDE
TARGET       = b3SystemUnix

target.path = $$BLZ3_LIB

INSTALLS  += target

SOURCES += \
	b3CPU.cc \
	b3Config.cc \
	b3Dir.cc \
	b3DisplayView.cc \
	b3Error.cc \
	b3File.cc \
	b3Log.cc \
	b3Plugin.cc \
	b3Thread.cc \
	b3Time.cc

HEADERS += \
	../../include_unix/blz3/system/b3Assert.h \
	../../include_unix/blz3/system/b3Dir.h \
	../../include_unix/blz3/system/b3DisplayView.h \
	../../include_unix/blz3/system/b3Error.h \
	../../include_unix/blz3/system/b3File.h \
	../../include_unix/blz3/system/b3Log.h \
	../../include_unix/blz3/system/b3Mem.h \
	../../include_unix/blz3/system/b3Plugin.h \
	../../include_unix/blz3/system/b3Thread.h \
	../../include_unix/blz3/system/b3Time.h
