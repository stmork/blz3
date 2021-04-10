CONFIG  += c++17

BLZ3_HOME = $$(BLZ3_HOME)

isEmpty(BLZ3_HOME) {
	BLZ3_HOME = ../..
	message("Setting BLZ3_HOME to parent directory.")
}

BLZ3_INCLUDE = $$BLZ3_HOME/include
BLZ3_LIB     = $$BLZ3_HOME/lib
BLZ3_BIN     = $$BLZ3_HOME/bin
BLZ3_LCOV    = $$BLZ3_HOME/lcov-out

include(flags.pri)

QMAKE_CXXFLAGS         += -std=c++17 # qmake 5.9.x does not support C++17 :-(
QMAKE_CXXFLAGS         += -Wextra -Wshadow -Wsuggest-override
QMAKE_CXXFLAGS_RELEASE  = $$replace(QMAKE_CXXFLAGS_RELEASE,"-O2","-O3")
QMAKE_CLEAN            += *.gcno *.gcda coverage.info

linux:CONFIG(debug) {
	DEFINES        += _DEBUG
}

linux:CONFIG(gcov) {
	QMAKE_CXXFLAGS += --coverage
	QMAKE_LFLAGS   += --coverage
	LIBS           += -lgcov
}

unix {
	INCLUDEPATH += $$BLZ3_HOME/include_unix
}

win32 {
	INCLUDEPATH += $$BLZ3_HOME/include_win32
}

INCLUDEPATH += $$BLZ3_INCLUDE
