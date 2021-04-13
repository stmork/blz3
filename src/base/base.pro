# Blizzard III Basic algorithm library

CONFIG   -= gui core
TEMPLATE  = lib

include(../common.pri)

target.path = $$BLZ3_LIB
TARGET      = b3Base
INSTALLS   += target

message("*** Blizzard III basic algorithm library $$VERSION ***")
message("$$BLZ3_LIB")

HEADERS += \
	$$BLZ3_INCLUDE/blz3/base/b3Array.h \
	$$BLZ3_INCLUDE/blz3/base/b3Aux.h \
	$$BLZ3_INCLUDE/blz3/base/b3Clouds.h \
	$$BLZ3_INCLUDE/blz3/base/b3Color.h \
	$$BLZ3_INCLUDE/blz3/base/b3ColorSSE.h \
	$$BLZ3_INCLUDE/blz3/base/b3ColorStd.h \
	$$BLZ3_INCLUDE/blz3/base/b3Compare.h \
	$$BLZ3_INCLUDE/blz3/base/b3Complex.h \
	$$BLZ3_INCLUDE/blz3/base/b3Complex64.h \
	$$BLZ3_INCLUDE/blz3/base/b3Cubic.h \
	$$BLZ3_INCLUDE/blz3/base/b3Endian.h \
	$$BLZ3_INCLUDE/blz3/base/b3FFT.h \
	$$BLZ3_INCLUDE/blz3/base/b3FileList.h \
	$$BLZ3_INCLUDE/blz3/base/b3FileMem.h \
	$$BLZ3_INCLUDE/blz3/base/b3Filter.h \
	$$BLZ3_INCLUDE/blz3/base/b3List.h \
	$$BLZ3_INCLUDE/blz3/base/b3Math.h \
	$$BLZ3_INCLUDE/blz3/base/b3Matrix.h \
	$$BLZ3_INCLUDE/blz3/base/b3MultiSample.h \
	$$BLZ3_INCLUDE/blz3/base/b3OceanWave.h \
	$$BLZ3_INCLUDE/blz3/base/b3Pick.h \
	$$BLZ3_INCLUDE/blz3/base/b3Procedure.h \
	$$BLZ3_INCLUDE/blz3/base/b3Random.h \
	$$BLZ3_INCLUDE/blz3/base/b3Render.h \
	$$BLZ3_INCLUDE/blz3/base/b3RenderTypes.h \
	$$BLZ3_INCLUDE/blz3/base/b3SearchPath.h \
	$$BLZ3_INCLUDE/blz3/base/b3Spline.h \
	$$BLZ3_INCLUDE/blz3/base/b3UndoOperation.h \
	$$BLZ3_INCLUDE/blz3/base/b3Vector.h \
	$$BLZ3_INCLUDE/blz3/base/b3VectorBufferObjects.h \
	$$BLZ3_INCLUDE/blz3/base/b3VertexBuffer.h \
	$$BLZ3_INCLUDE/blz3/base/b3Water.h \
	$$BLZ3_INCLUDE/blz3/base/b3Wood.h \
	$$BLZ3_INCLUDE/blz3/base/b3World.h \
	b3ItemRegister.h

SOURCES += \
	b3Aux.cc \
	b3Clouds.cc \
	b3Color.cc \
	b3Cubic.cc \
	b3Endian.cc \
	b3FFT.cc \
	b3FileList.cc \
	b3FileMem.cc \
	b3Filter.cc \
	b3Item.cc \
	b3ItemRegister.cc \
	b3ItemRegisterEntry.cc \
	b3Math.cc \
	b3Matrix.cc \
	b3MultiSample.cc \
	b3OceanWave.cc \
	b3Pick.cc \
	b3Procedure.cc \
	b3Render.cc \
	b3RenderContext.cc \
	b3SearchPath.cc \
	b3UndoOperation.cc \
	b3VectorBufferObjects.cc \
	b3VertexBuffer.cc \
	b3Water.cc \
	b3Wood.cc \
	b3World.cc
