# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
PRGS			= RemDepend RemCR RestoreExt CleanRes testimg scale calibrate
REMDEPEND_OBJS		= RemDepend.o
REMCR_OBJS		= RemCR.o
RESTOREEXT_OBJS		= RestoreExt.o
CLEANRES_OBJS		= CleanRes.o
SCALE_OBJS		= scale.o
CALIBRATE_OBJS		= calibrate.o
TESTIMG_OBJS		= testimg.o

PRG_SOURCE		= RemDepend.cc RemCR.cc RestoreExt.cc CleanRes.cc scale.cc calibrate.cc testimg.cc
PRG_OBJS		= $(REMDEPEND_OBJS) $(REMCR_OBJS) $(CLEANRES_OBJS) $(RESTOREEXT_OBJS) $(SCALE_OBJS) $(CALIBRATE_OBJS) $(TESTIMG_OBJS)
#PRG_SOURCE		= ${PRG_OBJS:.o=.cc}

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- ${PRG_SOURCE}
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

RemDepend:	$(REMDEPEND_OBJS)
	$(LINK.cc) -o $@ $(REMDEPEND_OBJS) -lB3System -lpthread
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

RemCR:	$(REMCR_OBJS)
	$(LINK.cc) -o $@ $(REMCR_OBJS) -lB3System -lpthread
# $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

RestoreExt:	$(RESTOREEXT_OBJS)
	$(LINK.cc) -o $@ $(RESTOREEXT_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

CleanRes:	$(CLEANRES_OBJS)
	$(LINK.cc) -o $@ $(CLEANRES_OBJS) -lB3System -lpthread
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

scale:	$(SCALE_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

testimg:	$(TESTIMG_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

calibrate:	$(CALIBRATE_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

-include .dep
