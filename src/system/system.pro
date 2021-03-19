# Blizzard III System library

TEMPLATE    = lib

include(../common.pri)

target.path = $$BLZ3_LIB

TARGET      = b3System
INSTALLS   += target

message("*** Blizzard III system access library $$VERSION ***")

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

HEADERS += \
	$$BLZ3_INCLUDE/blz3/system/b3Atomic.h \
	$$BLZ3_INCLUDE/blz3/system/b3CPUBase.h \
	$$BLZ3_INCLUDE/blz3/system/b3Date.h \
	$$BLZ3_INCLUDE/blz3/system/b3DirAbstract.h \
	$$BLZ3_INCLUDE/blz3/system/b3Display.h \
	$$BLZ3_INCLUDE/blz3/system/b3ErrorAbstract.h \
	$$BLZ3_INCLUDE/blz3/system/b3Exception.h \
	$$BLZ3_INCLUDE/blz3/system/b3FileAbstract.h \
	$$BLZ3_INCLUDE/blz3/system/b3Locale.h \
	$$BLZ3_INCLUDE/blz3/system/b3LogBase.h \
	$$BLZ3_INCLUDE/blz3/system/b3Memory.h \
	$$BLZ3_INCLUDE/blz3/system/b3MutexAbstract.h \
	$$BLZ3_INCLUDE/blz3/system/b3PluginBase.h \
	$$BLZ3_INCLUDE/blz3/system/b3SelfTest.h \
	$$BLZ3_INCLUDE/blz3/system/b3ThreadAbstract.h \
	$$BLZ3_INCLUDE/blz3/system/b3TimeAbstract.h \
	$$BLZ3_INCLUDE/blz3/system/b3TimeStop.h
