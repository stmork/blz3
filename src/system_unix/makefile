# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3System.a
LIB_SOURCE		=\
	b3Config.cc\
	b3Date.cc\
	b3Dir.cc\
	b3Display.cc\
	b3File.cc\
	b3Log.cc\
	b3Mem.cc\
	b3Row.cc\
	b3Thread.cc

LIB_OBJ			=\
	b3Config.o\
	b3Date.o\
	b3Dir.o\
	b3Display.o\
	b3File.o\
	b3Log.o\
	b3Mem.o\
	b3Row.o\
	b3Thread.o

all:	lib

depend:
	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)
 
libB3System.a:	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

# DO NOT DELETE


b3Config..o: /home/blz3/include/blz3/b3Types.h
b3Config..o: /home/blz3/include_unix/blz3/b3Config.h
b3Config..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Config..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Config..o: /usr/include/bits/confname.h /usr/include/bits/endian.h
b3Config..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3Config..o: /usr/include/bits/huge_val.h /usr/include/bits/mathcalls.h
b3Config..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3Config..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Config..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Config..o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
b3Config..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3Config..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3Config..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3Config..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3Config..o: /usr/include/endian.h /usr/include/fcntl.h
b3Config..o: /usr/include/features.h /usr/include/gconv.h
b3Config..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Config..o: /usr/include/libio.h /usr/include/math.h /usr/include/stdio.h
b3Config..o: /usr/include/stdlib.h /usr/include/string.h
b3Config..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Config..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Config..o: /usr/include/sys/types.h /usr/include/time.h
b3Config..o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
b3Config..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Config..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Config..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Date..o: /home/blz3/include/blz3/b3Types.h
b3Date..o: /home/blz3/include_unix/blz3/b3Config.h
b3Date..o: /home/blz3/include_unix/blz3/system/b3Date.h
b3Date..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Date..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Date..o: /usr/include/bits/endian.h /usr/include/bits/fcntl.h
b3Date..o: /usr/include/bits/huge_val.h /usr/include/bits/mathcalls.h
b3Date..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3Date..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3Date..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3Date..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Date..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Date..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Date..o: /usr/include/bits/wchar.h /usr/include/ctype.h
b3Date..o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
b3Date..o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3Date..o: /usr/include/math.h /usr/include/stdio.h /usr/include/stdlib.h
b3Date..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Date..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Date..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Date..o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
b3Date..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Date..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Date..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Dir..o: /home/blz3/include/blz3/b3Types.h
b3Dir..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Dir..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Dir..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Dir..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Dir..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Dir..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Dir..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3Dir..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3Dir..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3Dir..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3Dir..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Dir..o: /usr/include/bits/wordsize.h /usr/include/dirent.h
b3Dir..o: /usr/include/endian.h /usr/include/features.h /usr/include/getopt.h
b3Dir..o: /usr/include/gnu/stubs.h /usr/include/linux/limits.h
b3Dir..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3Dir..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Dir..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Dir..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Dir..o: /usr/include/time.h /usr/include/unistd.h /usr/include/xlocale.h
b3Dir..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Display..o: /home/blz3/include/blz3/b3Types.h
b3Display..o: /home/blz3/include/blz3/base/b3List.h
b3Display..o: /home/blz3/include_unix/blz3/b3Config.h
b3Display..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3Display..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Display..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Display..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Display..o: /usr/X11R6/include/X11/Composite.h
b3Display..o: /usr/X11R6/include/X11/Constraint.h
b3Display..o: /usr/X11R6/include/X11/Core.h /usr/X11R6/include/X11/ICE/ICE.h
b3Display..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3Display..o: /usr/X11R6/include/X11/Intrinsic.h
b3Display..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3Display..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3Display..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3Display..o: /usr/X11R6/include/X11/Xarch.h
b3Display..o: /usr/X11R6/include/X11/Xfuncproto.h
b3Display..o: /usr/X11R6/include/X11/Xlib.h /usr/X11R6/include/X11/Xos.h
b3Display..o: /usr/X11R6/include/X11/Xosdefs.h
b3Display..o: /usr/X11R6/include/X11/Xresource.h
b3Display..o: /usr/X11R6/include/X11/Xutil.h /usr/X11R6/include/X11/keysym.h
b3Display..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3Display..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3Display..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Display..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Display..o: /usr/include/bits/initspin.h /usr/include/bits/mathcalls.h
b3Display..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3Display..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Display..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Display..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Display..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Display..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Display..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Display..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Display..o: /usr/include/ctype.h /usr/include/endian.h /usr/include/fcntl.h
b3Display..o: /usr/include/features.h /usr/include/gconv.h
b3Display..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Display..o: /usr/include/libio.h /usr/include/math.h /usr/include/pthread.h
b3Display..o: /usr/include/sched.h /usr/include/signal.h /usr/include/stdio.h
b3Display..o: /usr/include/stdlib.h /usr/include/string.h
b3Display..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Display..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Display..o: /usr/include/sys/time.h /usr/include/sys/types.h
b3Display..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Display..o: /usr/include/xlocale.h
b3Display..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Display..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Display..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3File..o: /home/blz3/include/blz3/b3Types.h
b3File..o: /home/blz3/include_unix/blz3/system/b3File.h
b3File..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3File..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3File..o: /usr/include/_G_config.h /usr/include/bits/confname.h
b3File..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3File..o: /usr/include/bits/fcntl.h /usr/include/bits/initspin.h
b3File..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3File..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3File..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3File..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3File..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3File..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3File..o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
b3File..o: /usr/include/gconv.h /usr/include/getopt.h /usr/include/gnu/stubs.h
b3File..o: /usr/include/libio.h /usr/include/pthread.h /usr/include/sched.h
b3File..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/sys/cdefs.h
b3File..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3File..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3File..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3File..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3File..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Log..o: /home/blz3/include/blz3/b3Types.h
b3Log..o: /home/blz3/include_unix/blz3/b3Config.h
b3Log..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Log..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Log..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Log..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Log..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Log..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Log..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Log..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Log..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Log..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Log..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Log..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Log..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Log..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Log..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Log..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Log..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Log..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Log..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Log..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Log..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3Log..o: /usr/include/linux/limits.h /usr/include/math.h
b3Log..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3Log..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
b3Log..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Log..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Log..o: /usr/include/sys/timeb.h /usr/include/sys/types.h
b3Log..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Log..o: /usr/include/xlocale.h
b3Log..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Log..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Log..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Mem..o: /home/blz3/include/blz3/b3Types.h
b3Mem..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Mem..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Mem..o: /home/blz3/include_unix/blz3/system/b3Thread.h /usr/include/alloca.h
b3Mem..o: /usr/include/bits/endian.h /usr/include/bits/initspin.h
b3Mem..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3Mem..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3Mem..o: /usr/include/bits/sigthread.h /usr/include/bits/time.h
b3Mem..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3Mem..o: /usr/include/bits/waitstatus.h /usr/include/endian.h
b3Mem..o: /usr/include/features.h /usr/include/gnu/stubs.h
b3Mem..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3Mem..o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
b3Mem..o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
b3Mem..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/xlocale.h
b3Mem..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Row..o: /home/blz3/include/blz3/b3Types.h
b3Row..o: /home/blz3/include/blz3/base/b3List.h
b3Row..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3Row..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Row..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Row..o: /usr/X11R6/include/X11/Composite.h
b3Row..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
b3Row..o: /usr/X11R6/include/X11/ICE/ICE.h /usr/X11R6/include/X11/ICE/ICElib.h
b3Row..o: /usr/X11R6/include/X11/Intrinsic.h /usr/X11R6/include/X11/Object.h
b3Row..o: /usr/X11R6/include/X11/RectObj.h /usr/X11R6/include/X11/SM/SM.h
b3Row..o: /usr/X11R6/include/X11/SM/SMlib.h /usr/X11R6/include/X11/Shell.h
b3Row..o: /usr/X11R6/include/X11/X.h /usr/X11R6/include/X11/Xarch.h
b3Row..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
b3Row..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
b3Row..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
b3Row..o: /usr/X11R6/include/X11/keysym.h /usr/X11R6/include/X11/keysymdef.h
b3Row..o: /usr/include/bits/confname.h /usr/include/bits/endian.h
b3Row..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3Row..o: /usr/include/bits/initspin.h /usr/include/bits/posix_opt.h
b3Row..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3Row..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3Row..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3Row..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Row..o: /usr/include/bits/wordsize.h /usr/include/endian.h
b3Row..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/getopt.h
b3Row..o: /usr/include/gnu/stubs.h /usr/include/pthread.h /usr/include/sched.h
b3Row..o: /usr/include/signal.h /usr/include/string.h /usr/include/sys/cdefs.h
b3Row..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Row..o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
b3Row..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
b3Row..o: /usr/include/xlocale.h
b3Row..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Thread..o: /home/blz3/include/blz3/b3Types.h
b3Thread..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Thread..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Thread..o: /usr/include/asm/errno.h /usr/include/bits/confname.h
b3Thread..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Thread..o: /usr/include/bits/errno.h /usr/include/bits/initspin.h
b3Thread..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Thread..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Thread..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Thread..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Thread..o: /usr/include/bits/wordsize.h /usr/include/endian.h
b3Thread..o: /usr/include/errno.h /usr/include/features.h
b3Thread..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Thread..o: /usr/include/linux/errno.h /usr/include/pthread.h
b3Thread..o: /usr/include/sched.h /usr/include/signal.h
b3Thread..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Thread..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Thread..o: /usr/include/time.h /usr/include/unistd.h
b3Thread..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
