# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3)
PRGS			= 
LIBS			= libB3Image.a
LIB_SOURCE		=\
	b3Measure.cc\
	b3Tx.cc\
	b3TxBlit.cc\
	b3TxDeskew.cc\
	b3TxEasy.cc\
	b3TxGIF.cc\
	b3TxHist.cc\
	b3TxIFF.cc\
	b3TxIMG.cc\
	b3TxImage.cc\
	b3TxJPG.cc\
	b3TxLoadImage.cc\
	b3TxLoadTIFF.cc\
	b3TxPCX.cc\
	b3TxPool.cc\
	b3TxSaveJPEG.cc\
	b3TxSaveTIFF.cc\
	b3TxScale.cc\
	b3TxTGA.cc\
	b3TxTIFF.cc

LIB_OBJ			=\
	b3Measure.o\
	b3Tx.o\
	b3TxBlit.o\
	b3TxDeskew.o\
	b3TxEasy.o\
	b3TxGIF.o\
	b3TxHist.o\
	b3TxIFF.o\
	b3TxIMG.o\
	b3TxImage.o\
	b3TxJPG.o\
	b3TxLoadImage.o\
	b3TxLoadTIFF.o\
	b3TxPCX.o\
	b3TxPool.o\
	b3TxSaveJPEG.o\
	b3TxSaveTIFF.o\
	b3TxScale.o\
	b3TxTGA.o\
	b3TxTIFF.o

all:	lib

depend:
#	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)
	gcc -M  $(LIB_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)
 
libB3Image.a:	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

-include .dep
