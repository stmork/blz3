# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3System.a
LIB_SOURCE		=\
	b3Config.cc\
	b3Date.cc\
	b3Dir.cc\
	b3Display.cc\
	b3File.cc\
	b3Log.cc\
	b3Mem.cc\
	b3Row.cc\
	b3Thread.cc

LIB_OBJ			=\
	b3Config.o\
	b3Date.o\
	b3Dir.o\
	b3Display.o\
	b3File.o\
	b3Log.o\
	b3Mem.o\
	b3Row.o\
	b3Thread.o

all:	lib

depend:
#	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)
	gcc -M  $(LIB_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)
 
libB3System.a:	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

-include .dep
