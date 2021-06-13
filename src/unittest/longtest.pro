# Blizzard III long running unit tests.

QT       -= gui core
TEMPLATE  = app

include(../common.pri)

message("*** Blizzard III long running unit tests $$VERSION ***")

HEADERS += b3SkeletonTest.h \
	b3FFTTest.h \
	b3ImageSetup.h \
	b3ImageTest.h \
	b3MaterialTest.h \
	b3MovieTest.h \
	b3SamplerTest.h

SOURCES += basetest.cc b3SkeletonTest.cc \
	b3FFTTest.cc \
	b3ImageSetup.cc \
	b3ImageTest.cc \
	b3MaterialTest.cc \
	b3MovieTest.cc \
	b3SamplerTest.cc

LIBS        += -L$$BLZ3_LIB
LIBS        += -lb3Raytrace -lb3Base -lb3Image -lb3SystemUnix -lb3System
LIBS        += $$LIBS_CPPUNIT
QMAKE_CLEAN += *.jpg *.ps *.tiff *.tga *.rgb8 *.exr
QMAKE_CLEAN += *.avi *.mp4 *.mpg *.mov *.hevc *.mkv *.m2v *.webm
QMAKE_CLEAN += *test-results.xml test*.bwd longtest

#####################################################################
#
# Extra target valgrind
#
#####################################################################

valgrind.commands = valgrind\
	--suppressions=glib.supp\
	--suppressions=b3.supp\
	--suppressions=exiv2.supp\
	--tool=memcheck --leak-check=full\
	--show-reachable=no --undef-value-errors=no --track-origins=no\
	--child-silent-after-fork=no --trace-children=no --gen-suppressions=no\
	--xml=yes --xml-file=valgrind-longtest.xml ./longtest

QMAKE_EXTRA_TARGETS += valgrind
QMAKE_CLEAN         += valgrind.xml
