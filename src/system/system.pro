# Blizzard III System library

CONFIG   += c++14
TEMPLATE  = lib

BLZ3_HOME = ../..
BLZ3_INCLUDE = $$BLZ3_HOME/include
BLZ3_LIB     = $$BLZ3_HOME/lib

unix
{
	INCLUDEPATH += $$BLZ3_HOME/include_unix
}

INCLUDEPATH += $$BLZ3_INCLUDE
TARGET       = b3System

target.path = $$BLZ3_LIB

INSTALLS  += target

SOURCES += \
	b3CPUBase.cc \
	b3Date.cc \
	b3Display.cc \
	b3Exception.cc \
	b3Locale.cc \
	b3LogBase.cc \
	b3Memory.cc \
	b3PluginBase.cc \
	b3Row.cc \
	b3SelfTest.cc \
	b3SystemIndInclude.cc \
	b3TimeAbstract.cc
