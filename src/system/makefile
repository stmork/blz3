# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3)
PRGS			= 
LIBS			= libB3system_ind.a
LIB_SOURCE =\
	b3Date.cc\
	b3Display.cc\
	b3Exception.cc\
	b3Row.cc\
	b3TimeAbstract.cc

LIB_OBJ	=\
	b3Date.o\
	b3Display.o\
	b3Exception.o\
	b3Row.o\
	b3TimeAbstract.o

all:	lib

depend:
#	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)
	gcc -M $(LIB_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.a *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)

$(LIBS):	$(LIB_OBJ)
	rm -f core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@

-include .dep
