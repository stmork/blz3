# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_OGL3) $(LIB_B3) $(LIB_X3) $(LIB_S3)
LIBS			= libB3xxx.a
PRGS			= simple render
SIMPLE_OBJ		= simple.o
SIMPLE_SOURCE		= simple.cc
RENDER_OBJ		= render.o
RENDER_SOURCE		= render.cc
PRG_SOURCE		= $(SIMPLE_SOURCE) $(RENDER_SRC)

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

render:	$(RENDER_OBJ)
	$(LINK.cc) -o $@ $(RENDER_OBJ) $(LDLIBS)
