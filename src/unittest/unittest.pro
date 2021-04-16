# Blizzard III basic unittests.

QT       -= gui core
TEMPLATE  = app

include(../common.pri)

HEADERS += b3SkeletonTest.h \
	b3AlignTest.h \
	b3ArrayTest.h \
	b3AuxTest.h \
	b3ColorTest.h \
	b3CompareTest.h \
	b3ComplexTest.h \
	b3DataSizeTest.h \
	b3EndianTest.h \
	b3ErrorTest.h \
	b3ExceptionTest.h \
	b3FactoryTest.h \
	b3FFTTest.h \
	b3FileListTest.h \
	b3FileTest.h \
	b3FilterTest.h \
	b3ImageTest.h \
	b3ListTest.h \
	b3LocaleTest.h \
	b3MaterialTest.h \
	b3MathTest.h \
	b3MatrixTest.h \
	b3MemoryTest.h \
	b3PathTest.h \
	b3RandomTest.h \
	b3SamplerTest.h \
	b3SearchPathTest.h \
	b3SelfTestSuite.h \
	b3SplineTest.h \
	b3TimeTest.h \
	b3VectorTest.h \
	b3WorldTest.h

SOURCES += basetest.cc b3SkeletonTest.cc \
	b3AlignTest.cc \
	b3ArrayTest.cc \
	b3AuxTest.cc \
	b3ColorTest.cc \
	b3CompareTest.cc \
	b3ComplexTest.cc \
	b3DataSizeTest.cc \
	b3EndianTest.cc \
	b3ErrorTest.cc \
	b3ExceptionTest.cc \
	b3FactoryTest.cc \
	b3FFTTest.cc \
	b3FileListTest.cc \
	b3FileTest.cc \
	b3FilterTest.cc \
	b3ImageTest.cc \
	b3ListTest.cc \
	b3LocaleTest.cc \
	b3MaterialTest.cc \
	b3MathTest.cc \
	b3MatrixTest.cc \
	b3MemoryTest.cc \
	b3PathTest.cc \
	b3RandomTest.cc \
	b3SamplerTest.cc \
	b3SearchPathTest.cc \
	b3SelfTestSuite.cc \
	b3SplineTest.cc \
	b3TimeTest.cc \
	b3VectorTest.cc \
	b3WorldTest.cc \

LIBS        += -L$$BLZ3_LIB
LIBS        += -lb3Raytrace -lb3Base -lb3Image -lb3SystemUnix -lb3System
LIBS        += -lcppunit -ljpeg -ltiff -lGLU -ldl
QMAKE_CLEAN += *.jpg *.ps *.tiff *.tga *.rgb8 *.exr
QMAKE_CLEAN += test*.xml test*.bwd unittest

#####################################################################
#
# Extra target valgrind
#
#####################################################################

valgrind.commands = valgrind\
	--tool=memcheck --leak-check=full\
	--show-reachable=no --undef-value-errors=no --track-origins=no\
	--child-silent-after-fork=no --trace-children=no --gen-suppressions=no\
	--xml=yes --xml-file=valgrind.xml ./unittest

QMAKE_EXTRA_TARGETS += valgrind
QMAKE_CLEAN         += valgrind.xml
