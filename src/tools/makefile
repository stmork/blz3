# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CXX			= g++
LDLIBS			= -lB3System -lpthread
PRGS			= RemDepend RemCR BlzDump
REMDEPEND_OBJS	= RemDepend.o
REMCR_OBJS		= RemCR.o
BLZDUMP_OBJS	= BlzDump.o
PRG_SOURCE		= RemDepend.cc RemCR.cc BlzDump.cc
PRG_OBJS		= $(REMDEPEND_OBJS) $(REMCR_OBJS) $(BLZDUMP_OBJS)
#PRG_SOURCE		= ${PRG_OBJS:.o=.cc}

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- ${PRG_SOURCE}
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

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

BlzDump:	$(BLZDUMP_OBJS)
	$(LINK.cc) -o $@ $(BLZDUMP_OBJS) $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

-include .dep
