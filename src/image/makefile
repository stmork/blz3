# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= $(OFLAGS) 
CXXFLAGS		= $(OFLAGS)
LDLIBS			= $(LIB_B3) $(LIB_X) $(LIB_S)
PRGS			= 
LIBS			= libB3Image.a
LIB_SOURCE		=\
	b3Tx.cc\
	b3TxBlit.cc\
	b3TxDeskew.cc\
	b3TxHist.cc\
	b3TxImage.cc\
	b3TxLoadTIFF.cc\
	b3TxSaveTIFF.cc\
	b3TxScale.cc\
	b3TxTIFF.cc\
	b3TxTool.cc

LIB_OBJ			=\
	b3Tx.o\
	b3TxBlit.o\
	b3TxDeskew.o\
	b3TxHist.o\
	b3TxImage.o\
	b3TxLoadTIFF.o\
	b3TxSaveTIFF.o\
	b3TxScale.o\
	b3TxTIFF.o\
	b3TxTool.o

all:	lib

depend:
	makedepend -- $(CPPFLAGS) -- $(LIB_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)
 
libB3Image.a:	$(LIB_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

# DO NOT DELETE


b3Tx..o: /home/blz3/include/blz3/b3Types.h
b3Tx..o: /home/blz3/include/blz3/base/b3List.h
b3Tx..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3Tx..o: /home/blz3/include/tiffio.h /home/blz3/include_unix/blz3/b3Config.h
b3Tx..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3Tx..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3Tx..o: /home/blz3/include_unix/blz3/system/b3File.h
b3Tx..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3Tx..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3Tx..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3Tx..o: /usr/X11R6/include/X11/Composite.h
b3Tx..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
b3Tx..o: /usr/X11R6/include/X11/ICE/ICE.h /usr/X11R6/include/X11/ICE/ICElib.h
b3Tx..o: /usr/X11R6/include/X11/Intrinsic.h /usr/X11R6/include/X11/Object.h
b3Tx..o: /usr/X11R6/include/X11/RectObj.h /usr/X11R6/include/X11/SM/SM.h
b3Tx..o: /usr/X11R6/include/X11/SM/SMlib.h /usr/X11R6/include/X11/Shell.h
b3Tx..o: /usr/X11R6/include/X11/X.h /usr/X11R6/include/X11/Xarch.h
b3Tx..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
b3Tx..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
b3Tx..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
b3Tx..o: /usr/X11R6/include/X11/keysym.h /usr/X11R6/include/X11/keysymdef.h
b3Tx..o: /usr/include/_G_config.h /usr/include/alloca.h
b3Tx..o: /usr/include/bits/confname.h /usr/include/bits/dirent.h
b3Tx..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
b3Tx..o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
b3Tx..o: /usr/include/bits/initspin.h /usr/include/bits/local_lim.h
b3Tx..o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
b3Tx..o: /usr/include/bits/nan.h /usr/include/bits/posix1_lim.h
b3Tx..o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
b3Tx..o: /usr/include/bits/sched.h /usr/include/bits/select.h
b3Tx..o: /usr/include/bits/sigset.h /usr/include/bits/sigthread.h
b3Tx..o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
b3Tx..o: /usr/include/bits/time.h /usr/include/bits/types.h
b3Tx..o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
b3Tx..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
b3Tx..o: /usr/include/ctype.h /usr/include/dirent.h /usr/include/endian.h
b3Tx..o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
b3Tx..o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
b3Tx..o: /usr/include/linux/limits.h /usr/include/math.h
b3Tx..o: /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h
b3Tx..o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
b3Tx..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3Tx..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3Tx..o: /usr/include/sys/time.h /usr/include/sys/types.h /usr/include/time.h
b3Tx..o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
b3Tx..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3Tx..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3Tx..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxBlit..o: /home/blz3/include/blz3/b3Types.h
b3TxBlit..o: /home/blz3/include/blz3/base/b3List.h
b3TxBlit..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxBlit..o: /home/blz3/include/tiffio.h
b3TxBlit..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxBlit..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxBlit..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxBlit..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxBlit..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxBlit..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxBlit..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxBlit..o: /usr/X11R6/include/X11/Composite.h
b3TxBlit..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
b3TxBlit..o: /usr/X11R6/include/X11/ICE/ICE.h
b3TxBlit..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxBlit..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxBlit..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3TxBlit..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3TxBlit..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxBlit..o: /usr/X11R6/include/X11/Xarch.h
b3TxBlit..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
b3TxBlit..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
b3TxBlit..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
b3TxBlit..o: /usr/X11R6/include/X11/keysym.h
b3TxBlit..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxBlit..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxBlit..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxBlit..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxBlit..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxBlit..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxBlit..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxBlit..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxBlit..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxBlit..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxBlit..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxBlit..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxBlit..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxBlit..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxBlit..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxBlit..o: /usr/include/dirent.h /usr/include/endian.h /usr/include/fcntl.h
b3TxBlit..o: /usr/include/features.h /usr/include/gconv.h
b3TxBlit..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3TxBlit..o: /usr/include/libio.h /usr/include/linux/limits.h
b3TxBlit..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3TxBlit..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3TxBlit..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3TxBlit..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3TxBlit..o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
b3TxBlit..o: /usr/include/sys/types.h /usr/include/time.h
b3TxBlit..o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
b3TxBlit..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxBlit..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxBlit..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxDeskew..o: /home/blz3/include/blz3/b3Types.h
b3TxDeskew..o: /home/blz3/include/blz3/base/b3List.h
b3TxDeskew..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxDeskew..o: /home/blz3/include/tiffio.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxDeskew..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxDeskew..o: /usr/X11R6/include/X11/Composite.h
b3TxDeskew..o: /usr/X11R6/include/X11/Constraint.h
b3TxDeskew..o: /usr/X11R6/include/X11/Core.h /usr/X11R6/include/X11/ICE/ICE.h
b3TxDeskew..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxDeskew..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxDeskew..o: /usr/X11R6/include/X11/Object.h
b3TxDeskew..o: /usr/X11R6/include/X11/RectObj.h /usr/X11R6/include/X11/SM/SM.h
b3TxDeskew..o: /usr/X11R6/include/X11/SM/SMlib.h
b3TxDeskew..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxDeskew..o: /usr/X11R6/include/X11/Xarch.h
b3TxDeskew..o: /usr/X11R6/include/X11/Xfuncproto.h
b3TxDeskew..o: /usr/X11R6/include/X11/Xlib.h /usr/X11R6/include/X11/Xos.h
b3TxDeskew..o: /usr/X11R6/include/X11/Xosdefs.h
b3TxDeskew..o: /usr/X11R6/include/X11/Xresource.h
b3TxDeskew..o: /usr/X11R6/include/X11/Xutil.h /usr/X11R6/include/X11/keysym.h
b3TxDeskew..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxDeskew..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxDeskew..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxDeskew..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxDeskew..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxDeskew..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxDeskew..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxDeskew..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxDeskew..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxDeskew..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxDeskew..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxDeskew..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxDeskew..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxDeskew..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxDeskew..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxDeskew..o: /usr/include/dirent.h /usr/include/endian.h
b3TxDeskew..o: /usr/include/fcntl.h /usr/include/features.h
b3TxDeskew..o: /usr/include/gconv.h /usr/include/getopt.h
b3TxDeskew..o: /usr/include/gnu/stubs.h /usr/include/libio.h
b3TxDeskew..o: /usr/include/linux/limits.h /usr/include/math.h
b3TxDeskew..o: /usr/include/pthread.h /usr/include/sched.h
b3TxDeskew..o: /usr/include/signal.h /usr/include/stdio.h
b3TxDeskew..o: /usr/include/stdlib.h /usr/include/string.h
b3TxDeskew..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3TxDeskew..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3TxDeskew..o: /usr/include/sys/time.h /usr/include/sys/types.h
b3TxDeskew..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3TxDeskew..o: /usr/include/xlocale.h
b3TxDeskew..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxDeskew..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxDeskew..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxHist..o: /home/blz3/include/blz3/b3Types.h
b3TxHist..o: /home/blz3/include/blz3/base/b3List.h
b3TxHist..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxHist..o: /home/blz3/include/tiffio.h
b3TxHist..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxHist..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxHist..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxHist..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxHist..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxHist..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxHist..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxHist..o: /usr/X11R6/include/X11/Composite.h
b3TxHist..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
b3TxHist..o: /usr/X11R6/include/X11/ICE/ICE.h
b3TxHist..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxHist..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxHist..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3TxHist..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3TxHist..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxHist..o: /usr/X11R6/include/X11/Xarch.h
b3TxHist..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
b3TxHist..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
b3TxHist..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
b3TxHist..o: /usr/X11R6/include/X11/keysym.h
b3TxHist..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxHist..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxHist..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxHist..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxHist..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxHist..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxHist..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxHist..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxHist..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxHist..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxHist..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxHist..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxHist..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxHist..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxHist..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxHist..o: /usr/include/dirent.h /usr/include/endian.h /usr/include/fcntl.h
b3TxHist..o: /usr/include/features.h /usr/include/gconv.h
b3TxHist..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3TxHist..o: /usr/include/libio.h /usr/include/linux/limits.h
b3TxHist..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3TxHist..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3TxHist..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3TxHist..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3TxHist..o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
b3TxHist..o: /usr/include/sys/types.h /usr/include/time.h
b3TxHist..o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
b3TxHist..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxHist..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxHist..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxImage..o: /home/blz3/include/blz3/b3Types.h
b3TxImage..o: /home/blz3/include/blz3/base/b3List.h
b3TxImage..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxImage..o: /home/blz3/include/tiffio.h
b3TxImage..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxImage..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxImage..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxImage..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxImage..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxImage..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxImage..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxImage..o: /usr/X11R6/include/X11/Composite.h
b3TxImage..o: /usr/X11R6/include/X11/Constraint.h
b3TxImage..o: /usr/X11R6/include/X11/Core.h /usr/X11R6/include/X11/ICE/ICE.h
b3TxImage..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxImage..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxImage..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3TxImage..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3TxImage..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxImage..o: /usr/X11R6/include/X11/Xarch.h
b3TxImage..o: /usr/X11R6/include/X11/Xfuncproto.h
b3TxImage..o: /usr/X11R6/include/X11/Xlib.h /usr/X11R6/include/X11/Xos.h
b3TxImage..o: /usr/X11R6/include/X11/Xosdefs.h
b3TxImage..o: /usr/X11R6/include/X11/Xresource.h
b3TxImage..o: /usr/X11R6/include/X11/Xutil.h /usr/X11R6/include/X11/keysym.h
b3TxImage..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxImage..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxImage..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxImage..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxImage..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxImage..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxImage..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxImage..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxImage..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxImage..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxImage..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxImage..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxImage..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxImage..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxImage..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxImage..o: /usr/include/dirent.h /usr/include/endian.h /usr/include/fcntl.h
b3TxImage..o: /usr/include/features.h /usr/include/gconv.h
b3TxImage..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3TxImage..o: /usr/include/libio.h /usr/include/linux/limits.h
b3TxImage..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3TxImage..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3TxImage..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3TxImage..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3TxImage..o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
b3TxImage..o: /usr/include/sys/types.h /usr/include/time.h
b3TxImage..o: /usr/include/unistd.h /usr/include/wchar.h
b3TxImage..o: /usr/include/xlocale.h
b3TxImage..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxImage..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxImage..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxLoadTIFF..o: /home/blz3/include/blz3/b3Types.h
b3TxLoadTIFF..o: /home/blz3/include/blz3/base/b3List.h
b3TxLoadTIFF..o: /home/blz3/include/blz3/image/b3Tx.h
b3TxLoadTIFF..o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
b3TxLoadTIFF..o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
b3TxLoadTIFF..o: /home/blz3/include/tiffiop.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxLoadTIFF..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxLoadTIFF..o: /home/blz3/include_unix/port.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Composite.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Constraint.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Core.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/ICE/ICE.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Object.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/RectObj.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/SM/SM.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/SM/SMlib.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Xarch.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Xfuncproto.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Xlib.h /usr/X11R6/include/X11/Xos.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Xosdefs.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Xresource.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/Xutil.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/keysym.h
b3TxLoadTIFF..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxLoadTIFF..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxLoadTIFF..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxLoadTIFF..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxLoadTIFF..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxLoadTIFF..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxLoadTIFF..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxLoadTIFF..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxLoadTIFF..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxLoadTIFF..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxLoadTIFF..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxLoadTIFF..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxLoadTIFF..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxLoadTIFF..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxLoadTIFF..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxLoadTIFF..o: /usr/include/dirent.h /usr/include/endian.h
b3TxLoadTIFF..o: /usr/include/fcntl.h /usr/include/features.h
b3TxLoadTIFF..o: /usr/include/gconv.h /usr/include/getopt.h
b3TxLoadTIFF..o: /usr/include/gnu/stubs.h /usr/include/libio.h
b3TxLoadTIFF..o: /usr/include/linux/limits.h /usr/include/math.h
b3TxLoadTIFF..o: /usr/include/pthread.h /usr/include/sched.h
b3TxLoadTIFF..o: /usr/include/signal.h /usr/include/stdio.h
b3TxLoadTIFF..o: /usr/include/stdlib.h /usr/include/string.h
b3TxLoadTIFF..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3TxLoadTIFF..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3TxLoadTIFF..o: /usr/include/sys/time.h /usr/include/sys/types.h
b3TxLoadTIFF..o: /usr/include/time.h /usr/include/unistd.h
b3TxLoadTIFF..o: /usr/include/wchar.h /usr/include/xlocale.h
b3TxLoadTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxLoadTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxLoadTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxSaveTIFF..o: /home/blz3/include/blz3/b3Types.h
b3TxSaveTIFF..o: /home/blz3/include/blz3/base/b3List.h
b3TxSaveTIFF..o: /home/blz3/include/blz3/image/b3Tx.h
b3TxSaveTIFF..o: /home/blz3/include/tiff.h /home/blz3/include/tiffio.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxSaveTIFF..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Composite.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Constraint.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Core.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/ICE/ICE.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Object.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/RectObj.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/SM/SM.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/SM/SMlib.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Xarch.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Xfuncproto.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Xlib.h /usr/X11R6/include/X11/Xos.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Xosdefs.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Xresource.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/Xutil.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/keysym.h
b3TxSaveTIFF..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxSaveTIFF..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxSaveTIFF..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxSaveTIFF..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxSaveTIFF..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxSaveTIFF..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxSaveTIFF..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxSaveTIFF..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxSaveTIFF..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxSaveTIFF..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxSaveTIFF..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxSaveTIFF..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxSaveTIFF..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxSaveTIFF..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxSaveTIFF..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxSaveTIFF..o: /usr/include/dirent.h /usr/include/endian.h
b3TxSaveTIFF..o: /usr/include/fcntl.h /usr/include/features.h
b3TxSaveTIFF..o: /usr/include/gconv.h /usr/include/getopt.h
b3TxSaveTIFF..o: /usr/include/gnu/stubs.h /usr/include/libio.h
b3TxSaveTIFF..o: /usr/include/linux/limits.h /usr/include/math.h
b3TxSaveTIFF..o: /usr/include/pthread.h /usr/include/sched.h
b3TxSaveTIFF..o: /usr/include/signal.h /usr/include/stdio.h
b3TxSaveTIFF..o: /usr/include/stdlib.h /usr/include/string.h
b3TxSaveTIFF..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3TxSaveTIFF..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3TxSaveTIFF..o: /usr/include/sys/time.h /usr/include/sys/types.h
b3TxSaveTIFF..o: /usr/include/time.h /usr/include/unistd.h
b3TxSaveTIFF..o: /usr/include/wchar.h /usr/include/xlocale.h
b3TxSaveTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxSaveTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxSaveTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxScale..o: /home/blz3/include/blz3/b3Types.h
b3TxScale..o: /home/blz3/include/blz3/base/b3List.h
b3TxScale..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxScale..o: /home/blz3/include/tiffio.h
b3TxScale..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxScale..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxScale..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxScale..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxScale..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxScale..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxScale..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxScale..o: /usr/X11R6/include/X11/Composite.h
b3TxScale..o: /usr/X11R6/include/X11/Constraint.h
b3TxScale..o: /usr/X11R6/include/X11/Core.h /usr/X11R6/include/X11/ICE/ICE.h
b3TxScale..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxScale..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxScale..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3TxScale..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3TxScale..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxScale..o: /usr/X11R6/include/X11/Xarch.h
b3TxScale..o: /usr/X11R6/include/X11/Xfuncproto.h
b3TxScale..o: /usr/X11R6/include/X11/Xlib.h /usr/X11R6/include/X11/Xos.h
b3TxScale..o: /usr/X11R6/include/X11/Xosdefs.h
b3TxScale..o: /usr/X11R6/include/X11/Xresource.h
b3TxScale..o: /usr/X11R6/include/X11/Xutil.h /usr/X11R6/include/X11/keysym.h
b3TxScale..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxScale..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxScale..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxScale..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxScale..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxScale..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxScale..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxScale..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxScale..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxScale..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxScale..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxScale..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxScale..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxScale..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxScale..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxScale..o: /usr/include/dirent.h /usr/include/endian.h /usr/include/fcntl.h
b3TxScale..o: /usr/include/features.h /usr/include/gconv.h
b3TxScale..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3TxScale..o: /usr/include/libio.h /usr/include/linux/limits.h
b3TxScale..o: /usr/include/malloc.h /usr/include/math.h /usr/include/pthread.h
b3TxScale..o: /usr/include/sched.h /usr/include/signal.h /usr/include/stdio.h
b3TxScale..o: /usr/include/stdlib.h /usr/include/string.h
b3TxScale..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
b3TxScale..o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
b3TxScale..o: /usr/include/sys/time.h /usr/include/sys/types.h
b3TxScale..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
b3TxScale..o: /usr/include/xlocale.h
b3TxScale..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxScale..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxScale..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxTIFF..o: /home/blz3/include/blz3/b3Types.h
b3TxTIFF..o: /home/blz3/include/blz3/base/b3List.h
b3TxTIFF..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxTIFF..o: /home/blz3/include/tiffio.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxTIFF..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxTIFF..o: /usr/X11R6/include/X11/Composite.h
b3TxTIFF..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
b3TxTIFF..o: /usr/X11R6/include/X11/ICE/ICE.h
b3TxTIFF..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxTIFF..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxTIFF..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3TxTIFF..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3TxTIFF..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxTIFF..o: /usr/X11R6/include/X11/Xarch.h
b3TxTIFF..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
b3TxTIFF..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
b3TxTIFF..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
b3TxTIFF..o: /usr/X11R6/include/X11/keysym.h
b3TxTIFF..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxTIFF..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxTIFF..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxTIFF..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxTIFF..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxTIFF..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxTIFF..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxTIFF..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxTIFF..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxTIFF..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxTIFF..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxTIFF..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxTIFF..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxTIFF..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxTIFF..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxTIFF..o: /usr/include/dirent.h /usr/include/endian.h /usr/include/fcntl.h
b3TxTIFF..o: /usr/include/features.h /usr/include/gconv.h
b3TxTIFF..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3TxTIFF..o: /usr/include/libio.h /usr/include/linux/limits.h
b3TxTIFF..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3TxTIFF..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3TxTIFF..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3TxTIFF..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3TxTIFF..o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
b3TxTIFF..o: /usr/include/sys/types.h /usr/include/time.h
b3TxTIFF..o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
b3TxTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxTIFF..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
b3TxTool..o: /home/blz3/include/blz3/b3Types.h
b3TxTool..o: /home/blz3/include/blz3/base/b3List.h
b3TxTool..o: /home/blz3/include/blz3/image/b3Tx.h /home/blz3/include/tiff.h
b3TxTool..o: /home/blz3/include/tiffio.h
b3TxTool..o: /home/blz3/include_unix/blz3/b3Config.h
b3TxTool..o: /home/blz3/include_unix/blz3/system/b3Dir.h
b3TxTool..o: /home/blz3/include_unix/blz3/system/b3Display.h
b3TxTool..o: /home/blz3/include_unix/blz3/system/b3File.h
b3TxTool..o: /home/blz3/include_unix/blz3/system/b3Log.h
b3TxTool..o: /home/blz3/include_unix/blz3/system/b3Mem.h
b3TxTool..o: /home/blz3/include_unix/blz3/system/b3Thread.h
b3TxTool..o: /usr/X11R6/include/X11/Composite.h
b3TxTool..o: /usr/X11R6/include/X11/Constraint.h /usr/X11R6/include/X11/Core.h
b3TxTool..o: /usr/X11R6/include/X11/ICE/ICE.h
b3TxTool..o: /usr/X11R6/include/X11/ICE/ICElib.h
b3TxTool..o: /usr/X11R6/include/X11/Intrinsic.h
b3TxTool..o: /usr/X11R6/include/X11/Object.h /usr/X11R6/include/X11/RectObj.h
b3TxTool..o: /usr/X11R6/include/X11/SM/SM.h /usr/X11R6/include/X11/SM/SMlib.h
b3TxTool..o: /usr/X11R6/include/X11/Shell.h /usr/X11R6/include/X11/X.h
b3TxTool..o: /usr/X11R6/include/X11/Xarch.h
b3TxTool..o: /usr/X11R6/include/X11/Xfuncproto.h /usr/X11R6/include/X11/Xlib.h
b3TxTool..o: /usr/X11R6/include/X11/Xos.h /usr/X11R6/include/X11/Xosdefs.h
b3TxTool..o: /usr/X11R6/include/X11/Xresource.h /usr/X11R6/include/X11/Xutil.h
b3TxTool..o: /usr/X11R6/include/X11/keysym.h
b3TxTool..o: /usr/X11R6/include/X11/keysymdef.h /usr/include/_G_config.h
b3TxTool..o: /usr/include/alloca.h /usr/include/bits/confname.h
b3TxTool..o: /usr/include/bits/dirent.h /usr/include/bits/endian.h
b3TxTool..o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
b3TxTool..o: /usr/include/bits/huge_val.h /usr/include/bits/initspin.h
b3TxTool..o: /usr/include/bits/local_lim.h /usr/include/bits/mathcalls.h
b3TxTool..o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
b3TxTool..o: /usr/include/bits/posix1_lim.h /usr/include/bits/posix_opt.h
b3TxTool..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
b3TxTool..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
b3TxTool..o: /usr/include/bits/sigthread.h /usr/include/bits/stat.h
b3TxTool..o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
b3TxTool..o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
b3TxTool..o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
b3TxTool..o: /usr/include/bits/wordsize.h /usr/include/ctype.h
b3TxTool..o: /usr/include/dirent.h /usr/include/endian.h /usr/include/fcntl.h
b3TxTool..o: /usr/include/features.h /usr/include/gconv.h
b3TxTool..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
b3TxTool..o: /usr/include/libio.h /usr/include/linux/limits.h
b3TxTool..o: /usr/include/math.h /usr/include/pthread.h /usr/include/sched.h
b3TxTool..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/stdlib.h
b3TxTool..o: /usr/include/string.h /usr/include/sys/cdefs.h
b3TxTool..o: /usr/include/sys/select.h /usr/include/sys/stat.h
b3TxTool..o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
b3TxTool..o: /usr/include/sys/types.h /usr/include/time.h
b3TxTool..o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
b3TxTool..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
b3TxTool..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
b3TxTool..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
