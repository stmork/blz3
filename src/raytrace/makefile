# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS                = $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3Raytrace.a
PRG_SOURCE		= b3Init.cc\
	b3Condition.cc b3Bump.cc b3Material.cc\
	b3Shape.cc b3BBox.cc\
	b3Animation.cc b3Light.cc b3Special.cc\
	b3Scene.cc
PRG_OBJ			= b3Init.o\
	b3Condition.o b3Bump.o b3Material.o\
	b3Shape.o b3BBox.o\
	b3Animation.o b3Light.o b3Special.o\
	b3Scene.o

all:	lib

depend:
	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)

libB3Raytrace.a:	$(PRG_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)
