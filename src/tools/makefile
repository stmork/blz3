# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CXX			= g++
LDLIBS			= -lB3System -lpthread
PRGS			= RemDepend RemCR RestoreExt CleanRes
REMDEPEND_OBJS		= RemDepend.o
REMCR_OBJS		= RemCR.o
RESTOREEXT_OBJS		= RestoreExt.o
CLEANRES_OBJS		= CleanRes.o
PRG_SOURCE		= RemDepend.cc RemCR.cc RestoreExt.cc CleanRes.cc
PRG_OBJS		= $(REMDEPEND_OBJS) $(REMCR_OBJS) $(CLEANRES_OBJS)
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

RestoreExt:	$(RESTOREEXT_OBJS)
	$(LINK.cc) -o $@ $(RESTOREEXT_OBJS) $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

CleanRes:	$(CLEANRES_OBJS)
	$(LINK.cc) -o $@ $(CLEANRES_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

-include .dep
