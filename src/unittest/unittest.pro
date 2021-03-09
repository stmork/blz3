# Blizzard III basic unittests.

TEMPLATE = app

include(../common.pri)

LIBS += -L$$BLZ3_LIB

HEADERS += \
	b3AlignTest.h \
	b3ArrayTest.h \
	b3ColorTest.h \
	b3ComplexTest.h \
	b3DataSizeTest.h \
	b3FFTTest.h \
	b3FileTest.h \
	b3ImageTest.h \
	b3ListTest.h \
	b3MathTest.h \
	b3MatrixTest.h \
	b3RandomTest.h \
	b3SkeletonTest.h \
	b3SplineTest.h \
	b3TimeTest.h \
	b3VectorTest.h \
	b3WorldTest.h

SOURCES += \
	b3AlignTest.cc \
	b3ArrayTest.cc \
	b3ColorTest.cc \
	b3ComplexTest.cc \
	b3DataSizeTest.cc \
	b3FFTTest.cc \
	b3FileTest.cc \
	b3ImageTest.cc \
	b3ListTest.cc \
	b3MathTest.cc \
	b3MatrixTest.cc \
	b3RandomTest.cc \
	b3SkeletonTest.cc \
	b3SplineTest.cc \
	b3TimeTest.cc \
	b3VectorTest.cc \
	b3WorldTest.cc \
	basetest.cc

LIBS += -lb3System -lb3SystemUnix -lb3Image -lb3Base -lcppunit -ljpeg -ltiff -lGLU
