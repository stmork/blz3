# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC)

SIMPLE_OBJ		= simple.o
SIMPLE_SRC		= simple.cc
RENDER_OBJ		= render.o
RENDER_SRC		= render.cc
MATRIX_TEST_SRC		= matrix_test.cc
MATRIX_TEST_OBJ		= matrix_test.o
PRG_SOURCE		= $(SIMPLE_SRC) $(RENDER_SRC) $(MATRIX_TEST_SRC)

LDLIBS			= $(LIB_B3) $(LIB_OGL3) $(LIB_X3) $(LIB_S3)

PRGS			= simple render matrix_test

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) ALL QUIET

render:	$(RENDER_OBJ) $(BLZ3_LIB)/libB3Base.a $(BLZ3_LIB)/libB3Raytrace.a
	$(LINK.cc) -o $@ $(RENDER_OBJ) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

matrix_test:	$(MATRIX_TEST_OBJ)
	$(LINK.cc) -o $@ $(MATRIX_TEST_OBJ) $(LDLIBS)

-include .dep
