# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3Image.a
LIB_SOURCE		=\
	b3Tx.cc\
	b3TxBlit.cc\
	b3TxDeskew.cc\
	b3TxHist.cc\
	b3TxImage.cc\
	b3TxLoadTIFF.cc\
	b3TxSaveTIFF.cc\
	b3TxScale.cc\
	b3TxTIFF.cc\
	b3TxTool.cc

LIB_OBJ			=\
	b3Tx.o\
	b3TxBlit.o\
	b3TxDeskew.o\
	b3TxHist.o\
	b3TxImage.o\
	b3TxLoadTIFF.o\
	b3TxSaveTIFF.o\
	b3TxScale.o\
	b3TxTIFF.o\
	b3TxTool.o

all:	lib

depend:
	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)
 
libB3Image.a:	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)
