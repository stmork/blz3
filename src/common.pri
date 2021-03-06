CONFIG  += c++17
VERSION  = 3.1.0

BLZ3_HOME = $$(BLZ3_HOME)

isEmpty(BLZ3_HOME) {
	BLZ3_HOME = ../..
	message("Setting BLZ3_HOME to parent directory.")
}

BLZ3_INCLUDE = $$BLZ3_HOME/include
BLZ3_LIB     = $$BLZ3_HOME/lib
BLZ3_BIN     = $$BLZ3_HOME/bin

QMAKE_CXXFLAGS_DEBUG   += -Wsuggest-override
QMAKE_CXXFLAGS_RELEASE  = $$replace(QMAKE_CXXFLAGS_RELEASE,"-O2","-O3")
QMAKE_CXXFLAGS_RELEASE += -Ofast

unix
{
	INCLUDEPATH += $$BLZ3_HOME/include_unix
}

win32
{
	INCLUDEPATH += $$BLZ3_HOME/include_win32
}

INCLUDEPATH += $$BLZ3_INCLUDE
