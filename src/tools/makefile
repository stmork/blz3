# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
PRGS			= RemDepend RemCR RestoreExt CleanRes testimg scale calibrate half pw reduce

REMDEPEND_OBJS		= RemDepend.o
REMCR_OBJS		= RemCR.o
RESTOREEXT_OBJS		= RestoreExt.o
CLEANRES_OBJS		= CleanRes.o
SCALE_OBJS		= scale.o
HALF_OBJS		= half.o
CALIBRATE_OBJS		= calibrate.o
TESTIMG_OBJS		= testimg.o
PW_OBJS			= pw.o
DIVX_OBJS		= divx.o
REDUCE_OBJS		= reduce.o

PRG_SOURCE		= RemDepend.cc RemCR.cc RestoreExt.cc CleanRes.cc scale.cc calibrate.cc testimg.cc half.cc pw.cc divx.cc reduce.cc
PRG_OBJS		= $(REMDEPEND_OBJS) $(REMCR_OBJS) $(CLEANRES_OBJS) $(RESTOREEXT_OBJS) $(SCALE_OBJS) $(CALIBRATE_OBJS) $(TESTIMG_OBJS) $(HALF_OBJS) $(PW_OBJS) $(DIVX_OBJS) $(REDUCE_OBJS)
#PRG_SOURCE		= ${PRG_OBJS:.o=.cc}

all:	$(PRGS)
	chmod 775 $^
	cp $^ $(BLZ3_BIN)

depend:
	gcc -M $(CPPFLAGS) $(PRG_SOURCE) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) ALL QUIET

RemDepend:	$(REMDEPEND_OBJS)
	$(LINK.cc) -o $@ $^ -lB3System -lpthread

RemCR:	$(REMCR_OBJS)
	$(LINK.cc) -o $@ $^ -lB3System -lpthread # $(LDLIBS)

RestoreExt:	$(RESTOREEXT_OBJS)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

CleanRes:	$(CLEANRES_OBJS)
	$(LINK.cc) -o $@ $^ -lB3System -lpthread

scale:	$(SCALE_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)

half:	$(HALF_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)

pw:	$(PW_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)

divx:	$(DIVX_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS) -ldivxencore -lavi

testimg:	$(TESTIMG_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)

calibrate:	$(CALIBRATE_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)

reduce:	$(REDUCE_OBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include .dep
