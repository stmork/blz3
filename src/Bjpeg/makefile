# Beispiel Makefile

# Optionen

CPPFLAGS        = $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS          = $(OFLAGS)

DOCDIR		= $(BLZ3_DOC)
MANDIR		= $(BLZ3_MAN)
LIBS		= libB3jpeg.a

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
	makedepend -- $(CPPFLAGS) -- $(LIB_SRCS)

clean:
	rm -f *.o *.bak *~ core libB*.a $(PRGS)

re:
	$(AR) $(ARXFLAGS) $(BLZ3_LIB)/libB3tiff.a

lib:	$(LIBS)

libB3jpeg.a:	$(LIB_OBJS)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

# DO NOT DELETE


jcapimin.o: /usr/include/_G_config.h /usr/include/alloca.h
jcapimin.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcapimin.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcapimin.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcapimin.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcapimin.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcapimin.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcapimin.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcapimin.o: /usr/include/stdlib.h /usr/include/string.h
jcapimin.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcapimin.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcapimin.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcapimin.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcapimin.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcapimin.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcapistd.o: /usr/include/_G_config.h /usr/include/alloca.h
jcapistd.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcapistd.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcapistd.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcapistd.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcapistd.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcapistd.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcapistd.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcapistd.o: /usr/include/stdlib.h /usr/include/string.h
jcapistd.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcapistd.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcapistd.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcapistd.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcapistd.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcapistd.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jccoefct.o: /usr/include/_G_config.h /usr/include/alloca.h
jccoefct.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jccoefct.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jccoefct.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jccoefct.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jccoefct.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jccoefct.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jccoefct.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jccoefct.o: /usr/include/stdlib.h /usr/include/string.h
jccoefct.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jccoefct.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jccoefct.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jccoefct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jccoefct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jccoefct.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jccolor.o: /usr/include/_G_config.h /usr/include/alloca.h
jccolor.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jccolor.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jccolor.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jccolor.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jccolor.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jccolor.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jccolor.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jccolor.o: /usr/include/stdlib.h /usr/include/string.h
jccolor.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jccolor.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jccolor.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jccolor.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jccolor.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jccolor.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcdctmgr.o: /usr/include/_G_config.h /usr/include/alloca.h
jcdctmgr.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcdctmgr.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcdctmgr.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcdctmgr.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcdctmgr.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcdctmgr.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcdctmgr.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcdctmgr.o: /usr/include/stdlib.h /usr/include/string.h
jcdctmgr.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcdctmgr.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcdctmgr.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcdctmgr.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcdctmgr.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcdctmgr.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jchuff.o: /usr/include/_G_config.h /usr/include/alloca.h
jchuff.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jchuff.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jchuff.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jchuff.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jchuff.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jchuff.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jchuff.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jchuff.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
jchuff.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
jchuff.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
jchuff.o: /usr/include/xlocale.h
jchuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jchuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jchuff.h
jchuff.o: jconfig.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcinit.o: /usr/include/_G_config.h /usr/include/alloca.h
jcinit.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcinit.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcinit.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcinit.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcinit.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcinit.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcinit.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcinit.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
jcinit.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
jcinit.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
jcinit.o: /usr/include/xlocale.h
jcinit.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcinit.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcinit.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcmainct.o: /usr/include/_G_config.h /usr/include/alloca.h
jcmainct.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcmainct.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcmainct.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcmainct.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcmainct.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcmainct.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcmainct.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcmainct.o: /usr/include/stdlib.h /usr/include/string.h
jcmainct.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcmainct.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcmainct.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcmainct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcmainct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcmainct.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcmarker.o: /usr/include/_G_config.h /usr/include/alloca.h
jcmarker.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcmarker.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcmarker.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcmarker.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcmarker.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcmarker.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcmarker.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcmarker.o: /usr/include/stdlib.h /usr/include/string.h
jcmarker.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcmarker.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcmarker.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcmarker.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcmarker.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcmarker.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcmaster.o: /usr/include/_G_config.h /usr/include/alloca.h
jcmaster.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcmaster.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcmaster.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcmaster.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcmaster.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcmaster.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcmaster.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcmaster.o: /usr/include/stdlib.h /usr/include/string.h
jcmaster.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcmaster.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcmaster.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcmaster.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcmaster.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcmaster.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcomapi.o: /usr/include/_G_config.h /usr/include/alloca.h
jcomapi.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcomapi.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcomapi.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcomapi.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcomapi.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcomapi.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcomapi.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcomapi.o: /usr/include/stdlib.h /usr/include/string.h
jcomapi.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcomapi.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcomapi.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcomapi.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcomapi.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcomapi.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcparam.o: /usr/include/_G_config.h /usr/include/alloca.h
jcparam.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcparam.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcparam.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcparam.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcparam.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcparam.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcparam.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcparam.o: /usr/include/stdlib.h /usr/include/string.h
jcparam.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcparam.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcparam.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcparam.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcparam.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcparam.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcphuff.o: /usr/include/_G_config.h /usr/include/alloca.h
jcphuff.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcphuff.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcphuff.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcphuff.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcphuff.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcphuff.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcphuff.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcphuff.o: /usr/include/stdlib.h /usr/include/string.h
jcphuff.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcphuff.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcphuff.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcphuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcphuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jchuff.h
jcphuff.o: jconfig.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcprepct.o: /usr/include/_G_config.h /usr/include/alloca.h
jcprepct.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcprepct.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcprepct.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcprepct.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcprepct.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcprepct.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcprepct.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcprepct.o: /usr/include/stdlib.h /usr/include/string.h
jcprepct.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcprepct.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcprepct.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcprepct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcprepct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcprepct.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jcsample.o: /usr/include/_G_config.h /usr/include/alloca.h
jcsample.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jcsample.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jcsample.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jcsample.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jcsample.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jcsample.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jcsample.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jcsample.o: /usr/include/stdlib.h /usr/include/string.h
jcsample.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jcsample.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jcsample.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jcsample.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jcsample.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jcsample.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jctrans.o: /usr/include/_G_config.h /usr/include/alloca.h
jctrans.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jctrans.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jctrans.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jctrans.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jctrans.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jctrans.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jctrans.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jctrans.o: /usr/include/stdlib.h /usr/include/string.h
jctrans.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jctrans.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jctrans.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jctrans.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jctrans.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jctrans.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdapimin.o: /usr/include/_G_config.h /usr/include/alloca.h
jdapimin.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdapimin.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdapimin.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdapimin.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdapimin.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdapimin.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdapimin.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdapimin.o: /usr/include/stdlib.h /usr/include/string.h
jdapimin.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdapimin.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdapimin.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdapimin.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdapimin.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdapimin.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdapistd.o: /usr/include/_G_config.h /usr/include/alloca.h
jdapistd.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdapistd.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdapistd.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdapistd.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdapistd.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdapistd.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdapistd.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdapistd.o: /usr/include/stdlib.h /usr/include/string.h
jdapistd.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdapistd.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdapistd.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdapistd.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdapistd.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdapistd.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdatadst.o: /usr/include/_G_config.h /usr/include/alloca.h
jdatadst.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdatadst.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdatadst.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdatadst.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdatadst.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdatadst.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdatadst.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdatadst.o: /usr/include/stdlib.h /usr/include/string.h
jdatadst.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdatadst.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdatadst.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdatadst.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdatadst.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdatadst.o: jerror.h jinclude.h jmorecfg.h jpeglib.h
jdatasrc.o: /usr/include/_G_config.h /usr/include/alloca.h
jdatasrc.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdatasrc.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdatasrc.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdatasrc.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdatasrc.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdatasrc.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdatasrc.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdatasrc.o: /usr/include/stdlib.h /usr/include/string.h
jdatasrc.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdatasrc.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdatasrc.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdatasrc.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdatasrc.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdatasrc.o: jerror.h jinclude.h jmorecfg.h jpeglib.h
jdcoefct.o: /usr/include/_G_config.h /usr/include/alloca.h
jdcoefct.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdcoefct.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdcoefct.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdcoefct.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdcoefct.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdcoefct.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdcoefct.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdcoefct.o: /usr/include/stdlib.h /usr/include/string.h
jdcoefct.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdcoefct.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdcoefct.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdcoefct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdcoefct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdcoefct.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdcolor.o: /usr/include/_G_config.h /usr/include/alloca.h
jdcolor.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdcolor.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdcolor.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdcolor.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdcolor.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdcolor.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdcolor.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdcolor.o: /usr/include/stdlib.h /usr/include/string.h
jdcolor.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdcolor.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdcolor.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdcolor.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdcolor.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdcolor.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jddctmgr.o: /usr/include/_G_config.h /usr/include/alloca.h
jddctmgr.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jddctmgr.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jddctmgr.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jddctmgr.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jddctmgr.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jddctmgr.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jddctmgr.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jddctmgr.o: /usr/include/stdlib.h /usr/include/string.h
jddctmgr.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jddctmgr.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jddctmgr.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jddctmgr.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jddctmgr.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jddctmgr.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdhuff.o: /usr/include/_G_config.h /usr/include/alloca.h
jdhuff.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdhuff.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdhuff.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdhuff.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdhuff.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdhuff.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdhuff.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdhuff.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
jdhuff.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
jdhuff.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
jdhuff.o: /usr/include/xlocale.h
jdhuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdhuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdhuff.o: jdhuff.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdinput.o: /usr/include/_G_config.h /usr/include/alloca.h
jdinput.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdinput.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdinput.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdinput.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdinput.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdinput.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdinput.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdinput.o: /usr/include/stdlib.h /usr/include/string.h
jdinput.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdinput.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdinput.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdinput.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdinput.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdinput.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdmainct.o: /usr/include/_G_config.h /usr/include/alloca.h
jdmainct.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdmainct.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdmainct.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdmainct.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdmainct.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdmainct.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdmainct.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdmainct.o: /usr/include/stdlib.h /usr/include/string.h
jdmainct.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdmainct.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdmainct.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdmainct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdmainct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdmainct.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdmarker.o: /usr/include/_G_config.h /usr/include/alloca.h
jdmarker.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdmarker.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdmarker.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdmarker.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdmarker.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdmarker.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdmarker.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdmarker.o: /usr/include/stdlib.h /usr/include/string.h
jdmarker.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdmarker.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdmarker.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdmarker.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdmarker.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdmarker.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdmaster.o: /usr/include/_G_config.h /usr/include/alloca.h
jdmaster.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdmaster.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdmaster.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdmaster.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdmaster.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdmaster.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdmaster.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdmaster.o: /usr/include/stdlib.h /usr/include/string.h
jdmaster.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdmaster.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdmaster.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdmaster.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdmaster.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdmaster.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdmerge.o: /usr/include/_G_config.h /usr/include/alloca.h
jdmerge.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdmerge.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdmerge.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdmerge.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdmerge.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdmerge.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdmerge.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdmerge.o: /usr/include/stdlib.h /usr/include/string.h
jdmerge.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdmerge.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdmerge.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdmerge.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdmerge.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdmerge.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdphuff.o: /usr/include/_G_config.h /usr/include/alloca.h
jdphuff.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdphuff.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdphuff.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdphuff.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdphuff.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdphuff.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdphuff.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdphuff.o: /usr/include/stdlib.h /usr/include/string.h
jdphuff.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdphuff.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdphuff.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdphuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdphuff.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdphuff.o: jdhuff.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdpostct.o: /usr/include/_G_config.h /usr/include/alloca.h
jdpostct.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdpostct.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdpostct.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdpostct.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdpostct.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdpostct.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdpostct.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdpostct.o: /usr/include/stdlib.h /usr/include/string.h
jdpostct.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdpostct.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdpostct.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdpostct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdpostct.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdpostct.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdsample.o: /usr/include/_G_config.h /usr/include/alloca.h
jdsample.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdsample.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdsample.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdsample.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdsample.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdsample.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdsample.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdsample.o: /usr/include/stdlib.h /usr/include/string.h
jdsample.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdsample.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdsample.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdsample.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdsample.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdsample.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jdtrans.o: /usr/include/_G_config.h /usr/include/alloca.h
jdtrans.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jdtrans.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jdtrans.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jdtrans.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jdtrans.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jdtrans.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jdtrans.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jdtrans.o: /usr/include/stdlib.h /usr/include/string.h
jdtrans.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jdtrans.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jdtrans.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jdtrans.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jdtrans.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jdtrans.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jerror.o: /usr/include/_G_config.h /usr/include/alloca.h
jerror.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jerror.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jerror.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jerror.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jerror.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jerror.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jerror.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jerror.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
jerror.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
jerror.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
jerror.o: /usr/include/xlocale.h
jerror.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jerror.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jerror.o: jerror.h jinclude.h jmorecfg.h jpeglib.h jversion.h
jfdctflt.o: /usr/include/_G_config.h /usr/include/alloca.h
jfdctflt.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jfdctflt.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jfdctflt.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jfdctflt.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jfdctflt.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jfdctflt.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jfdctflt.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jfdctflt.o: /usr/include/stdlib.h /usr/include/string.h
jfdctflt.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jfdctflt.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jfdctflt.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jfdctflt.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jfdctflt.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jfdctflt.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jfdctfst.o: /usr/include/_G_config.h /usr/include/alloca.h
jfdctfst.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jfdctfst.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jfdctfst.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jfdctfst.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jfdctfst.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jfdctfst.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jfdctfst.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jfdctfst.o: /usr/include/stdlib.h /usr/include/string.h
jfdctfst.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jfdctfst.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jfdctfst.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jfdctfst.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jfdctfst.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jfdctfst.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jfdctint.o: /usr/include/_G_config.h /usr/include/alloca.h
jfdctint.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jfdctint.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jfdctint.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jfdctint.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jfdctint.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jfdctint.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jfdctint.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jfdctint.o: /usr/include/stdlib.h /usr/include/string.h
jfdctint.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jfdctint.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jfdctint.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jfdctint.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jfdctint.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jfdctint.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jidctflt.o: /usr/include/_G_config.h /usr/include/alloca.h
jidctflt.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jidctflt.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jidctflt.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jidctflt.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jidctflt.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jidctflt.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jidctflt.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jidctflt.o: /usr/include/stdlib.h /usr/include/string.h
jidctflt.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jidctflt.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jidctflt.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jidctflt.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jidctflt.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jidctflt.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jidctfst.o: /usr/include/_G_config.h /usr/include/alloca.h
jidctfst.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jidctfst.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jidctfst.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jidctfst.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jidctfst.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jidctfst.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jidctfst.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jidctfst.o: /usr/include/stdlib.h /usr/include/string.h
jidctfst.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jidctfst.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jidctfst.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jidctfst.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jidctfst.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jidctfst.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jidctint.o: /usr/include/_G_config.h /usr/include/alloca.h
jidctint.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jidctint.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jidctint.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jidctint.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jidctint.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jidctint.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jidctint.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jidctint.o: /usr/include/stdlib.h /usr/include/string.h
jidctint.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jidctint.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jidctint.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jidctint.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jidctint.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jidctint.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jidctred.o: /usr/include/_G_config.h /usr/include/alloca.h
jidctred.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jidctred.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jidctred.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jidctred.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jidctred.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jidctred.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jidctred.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jidctred.o: /usr/include/stdlib.h /usr/include/string.h
jidctred.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jidctred.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jidctred.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jidctred.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jidctred.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jidctred.o: jdct.h jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jmemmgr.o: /usr/include/_G_config.h /usr/include/alloca.h
jmemmgr.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jmemmgr.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jmemmgr.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jmemmgr.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jmemmgr.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jmemmgr.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jmemmgr.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jmemmgr.o: /usr/include/stdlib.h /usr/include/string.h
jmemmgr.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jmemmgr.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jmemmgr.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jmemmgr.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jmemmgr.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jmemmgr.o: jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h jpeglib.h
jmemnobs.o: /usr/include/_G_config.h /usr/include/alloca.h
jmemnobs.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jmemnobs.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jmemnobs.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jmemnobs.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jmemnobs.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jmemnobs.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jmemnobs.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jmemnobs.o: /usr/include/stdlib.h /usr/include/string.h
jmemnobs.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jmemnobs.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jmemnobs.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jmemnobs.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jmemnobs.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jmemnobs.o: jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h jpeglib.h
jquant1.o: /usr/include/_G_config.h /usr/include/alloca.h
jquant1.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jquant1.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jquant1.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jquant1.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jquant1.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jquant1.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jquant1.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jquant1.o: /usr/include/stdlib.h /usr/include/string.h
jquant1.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jquant1.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jquant1.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jquant1.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jquant1.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jquant1.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jquant2.o: /usr/include/_G_config.h /usr/include/alloca.h
jquant2.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jquant2.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jquant2.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jquant2.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jquant2.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jquant2.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jquant2.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jquant2.o: /usr/include/stdlib.h /usr/include/string.h
jquant2.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
jquant2.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
jquant2.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
jquant2.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jquant2.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jquant2.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
jutils.o: /usr/include/_G_config.h /usr/include/alloca.h
jutils.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
jutils.o: /usr/include/bits/sched.h /usr/include/bits/select.h
jutils.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
jutils.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
jutils.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
jutils.o: /usr/include/endian.h /usr/include/features.h /usr/include/gconv.h
jutils.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
jutils.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
jutils.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
jutils.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
jutils.o: /usr/include/xlocale.h
jutils.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
jutils.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h jconfig.h
jutils.o: jerror.h jinclude.h jmorecfg.h jpegint.h jpeglib.h
