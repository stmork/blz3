# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3)
PRGS			= 
LIBS			= libB3System_ind.o
LIB_SOURCE =\
	b3Date.cc\
	b3Display.cc\
	b3Exception.cc\
	b3LogBase.cc\
	b3Row.cc\
	b3TimeAbstract.cc

LIB_OBJ	=\
	b3Date.o\
	b3Display.o\
	b3Exception.o\
	b3LogBase.o\
	b3Row.o\
	b3TimeAbstract.o

all:	$(LIB_OBJ)

depend:
#	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)
	gcc -M $(LIB_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.a *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

-include .dep
