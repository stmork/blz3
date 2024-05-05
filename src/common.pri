CONFIG  += c++17 link_pkgconfig

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

DEFINES                += QT_DISABLE_DEPRECATED_UP_TO=0x050f00
DEFINES                += B3_VERSION=\"\\\"$$VERSION\\\"\"

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
	DEFINES        += USE_GCOV
}

unix {
	INCLUDEPATH += $$BLZ3_HOME/include_unix
}

win32 {
	INCLUDEPATH += $$BLZ3_HOME/include_win32
}

INCLUDEPATH += $$BLZ3_INCLUDE
