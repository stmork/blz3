# Blizzard III Image processing library

QT         -= gui core
CONFIG     += link_pkgconfig
TEMPLATE    = lib

include(../common.pri)

target.path = $$BLZ3_LIB

TARGET      = b3Image
PKGCONFIG   = OpenEXR
INSTALLS   += target

message("*** Blizzard III image processing library $$VERSION ***")

HEADERS += \
	$$BLZ3_INCLUDE/blz3/image/b3Encoder.h \
	$$BLZ3_INCLUDE/blz3/image/b3EncoderBuffer.h \
	$$BLZ3_INCLUDE/blz3/image/b3Sampler.h \
	$$BLZ3_INCLUDE/blz3/image/b3Tx.h \
	$$BLZ3_INCLUDE/blz3/image/b3TxMeasure.h \
	$$BLZ3_INCLUDE/blz3/image/b3TxPool.h \
	b3EncoderStream.h \
	b3TxAlgorithms.h \
	b3TxIFF.h \
	b3TxSaveInfo.h

SOURCES += \
	b3Encoder.cc \
	b3EncoderBuffer.cc \
	b3EncoderStream.cc \
	b3Sampler.cc \
	b3Tx.cc \
	b3TxBlit.cc \
	b3TxEXR.cc \
	b3TxEasy.cc \
	b3TxGIF.cc \
	b3TxHist.cc \
	b3TxIFF.cc \
	b3TxIMG.cc \
	b3TxImage.cc \
	b3TxJPG.cc \
	b3TxLoadImage.cc \
	b3TxLoadTIFF.cc \
	b3TxMirror.cc \
	b3TxPCX.cc \
	b3TxPool.cc \
	b3TxSaveEXR.cc \
	b3TxSaveInfo.cc \
	b3TxSaveJPEG.cc \
	b3TxSavePS.cc \
	b3TxSaveRGB8.cc \
	b3TxSaveTGA.cc \
	b3TxSaveTIFF.cc \
	b3TxScale.cc \
	b3TxTGA.cc

LIBS       += -L$$BLZ3_LIB
LIBS       += -lb3Base
