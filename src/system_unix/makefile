# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3)
PRGS			= 
LIBS			= libB3System.a
LIB_SOURCE		=\
	b3Config.cc\
	b3Date.cc\
	b3Dir.cc\
	b3DisplayView.cc\
	b3File.cc\
	b3Log.cc\
	b3Thread.cc\
	b3Time.cc

LIB_OBJ			=\
	b3Config.o\
	b3Date.o\
	b3Dir.o\
	b3DisplayView.o\
	b3File.o\
	b3Log.o\
	b3Thread.o\
	b3Time.o

all:	lib

depend:
#	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)
	gcc -M  $(LIB_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.a *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)
 
$(LIBS):	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@

-include .dep
