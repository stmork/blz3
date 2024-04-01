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

INCLUDEPATH += src-gen
INCLUDEPATH += src-lib model
LIBS        += -lgtest -lgtest_main
QMAKE_CLEAN += test*.xml qrender-sct
