# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
PRGS			= bench
BENCH_OBJS		= bench.o
PRG_SOURCE		= bench.cc
PRG_OBJS		= $(BENCH_OBJS)

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- ${PRG_SOURCE}
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

bench:	$(BENCH_OBJS)
	$(LINK.cc) -o $@ $(BENCH_OBJS) -lB3System -lpthread
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

-include .dep
