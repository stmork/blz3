# Beispiel Makefile

# Optionen

CPPFLAGS        = $(STDINC)

DOCDIR		= $(BLZ3_DOC)
MANDIR		= $(BLZ3_MAN)
LIBS		= libB3jpeg.o

LIB_SRCS	= jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
	jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c \
	jcphuff.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c \
	jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
	jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
	jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
 	jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
	jquant2.c jutils.c jmemmgr.c jmemnobs.c

LIB_OBJS	= jcapimin.o jcapistd.o jctrans.o jcparam.o jdatadst.o jcinit.o \
	jcmaster.o jcmarker.o jcmainct.o jcprepct.o jccoefct.o jccolor.o \
	jcsample.o jchuff.o jcphuff.o jcdctmgr.o jfdctfst.o jfdctflt.o \
	jfdctint.o jdapimin.o jdapistd.o jdtrans.o jdatasrc.o jdmaster.o \
 	jdinput.o jdmarker.o jdhuff.o jdphuff.o jdmainct.o jdcoefct.o \
	jdpostct.o jddctmgr.o jidctfst.o jidctflt.o jidctint.o jidctred.o \
	jdsample.o jdcolor.o jquant1.o jquant2.o jdmerge.o\
	jcomapi.o jutils.o jerror.o jmemmgr.o jmemnobs.o

# Ziele

all:	$(LIBS)

depend:	$(LIB_SRCS)
#	makedepend -- $(CPPFLAGS) -- $(LIB_SRCS)
	gcc -M  $(LIB_SRCS) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.a *.bak *~ core libB*.a $(PRGS)

libB3jpeg.o:	$(LIB_OBJS)
	rm -f core
	ld -r -o $@ $^

-include .dep
