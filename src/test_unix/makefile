# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= -g
CXXFLAGS		= -g
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S) -lpthread
LIBS			= libB3xxx.a
PRGS			= TestIo TestThread Mandel
TESTIO_OBJS		= TestIo.o  TestMem.o  TestFile.o  TestDir.o
TESTTHREAD_OBJS	= TestThread.o
MANDEL_OBJS		= Mandel.o
PRG_SOURCE		= TestIo.cc TestMem.cc TestFile.cc TestDir.cc TestThread.cc Mandel.cc
PRG_OBJS		= $(TESTIO_OBJS) $(TESTTHREAD_OBJS) $(MANDEL_OBJS)

all:	$(PRGS)

depend:
	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

TestIo:	$(TESTIO_OBJS)
	$(LINK.cc) -o $@ $(TESTIO_OBJS) $(LDLIBS)

TestThread:	$(TESTTHREAD_OBJS)
	$(LINK.cc) -o $@ $(TESTTHREAD_OBJS) $(LDLIBS)

Mandel:	$(MANDEL_OBJS)
	$(LINK.cc) -o $@ $(MANDEL_OBJS) $(LDLIBS)

# DO NOT DELETE


Mandel..o: /home/blz3/include/blz3/b3Types.h
Mandel..o: /home/blz3/include/blz3/base/b3List.h
Mandel..o: /home/blz3/include_unix/blz3/system/b3Display.h
Mandel..o: /home/blz3/include_unix/blz3/system/b3Log.h
Mandel..o: /home/blz3/include_unix/blz3/system/b3Mem.h
Mandel..o: /home/blz3/include_unix/blz3/system/b3Thread.h
Mandel..o: /usr/X11R6/include/X11/Composite.h
Mandel..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
Mandel..o: /usr/X11R6/include/X11/ICE/ICE.h
Mandel..o: /usr/X11R6/include/X11/ICE/ICElib.h
Mandel..o: /usr/X11R6/include/X11/Intrinsic.h /usr/X11R6/include/X11/Object.h
Mandel..o: /usr/X11R6/include/X11/RectObj.h /usr/X11R6/include/X11/SM/SM.h
Mandel..o: /usr/X11R6/include/X11/SM/SMlib.h /usr/X11R6/include/X11/Shell.h
Mandel..o: /usr/X11R6/include/X11/X.h /usr/X11R6/include/X11/Xarch.h
Mandel..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
Mandel..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
Mandel..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
Mandel..o: /usr/X11R6/include/X11/keysym.h /usr/X11R6/include/X11/keysymdef.h
Mandel..o: /usr/include/_G_config.h /usr/include/alloca.h
Mandel..o: /usr/include/bits/confname.h /usr/include/bits/endian.h
Mandel..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
Mandel..o: /usr/include/bits/initspin.h /usr/include/bits/posix_opt.h
Mandel..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
Mandel..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
Mandel..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
Mandel..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
Mandel..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
Mandel..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
Mandel..o: /usr/include/bits/wordsize.h /usr/include/endian.h
Mandel..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
Mandel..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
Mandel..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
Mandel..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
Mandel..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
Mandel..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
Mandel..o: /usr/include/sys/time.h /usr/include/sys/types.h
Mandel..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
Mandel..o: /usr/include/xlocale.h
Mandel..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
Mandel..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
TestDir..o: /home/blz3/include/blz3/b3Types.h
TestDir..o: /home/blz3/include_unix/blz3/system/b3Dir.h
TestDir..o: /home/blz3/include_unix/blz3/system/b3Log.h
TestDir..o: /home/blz3/include_unix/blz3/system/b3Mem.h
TestDir..o: /home/blz3/include_unix/blz3/system/b3Thread.h
TestDir..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
TestDir..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
TestDir..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
TestDir..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
TestDir..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
TestDir..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
TestDir..o: /usr/include/bits/sigthread.h /usr/include/bits/time.h
TestDir..o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TestDir..o: /usr/include/dirent.h /usr/include/endian.h
TestDir..o: /usr/include/features.h /usr/include/getopt.h
TestDir..o: /usr/include/gnu/stubs.h /usr/include/linux/limits.h
TestDir..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
TestDir..o: /usr/include/string.h /usr/include/sys/cdefs.h
TestDir..o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
TestDir..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
TestDir..o: /usr/include/xlocale.h
TestDir..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
TestFile..o: /home/blz3/include/blz3/b3Types.h
TestFile..o: /home/blz3/include_unix/blz3/b3Config.h
TestFile..o: /home/blz3/include_unix/blz3/system/b3Dir.h
TestFile..o: /home/blz3/include_unix/blz3/system/b3File.h
TestFile..o: /home/blz3/include_unix/blz3/system/b3Log.h
TestFile..o: /home/blz3/include_unix/blz3/system/b3Mem.h
TestFile..o: /home/blz3/include_unix/blz3/system/b3Thread.h
TestFile..o: /usr/include/_G_config.h /usr/include/alloca.h
TestFile..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
TestFile..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
TestFile..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
TestFile..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
TestFile..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
TestFile..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
TestFile..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
TestFile..o: /usr/include/bits/sched.h /usr/include/bits/select.h
TestFile..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
TestFile..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
TestFile..o: /usr/include/bits/time.h /usr/include/bits/types.h
TestFile..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
TestFile..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
TestFile..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
TestFile..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
TestFile..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
TestFile..o: /usr/include/libio.h /usr/include/linux/limits.h
TestFile..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
TestFile..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
TestFile..o: /usr/include/string.h /usr/include/sys/cdefs.h
TestFile..o: /usr/include/sys/select.h /usr/include/sys/stat.h
TestFile..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
TestFile..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
TestFile..o: /usr/include/xlocale.h
TestFile..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
TestFile..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
TestFile..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
TestIo..o: /home/blz3/include/blz3/b3Types.h
TestIo..o: /home/blz3/include/blz3/base/b3List.h
TestIo..o: /home/blz3/include/blz3/base/b3Nurbs.h
TestIo..o: /home/blz3/include/blz3/base/b3Spline.h
TestIo..o: /home/blz3/include/blz3/base/b3World.h
TestIo..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
TestIo..o: /home/blz3/include_unix/blz3/b3Config.h
TestIo..o: /home/blz3/include_unix/blz3/system/b3Date.h
TestIo..o: /home/blz3/include_unix/blz3/system/b3Dir.h
TestIo..o: /home/blz3/include_unix/blz3/system/b3File.h
TestIo..o: /home/blz3/include_unix/blz3/system/b3Log.h
TestIo..o: /home/blz3/include_unix/blz3/system/b3Mem.h
TestIo..o: /home/blz3/include_unix/blz3/system/b3Thread.h
TestIo..o: /usr/include/_G_config.h /usr/include/alloca.h
TestIo..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
TestIo..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
TestIo..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
TestIo..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
TestIo..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
TestIo..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
TestIo..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
TestIo..o: /usr/include/bits/sched.h /usr/include/bits/select.h
TestIo..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
TestIo..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
TestIo..o: /usr/include/bits/time.h /usr/include/bits/types.h
TestIo..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
TestIo..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
TestIo..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
TestIo..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
TestIo..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
TestIo..o: /usr/include/linux/limits.h /usr/include/math.h
TestIo..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
TestIo..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
TestIo..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
TestIo..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
TestIo..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
TestIo..o: /usr/include/wchar.h /usr/include/xlocale.h
TestIo..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
TestIo..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
TestIo..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
TestMem..o: /home/blz3/include/blz3/b3Types.h
TestMem..o: /home/blz3/include_unix/blz3/system/b3Log.h
TestMem..o: /home/blz3/include_unix/blz3/system/b3Mem.h
TestMem..o: /home/blz3/include_unix/blz3/system/b3Thread.h
TestMem..o: /usr/include/bits/endian.h /usr/include/bits/initspin.h
TestMem..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
TestMem..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
TestMem..o: /usr/include/bits/sigthread.h /usr/include/bits/time.h
TestMem..o: /usr/include/bits/types.h /usr/include/endian.h
TestMem..o: /usr/include/features.h /usr/include/gnu/stubs.h
TestMem..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
TestMem..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
TestMem..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
TestMem..o: /usr/include/time.h
TestMem..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
TestThread..o: /home/blz3/include/blz3/b3Types.h
TestThread..o: /home/blz3/include_unix/blz3/system/b3Log.h
TestThread..o: /home/blz3/include_unix/blz3/system/b3Thread.h
TestThread..o: /usr/include/bits/endian.h /usr/include/bits/initspin.h
TestThread..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
TestThread..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
TestThread..o: /usr/include/bits/sigthread.h /usr/include/bits/time.h
TestThread..o: /usr/include/bits/types.h /usr/include/endian.h
TestThread..o: /usr/include/features.h /usr/include/gnu/stubs.h
TestThread..o: /usr/include/pthread.h /usr/include/sched.h
TestThread..o: /usr/include/signal.h /usr/include/sys/cdefs.h
TestThread..o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
TestThread..o: /usr/include/sys/types.h /usr/include/time.h
TestThread..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
