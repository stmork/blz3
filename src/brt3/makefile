# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)

BRT3_OBJS		= brt3.o
BIMG3_OBJS		= bimg3.o
BCONV3_OBJS		= bconv3.o
PRG_SOURCE		= brt3.cc bimg3.cc bconv3.cc

LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
IMGLIBS			= -lB3Base -lB3Image -lB3Base -lB3System -lB3Base -lB3tiff -lB3jpeg $(LIB_X3) $(LIB_S3)

PRGS			= brt3 bimg3 bconv3

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M  $(PRG_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

brt3:	$(BRT3_OBJS) $(BLZ3_LIB)/libB3Base.a $(BLZ3_LIB)/libB3Raytrace.a
	$(LINK.cc) -o $@ $(BRT3_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

bimg3:	$(BIMG3_OBJS) $(BLZ3_LIB)/libB3Base.a
	$(LINK.cc) -o $@ $(BIMG3_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

bconv3:	$(BCONV3_OBJS)
	$(LINK.cc) -o $@ $(BCONV3_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

-include .dep
