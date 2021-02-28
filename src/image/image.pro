# Blizzard III Image processing library

CONFIG   += link_pkgconfig
TEMPLATE  = lib

BLZ3_HOME = ../..
include(../common.pri)

target.path = $$BLZ3_LIB

TARGET      = b3Image
PKGCONFIG   = OpenEXR
INSTALLS   += target

message("*** Blizzard III image processing library $$VERSION ***")

HEADERS += \
	$$BLZ3_INCLUDE/blz3/image/b3Sampler.h \
	$$BLZ3_INCLUDE/blz3/image/b3Tx.h \
	$$BLZ3_INCLUDE/blz3/image/b3TxMeasure.h \
	$$BLZ3_INCLUDE/blz3/image/b3TxPool.h \
	b3TxIFF.h \
	b3TxInclude.h \
	b3TxSaveInfo.h \
	b3TxTIFF.h

SOURCES += \
	b3Measure.cc \
	b3Sampler.cc \
	b3Tx.cc \
	b3TxBlit.cc \
	b3TxDeskew.cc \
	b3TxEXR.cc \
	b3TxEasy.cc \
	b3TxGIF.cc \
	b3TxHist.cc \
	b3TxIFF.cc \
	b3TxIMG.cc \
	b3TxImage.cc \
	b3TxInclude.cc \
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
	b3TxTGA.cc \
	b3TxTIFF.cc
