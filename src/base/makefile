# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3Base.a
PRG_SOURCE		= b3FileList.cc b3Node.cc b3World.cc
PRG_OBJ			= b3FileList.o  b3Node.o  b3World.o

all:	lib

depend:
	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)

libB3Base.a:	$(PRG_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

# DO NOT DELETE


b3FileList..o: /home/blz3/include/blz3/b3Types.h
b3FileList..o: /home/blz3/include/blz3/base/b3FileList.h
b3FileList..o: /home/blz3/include/blz3/base/b3List.h
b3FileList..o: /home/blz3/include_unix/blz3/b3Config.h
b3FileList..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3FileList..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3FileList..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3FileList..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3FileList..o: /usr/include/_G_config.h /usr/include/alloca.h
b3FileList..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3FileList..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3FileList..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3FileList..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3FileList..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3FileList..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3FileList..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3FileList..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3FileList..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3FileList..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3FileList..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3FileList..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3FileList..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3FileList..o: /usr/include/ctype.h /usr/include/dirent.h
b3FileList..o: /usr/include/endian.h /usr/include/fcntl.h
b3FileList..o: /usr/include/features.h /usr/include/gconv.h
b3FileList..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3FileList..o: /usr/include/libio.h /usr/include/linux/limits.h
b3FileList..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3FileList..o: /usr/include/signal.h /usr/include/stdio.h
b3FileList..o: /usr/include/stdlib.h /usr/include/string.h
b3FileList..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3FileList..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3FileList..o: /usr/include/sys/types.h /usr/include/time.h
b3FileList..o: /usr/include/unistd.h /usr/include/wchar.h
b3FileList..o: /usr/include/xlocale.h
b3FileList..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3FileList..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3FileList..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Node..o: /home/blz3/include/blz3/b3Types.h
b3Node..o: /home/blz3/include/blz3/base/b3List.h
b3Node..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Node..o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
b3Node..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Node..o: /usr/include/bits/sigset.h /usr/include/bits/types.h
b3Node..o: /usr/include/endian.h /usr/include/features.h
b3Node..o: /usr/include/gnu/stubs.h /usr/include/sys/cdefs.h
b3Node..o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
b3Node..o: /usr/include/sys/types.h /usr/include/time.h
b3Node..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3World..o: /home/blz3/include/blz3/b3Types.h
b3World..o: /home/blz3/include/blz3/base/b3List.h
b3World..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3World..o: /home/blz3/include/blz3/base/b3Spline.h
b3World..o: /home/blz3/include/blz3/base/b3World.h
b3World..o: /home/blz3/include_unix/blz3/b3Config.h
b3World..o: /home/blz3/include_unix/blz3/system/b3File.h
b3World..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3World..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3World..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3World..o: /usr/include/_G_config.h /usr/include/alloca.h
b3World..o: /usr/include/bits/endian.h /usr/include/bits/fcntl.h
b3World..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3World..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3World..o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
b3World..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3World..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3World..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3World..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3World..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3World..o: /usr/include/bits/wchar.h /usr/include/ctype.h
b3World..o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
b3World..o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3World..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3World..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3World..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3World..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3World..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3World..o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
b3World..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3World..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3World..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
