# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3)
PRGS			= 
LIBS			= libB3Base.a
PRG_SOURCE =\
	b3Aux.cc\
	b3Cubic.cc\
	b3Endian.cc\
	b3FileList.cc\
	b3Item.cc\
	b3ItemRegister.cc\
	b3ItemRegisterEntry.cc\
	b3Matrix.cc\
	b3Node.cc\
	b3Procedure.cc\
	b3Render.cc\
	b3SearchPath.cc\
	b3Spline.cc\
	b3World.cc

PRG_OBJ	=\
	b3Aux.o\
	b3Cubic.o\
	b3Endian.o\
	b3FileList.o\
	b3Item.o\
	b3ItemRegister.o\
	b3ItemRegisterEntry.o\
	b3Matrix.o\
	b3Node.o\
	b3Procedure.o\
	b3Render.o\
	b3SearchPath.o\
	b3Spline.o\
	b3World.o

all:	lib

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M $(PRG_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)

libB3Base.a:	$(PRG_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

-include .dep
