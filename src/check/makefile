# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CXX			= g++
LDLIBS			= -lB3System -lpthread
PRGS			= BlzDump
BLZDUMP_OBJS		= BlzDump.o
PRG_SOURCE		= BlzDump.cc
PRG_OBJS		= $(BLZDUMP_OBJS)

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- ${PRG_SOURCE}
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

BlzDump:	$(BLZDUMP_OBJS)
	$(LINK.cc) -o $@ $(BLZDUMP_OBJS) $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

-include .dep
