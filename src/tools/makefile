# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= -g
CXXFLAGS		= -g
LDLIBS			= -lB3System -lpthread
PRGS			= RemDepend RemCR
REMDEPEND_OBJS	= RemDepend.o
REMCR_OBJS		= RemCR.o
PRG_SOURCE		= RemDepend.cc RemCR.cc
PRG_OBJS		= $(REMDEPEND_OBJS) $(REMCR_OBJS)

all:	$(PRGS)

depend:
	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

RemDepend:	$(REMDEPEND_OBJS)
	$(LINK.cc) -o $@ $(REMDEPEND_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

RemCR:	$(REMCR_OBJS)
	$(LINK.cc) -o $@ $(REMCR_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)
