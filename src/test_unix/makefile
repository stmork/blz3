# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
LIBS			= libB3xxx.a
PRGS			= TestIo TestThread Mandel TestData TestUtil TestFilter TestVector TestMaterial
TESTDATA_OBJS		= TestData.o
TESTMATERIAL_OBJS	= TestMaterial.o
TESTFILTER_OBJS		= TestFilter.o
TESTIO_OBJS		= TestIo.o  TestMem.o  TestFile.o  TestDir.o
TESTTHREAD_OBJS		= TestThread.o
TESTUTIL_OBJS		= TestUtil.o
TESTVECTOR_OBJS		= TestVector.o
MANDEL_OBJS		= Mandel.o b3Mandel.o
PRG_SOURCE		=\
	TestIo.cc\
	TestMem.cc\
	TestFile.cc\
	TestFilter.cc\
	TestDir.cc\
	TestThread.cc\
	Mandel.cc\
	b3Mandel.cc\
	TestData.cc\
	TestUtil.cc\
	TestVector.cc\
	TestMaterial.cc

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M  $(PRG_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

TestIo:	$(TESTIO_OBJS) $(BLZ3_LIB)/libB3System.a
	$(LINK.cc) -o $@ $(TESTIO_OBJS) $(LDLIBS)

TestData:	$(TESTDATA_OBJS) $(BLZ3_LIB)/libB3System.a $(BLZ3_LIB)/libB3Base.a
	$(LINK.cc) -o $@ $(TESTDATA_OBJS) $(LDLIBS)

TestFilter:	$(TESTFILTER_OBJS) $(BLZ3_LIB)/libB3Base.a
	$(LINK.cc) -o $@ $(TESTFILTER_OBJS) $(LDLIBS)

TestUtil:	$(TESTUTIL_OBJS) $(BLZ3_LIB)/libB3Base.a
	$(LINK.cc) -o $@ $(TESTUTIL_OBJS) $(LDLIBS)

TestVector:	$(TESTVECTOR_OBJS) $(BLZ3_LIB)/libB3Base.a
	$(LINK.cc) -o $@ $(TESTVECTOR_OBJS) $(LDLIBS)

TestThread:	$(TESTTHREAD_OBJS) $(BLZ3_LIB)/libB3System.a
	$(LINK.cc) -o $@ $(TESTTHREAD_OBJS) $(LDLIBS)

TestMaterial:	$(TESTMATERIAL_OBJS)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

Mandel:	$(MANDEL_OBJS)
	$(LINK.cc) -o $@ $(MANDEL_OBJS) $(LDLIBS)

-include .dep
