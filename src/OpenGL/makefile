# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
LIBS			= libB3xxx.a
PRGS			= simple
SIMPLE_OBJ			= simple.o
SIMPLE_SOURCE		= simple.cc
PRG_SOURCE		= $(SIMPLE_SOURCE)

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

: $()
	$(LINK.cc) -o $@ $() $(LDLIBS)

lib:	$(LIBS)
 
libB3System.a:	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)
