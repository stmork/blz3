# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3) $(LIB_OGL3)
LIBS			= libB3xxx.a
PRGS			= TestIo TestThread Mandel TestData
TESTIO_OBJS		= TestIo.o  TestMem.o  TestFile.o  TestDir.o
TESTTHREAD_OBJS	= TestThread.o
MANDEL_OBJS		= Mandel.o b3Mandel.o
PRG_SOURCE		=\
	TestIo.cc\
	TestMem.cc\
	TestFile.cc\
	TestDir.cc\
	TestThread.cc\
	Mandel.cc\
	b3Mandel.cc\
	TestData.cc

all:	$(PRGS)

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M  $(PRG_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

TestIo:	$(TESTIO_OBJS)
	$(LINK.cc) -o $@ $(TESTIO_OBJS) $(LDLIBS)

TestData:	TestData.o
	$(LINK.cc) -o $@ TestData.o $(LDLIBS)

TestThread:	$(TESTTHREAD_OBJS)
	$(LINK.cc) -o $@ $(TESTTHREAD_OBJS) $(LDLIBS)

Mandel:	$(MANDEL_OBJS)
	$(LINK.cc) -o $@ $(MANDEL_OBJS) $(LDLIBS)

-include .dep
