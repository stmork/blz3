# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS                = $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3Raytrace.a
PRG_SOURCE		= b3Init.cc\
	b3Condition.cc b3Bump.cc b3Material.cc\
	b3Shape.cc b3BBox.cc\
	b3Animation.cc b3Light.cc b3Special.cc\
	b3Scene.cc
PRG_OBJ			= b3Init.o\
	b3Condition.o b3Bump.o b3Material.o\
	b3Shape.o b3BBox.o\
	b3Animation.o b3Light.o b3Special.o\
	b3Scene.o

all:	lib

depend:
	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)

libB3Raytrace.a:	$(PRG_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

# DO NOT DELETE


b3Animation..o: /home/blz3/include/blz3/b3Types.h
b3Animation..o: /home/blz3/include/blz3/base/b3List.h
b3Animation..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Animation..o: /home/blz3/include/blz3/base/b3Spline.h
b3Animation..o: /home/blz3/include/blz3/base/b3World.h
b3Animation..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Animation..o: /home/blz3/include_unix/blz3/b3Config.h
b3Animation..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Animation..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Animation..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Animation..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Animation..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Animation..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Animation..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Animation..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Animation..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Animation..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Animation..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Animation..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Animation..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Animation..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Animation..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Animation..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Animation..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Animation..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Animation..o: /usr/include/ctype.h /usr/include/dirent.h
b3Animation..o: /usr/include/endian.h /usr/include/fcntl.h
b3Animation..o: /usr/include/features.h /usr/include/gconv.h
b3Animation..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Animation..o: /usr/include/libio.h /usr/include/linux/limits.h
b3Animation..o: /usr/include/math.h /usr/include/pthread.h
b3Animation..o: /usr/include/sched.h /usr/include/signal.h
b3Animation..o: /usr/include/stdio.h /usr/include/stdlib.h
b3Animation..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Animation..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Animation..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Animation..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Animation..o: /usr/include/xlocale.h
b3Animation..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Animation..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Animation..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3BBox..o: /home/blz3/include/blz3/b3Types.h
b3BBox..o: /home/blz3/include/blz3/base/b3List.h
b3BBox..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3BBox..o: /home/blz3/include/blz3/base/b3Spline.h
b3BBox..o: /home/blz3/include/blz3/base/b3World.h
b3BBox..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3BBox..o: /home/blz3/include_unix/blz3/b3Config.h
b3BBox..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3BBox..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3BBox..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3BBox..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3BBox..o: /usr/include/_G_config.h /usr/include/alloca.h
b3BBox..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3BBox..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3BBox..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3BBox..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3BBox..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3BBox..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3BBox..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3BBox..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3BBox..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3BBox..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3BBox..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3BBox..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3BBox..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3BBox..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3BBox..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3BBox..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3BBox..o: /usr/include/linux/limits.h /usr/include/math.h
b3BBox..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3BBox..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
b3BBox..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3BBox..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3BBox..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
b3BBox..o: /usr/include/wchar.h /usr/include/xlocale.h
b3BBox..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3BBox..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3BBox..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Bump..o: /home/blz3/include/blz3/b3Types.h
b3Bump..o: /home/blz3/include/blz3/base/b3List.h
b3Bump..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Bump..o: /home/blz3/include/blz3/base/b3Spline.h
b3Bump..o: /home/blz3/include/blz3/base/b3World.h
b3Bump..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Bump..o: /home/blz3/include_unix/blz3/b3Config.h
b3Bump..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Bump..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Bump..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Bump..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Bump..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Bump..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Bump..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Bump..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Bump..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Bump..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Bump..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Bump..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Bump..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Bump..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Bump..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Bump..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Bump..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Bump..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Bump..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Bump..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Bump..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3Bump..o: /usr/include/linux/limits.h /usr/include/math.h
b3Bump..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3Bump..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
b3Bump..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Bump..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Bump..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
b3Bump..o: /usr/include/wchar.h /usr/include/xlocale.h
b3Bump..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Bump..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Bump..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Condition..o: /home/blz3/include/blz3/b3Types.h
b3Condition..o: /home/blz3/include/blz3/base/b3List.h
b3Condition..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Condition..o: /home/blz3/include/blz3/base/b3Spline.h
b3Condition..o: /home/blz3/include/blz3/base/b3World.h
b3Condition..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Condition..o: /home/blz3/include_unix/blz3/b3Config.h
b3Condition..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Condition..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Condition..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Condition..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Condition..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Condition..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Condition..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Condition..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Condition..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Condition..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Condition..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Condition..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Condition..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Condition..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Condition..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Condition..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Condition..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Condition..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Condition..o: /usr/include/ctype.h /usr/include/dirent.h
b3Condition..o: /usr/include/endian.h /usr/include/fcntl.h
b3Condition..o: /usr/include/features.h /usr/include/gconv.h
b3Condition..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Condition..o: /usr/include/libio.h /usr/include/linux/limits.h
b3Condition..o: /usr/include/math.h /usr/include/pthread.h
b3Condition..o: /usr/include/sched.h /usr/include/signal.h
b3Condition..o: /usr/include/stdio.h /usr/include/stdlib.h
b3Condition..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Condition..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Condition..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Condition..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Condition..o: /usr/include/xlocale.h
b3Condition..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Condition..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Condition..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Init..o: /home/blz3/include/blz3/b3Types.h
b3Init..o: /home/blz3/include/blz3/base/b3List.h
b3Init..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Init..o: /home/blz3/include/blz3/base/b3Spline.h
b3Init..o: /home/blz3/include/blz3/base/b3World.h
b3Init..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Init..o: /home/blz3/include_unix/blz3/b3Config.h
b3Init..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Init..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Init..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Init..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Init..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Init..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Init..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Init..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Init..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Init..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Init..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Init..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Init..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Init..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Init..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Init..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Init..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Init..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Init..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Init..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Init..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3Init..o: /usr/include/linux/limits.h /usr/include/math.h
b3Init..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3Init..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
b3Init..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Init..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Init..o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
b3Init..o: /usr/include/wchar.h /usr/include/xlocale.h
b3Init..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Init..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Init..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Light..o: /home/blz3/include/blz3/b3Types.h
b3Light..o: /home/blz3/include/blz3/base/b3List.h
b3Light..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Light..o: /home/blz3/include/blz3/base/b3Spline.h
b3Light..o: /home/blz3/include/blz3/base/b3World.h
b3Light..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Light..o: /home/blz3/include_unix/blz3/b3Config.h
b3Light..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Light..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Light..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Light..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Light..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Light..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Light..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Light..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Light..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Light..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Light..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Light..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Light..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Light..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Light..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Light..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Light..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Light..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Light..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Light..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Light..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Light..o: /usr/include/libio.h /usr/include/linux/limits.h
b3Light..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3Light..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3Light..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Light..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Light..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Light..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Light..o: /usr/include/xlocale.h
b3Light..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Light..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Light..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Material..o: /home/blz3/include/blz3/b3Types.h
b3Material..o: /home/blz3/include/blz3/base/b3List.h
b3Material..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Material..o: /home/blz3/include/blz3/base/b3Spline.h
b3Material..o: /home/blz3/include/blz3/base/b3World.h
b3Material..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Material..o: /home/blz3/include_unix/blz3/b3Config.h
b3Material..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Material..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Material..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Material..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Material..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Material..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Material..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Material..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Material..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Material..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Material..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Material..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Material..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Material..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Material..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Material..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Material..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Material..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Material..o: /usr/include/ctype.h /usr/include/dirent.h
b3Material..o: /usr/include/endian.h /usr/include/fcntl.h
b3Material..o: /usr/include/features.h /usr/include/gconv.h
b3Material..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Material..o: /usr/include/libio.h /usr/include/linux/limits.h
b3Material..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3Material..o: /usr/include/signal.h /usr/include/stdio.h
b3Material..o: /usr/include/stdlib.h /usr/include/string.h
b3Material..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Material..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Material..o: /usr/include/sys/types.h /usr/include/time.h
b3Material..o: /usr/include/unistd.h /usr/include/wchar.h
b3Material..o: /usr/include/xlocale.h
b3Material..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Material..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Material..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Scene..o: /home/blz3/include/blz3/b3Types.h
b3Scene..o: /home/blz3/include/blz3/base/b3List.h
b3Scene..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Scene..o: /home/blz3/include/blz3/base/b3Spline.h
b3Scene..o: /home/blz3/include/blz3/base/b3World.h
b3Scene..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Scene..o: /home/blz3/include_unix/blz3/b3Config.h
b3Scene..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Scene..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Scene..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Scene..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Scene..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Scene..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Scene..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Scene..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Scene..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Scene..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Scene..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Scene..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Scene..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Scene..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Scene..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Scene..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Scene..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Scene..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Scene..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Scene..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Scene..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Scene..o: /usr/include/libio.h /usr/include/linux/limits.h
b3Scene..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3Scene..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3Scene..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Scene..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Scene..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Scene..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Scene..o: /usr/include/xlocale.h
b3Scene..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Scene..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Scene..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Shape..o: /home/blz3/include/blz3/b3Types.h
b3Shape..o: /home/blz3/include/blz3/base/b3List.h
b3Shape..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Shape..o: /home/blz3/include/blz3/base/b3Spline.h
b3Shape..o: /home/blz3/include/blz3/base/b3World.h
b3Shape..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Shape..o: /home/blz3/include_unix/blz3/b3Config.h
b3Shape..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Shape..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Shape..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Shape..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Shape..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Shape..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Shape..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Shape..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Shape..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Shape..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Shape..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Shape..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Shape..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Shape..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Shape..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Shape..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Shape..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Shape..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Shape..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Shape..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Shape..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3Shape..o: /usr/include/libio.h /usr/include/linux/limits.h
b3Shape..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3Shape..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3Shape..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Shape..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Shape..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Shape..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Shape..o: /usr/include/xlocale.h
b3Shape..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Shape..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Shape..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3Special..o: /home/blz3/include/blz3/b3Types.h
b3Special..o: /home/blz3/include/blz3/base/b3List.h
b3Special..o: /home/blz3/include/blz3/base/b3Nurbs.h
b3Special..o: /home/blz3/include/blz3/base/b3Spline.h
b3Special..o: /home/blz3/include/blz3/base/b3World.h
b3Special..o: /home/blz3/include/blz3/raytrace/b3Raytrace.h
b3Special..o: /home/blz3/include_unix/blz3/b3Config.h
b3Special..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Special..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Special..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Special..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Special..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Special..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Special..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Special..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Special..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Special..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Special..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Special..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Special..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Special..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Special..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Special..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Special..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Special..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Special..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Special..o: /usr/include/fcntl.h /usr/include/features.h
b3Special..o: /usr/include/gconv.h /usr/include/getopt.h
b3Special..o: /usr/include/gnu/stubs.h /usr/include/libio.h
b3Special..o: /usr/include/linux/limits.h /usr/include/math.h
b3Special..o: /usr/include/pthread.h /usr/include/sched.h
b3Special..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3Special..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3Special..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3Special..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
b3Special..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3Special..o: /usr/include/xlocale.h
b3Special..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Special..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Special..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
