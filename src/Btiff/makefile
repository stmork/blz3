# Beispiel Makefile

# Optionen

CPPFLAGS        = $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS          = $(OFLAGS)

DOCDIR		= $(BLZ3_DOC)
MANDIR		= $(BLZ3_MAN)
LIBS		= libB3tiff.a
PRGS		= mkversion mkg3states
AUTO		= tif_fax3sm.c version.h

ALPHA	= tiff.alpha
VERSION	= VERSION

SRCS	= \
	mkversion.c\
	mkg3states.c\
	tif_aux.c \
	tif_close.c \
	tif_codec.c \
	tif_compress.c \
	tif_dir.c \
	tif_dirinfo.c \
	tif_dirread.c \
	tif_dirwrite.c \
	tif_dumpmode.c \
	tif_error.c \
	tif_fax3.c \
	tif_getimage.c \
	tif_jpeg.c \
	tif_flush.c \
	tif_luv.c \
	tif_lzw.c \
	tif_next.c \
	tif_open.c \
	tif_packbits.c \
	tif_pixarlog.c \
	tif_predict.c \
	tif_print.c \
	tif_read.c \
	tif_swab.c \
	tif_strip.c \
	tif_thunder.c \
	tif_tile.c \
	tif_unix.c \
	tif_version.c \
	tif_warning.c \
	tif_write.c \
	tif_zip.c

OBJS	= \
	tif_aux.o \
	tif_close.o \
	tif_codec.o \
	tif_compress.o \
	tif_dir.o \
	tif_dirinfo.o \
	tif_dirread.o \
	tif_dirwrite.o \
	tif_dumpmode.o \
	tif_error.o \
	tif_fax3.o \
	tif_fax3sm.o \
	tif_getimage.o \
	tif_jpeg.o \
	tif_flush.o \
	tif_luv.o \
	tif_lzw.o \
	tif_next.o \
	tif_open.o \
	tif_packbits.o \
	tif_pixarlog.o \
	tif_predict.o \
	tif_print.o \
	tif_read.o \
	tif_swab.o \
	tif_strip.o \
	tif_thunder.o \
	tif_tile.o \
	tif_unix.o \
	tif_version.o \
	tif_warning.o \
	tif_write.o \
	tif_zip.o

# Ziele

all:	lib

depend:	$(SRCS)
	makedepend -- $(CPPFLAGS) -- $(SRCS)

clean:
	rm -f *.o *.bak *~ core libB*.a $(PRGS) $(AUTO) version.h

re:
	$(AR) $(ARXFLAGS) $(BLZ3_LIB)/libB3tiff.a

lib:	$(LIBS)

libB3tiff.a: $(OBJS)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@
	cp $@ $(BLZ3_LIB)

tif_fax3sm.c:	mkg3states
	mkg3states -c const tif_fax3sm.c

tif_version.c:	version.h

version.h:	mkversion
	mkversion -v ${VERSION} -a ${ALPHA} version.h

mkversion:	mkversion.o
	$(LINK.c) -o $@ mkversion.o $(LIB_S)
	chmod 775 mkversion
	cp mkversion $(BLZ3_BIN)

mkg3states:	mkg3states.o
	$(LINK.c) -o $@ mkg3states.o $(LIB_S)
	chmod 775 mkg3states
	cp mkg3states $(BLZ3_BIN)

# DO NOT DELETE


mkg3states.o: /home/blz3/include/tiff.h /usr/include/_G_config.h
mkg3states.o: /usr/include/alloca.h /usr/include/bits/confname.h
mkg3states.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
mkg3states.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
mkg3states.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
mkg3states.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
mkg3states.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
mkg3states.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
mkg3states.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
mkg3states.o: /usr/include/bits/wordsize.h /usr/include/endian.h
mkg3states.o: /usr/include/fcntl.h /usr/include/features.h
mkg3states.o: /usr/include/gconv.h /usr/include/getopt.h
mkg3states.o: /usr/include/gnu/stubs.h /usr/include/libio.h
mkg3states.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
mkg3states.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
mkg3states.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
mkg3states.o: /usr/include/sys/types.h /usr/include/time.h
mkg3states.o: /usr/include/unistd.h /usr/include/wchar.h
mkg3states.o: /usr/include/xlocale.h
mkg3states.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
mkg3states.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h port.h
mkg3states.o: tif_fax3.h
mkversion.o: /usr/include/_G_config.h /usr/include/alloca.h
mkversion.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
mkversion.o: /usr/include/bits/sched.h /usr/include/bits/select.h
mkversion.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
mkversion.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
mkversion.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
mkversion.o: /usr/include/endian.h /usr/include/features.h
mkversion.o: /usr/include/gconv.h /usr/include/gnu/stubs.h
mkversion.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
mkversion.o: /usr/include/string.h /usr/include/sys/cdefs.h
mkversion.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
mkversion.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
mkversion.o: /usr/include/xlocale.h
mkversion.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
mkversion.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_aux.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_aux.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_aux.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_aux.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_aux.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_aux.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_aux.o: /usr/include/bits/huge_val.h /usr/include/bits/mathcalls.h
tif_aux.o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
tif_aux.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_aux.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_aux.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_aux.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_aux.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_aux.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_aux.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_aux.o: /usr/include/gconv.h /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_aux.o: /usr/include/libio.h /usr/include/math.h /usr/include/stdio.h
tif_aux.o: /usr/include/stdlib.h /usr/include/string.h
tif_aux.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_aux.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_aux.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_aux.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_aux.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
tif_aux.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_aux.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_close.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_close.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_close.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_close.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_close.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_close.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_close.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_close.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_close.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_close.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_close.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_close.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_close.o: /usr/include/endian.h /usr/include/fcntl.h
tif_close.o: /usr/include/features.h /usr/include/gconv.h
tif_close.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_close.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_close.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_close.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_close.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_close.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_close.o: /usr/include/xlocale.h
tif_close.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_close.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_codec.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_codec.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_codec.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_codec.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_codec.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_codec.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_codec.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_codec.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_codec.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_codec.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_codec.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_codec.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_codec.o: /usr/include/endian.h /usr/include/fcntl.h
tif_codec.o: /usr/include/features.h /usr/include/gconv.h
tif_codec.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_codec.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_codec.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_codec.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_codec.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_codec.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_codec.o: /usr/include/xlocale.h
tif_codec.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_codec.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_compress.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_compress.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_compress.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_compress.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_compress.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_compress.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_compress.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_compress.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_compress.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_compress.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_compress.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_compress.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_compress.o: /usr/include/endian.h /usr/include/fcntl.h
tif_compress.o: /usr/include/features.h /usr/include/gconv.h
tif_compress.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_compress.o: /usr/include/libio.h /usr/include/stdio.h
tif_compress.o: /usr/include/stdlib.h /usr/include/string.h
tif_compress.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_compress.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_compress.o: /usr/include/sys/types.h /usr/include/time.h
tif_compress.o: /usr/include/unistd.h /usr/include/wchar.h
tif_compress.o: /usr/include/xlocale.h
tif_compress.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_compress.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_dir.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_dir.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_dir.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_dir.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_dir.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_dir.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_dir.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_dir.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_dir.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_dir.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_dir.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_dir.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_dir.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_dir.o: /usr/include/gconv.h /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_dir.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_dir.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_dir.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_dir.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_dir.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_dir.o: /usr/include/xlocale.h
tif_dir.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_dir.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_dirinfo.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_dirinfo.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_dirinfo.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_dirinfo.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_dirinfo.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_dirinfo.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_dirinfo.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_dirinfo.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_dirinfo.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_dirinfo.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_dirinfo.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_dirinfo.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_dirinfo.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_dirinfo.o: /usr/include/fcntl.h /usr/include/features.h
tif_dirinfo.o: /usr/include/gconv.h /usr/include/getopt.h
tif_dirinfo.o: /usr/include/gnu/stubs.h /usr/include/libio.h
tif_dirinfo.o: /usr/include/stdio.h /usr/include/stdlib.h
tif_dirinfo.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_dirinfo.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_dirinfo.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_dirinfo.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_dirinfo.o: /usr/include/xlocale.h
tif_dirinfo.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_dirinfo.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_dirread.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_dirread.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_dirread.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_dirread.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_dirread.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_dirread.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_dirread.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_dirread.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_dirread.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_dirread.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_dirread.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_dirread.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_dirread.o: /usr/include/endian.h /usr/include/fcntl.h
tif_dirread.o: /usr/include/features.h /usr/include/gconv.h
tif_dirread.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_dirread.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_dirread.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_dirread.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_dirread.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_dirread.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_dirread.o: /usr/include/xlocale.h
tif_dirread.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_dirread.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_dirwrite.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_dirwrite.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_dirwrite.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_dirwrite.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_dirwrite.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_dirwrite.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_dirwrite.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_dirwrite.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_dirwrite.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_dirwrite.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_dirwrite.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_dirwrite.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_dirwrite.o: /usr/include/endian.h /usr/include/fcntl.h
tif_dirwrite.o: /usr/include/features.h /usr/include/gconv.h
tif_dirwrite.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_dirwrite.o: /usr/include/libio.h /usr/include/stdio.h
tif_dirwrite.o: /usr/include/stdlib.h /usr/include/string.h
tif_dirwrite.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_dirwrite.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_dirwrite.o: /usr/include/sys/types.h /usr/include/time.h
tif_dirwrite.o: /usr/include/unistd.h /usr/include/wchar.h
tif_dirwrite.o: /usr/include/xlocale.h
tif_dirwrite.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_dirwrite.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_dumpmode.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_dumpmode.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_dumpmode.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_dumpmode.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_dumpmode.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_dumpmode.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_dumpmode.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_dumpmode.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_dumpmode.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_dumpmode.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_dumpmode.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_dumpmode.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_dumpmode.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_dumpmode.o: /usr/include/fcntl.h /usr/include/features.h
tif_dumpmode.o: /usr/include/gconv.h /usr/include/getopt.h
tif_dumpmode.o: /usr/include/gnu/stubs.h /usr/include/libio.h
tif_dumpmode.o: /usr/include/stdio.h /usr/include/stdlib.h
tif_dumpmode.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_dumpmode.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_dumpmode.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_dumpmode.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_dumpmode.o: /usr/include/xlocale.h
tif_dumpmode.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_dumpmode.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_error.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_error.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_error.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_error.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_error.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_error.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_error.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_error.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_error.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_error.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_error.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_error.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_error.o: /usr/include/endian.h /usr/include/fcntl.h
tif_error.o: /usr/include/features.h /usr/include/gconv.h
tif_error.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_error.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_error.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_error.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_error.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_error.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_error.o: /usr/include/xlocale.h
tif_error.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_error.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_fax3.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_fax3.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_fax3.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_fax3.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_fax3.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_fax3.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_fax3.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_fax3.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_fax3.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_fax3.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_fax3.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_fax3.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_fax3.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_fax3.o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
tif_fax3.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_fax3.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_fax3.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_fax3.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_fax3.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_fax3.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_fax3.o: /usr/include/xlocale.h
tif_fax3.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_fax3.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h t4.h
tif_fax3.o: tif_fax3.h
tif_flush.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_flush.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_flush.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_flush.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_flush.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_flush.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_flush.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_flush.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_flush.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_flush.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_flush.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_flush.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_flush.o: /usr/include/endian.h /usr/include/fcntl.h
tif_flush.o: /usr/include/features.h /usr/include/gconv.h
tif_flush.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_flush.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_flush.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_flush.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_flush.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_flush.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_flush.o: /usr/include/xlocale.h
tif_flush.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_flush.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_getimage.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_getimage.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_getimage.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_getimage.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_getimage.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_getimage.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_getimage.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_getimage.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_getimage.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_getimage.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_getimage.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_getimage.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_getimage.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_getimage.o: /usr/include/fcntl.h /usr/include/features.h
tif_getimage.o: /usr/include/gconv.h /usr/include/getopt.h
tif_getimage.o: /usr/include/gnu/stubs.h /usr/include/libio.h
tif_getimage.o: /usr/include/stdio.h /usr/include/stdlib.h
tif_getimage.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_getimage.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_getimage.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_getimage.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_getimage.o: /usr/include/xlocale.h
tif_getimage.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_getimage.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_jpeg.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_jpeg.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_jpeg.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_jpeg.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_jpeg.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_jpeg.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_jpeg.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_jpeg.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_jpeg.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_jpeg.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_jpeg.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_jpeg.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_jpeg.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_jpeg.o: /usr/include/gconv.h /usr/include/getopt.h
tif_jpeg.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
tif_jpeg.o: /usr/include/stdlib.h /usr/include/string.h
tif_jpeg.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_jpeg.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_jpeg.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_jpeg.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_jpeg.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_jpeg.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_luv.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_luv.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_luv.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_luv.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_luv.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_luv.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_luv.o: /usr/include/bits/fcntl.h /usr/include/bits/huge_val.h
tif_luv.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
tif_luv.o: /usr/include/bits/nan.h /usr/include/bits/posix_opt.h
tif_luv.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_luv.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_luv.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_luv.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_luv.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_luv.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_luv.o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
tif_luv.o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
tif_luv.o: /usr/include/math.h /usr/include/stdio.h /usr/include/stdlib.h
tif_luv.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_luv.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_luv.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_luv.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_luv.o: /usr/include/xlocale.h
tif_luv.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/float.h
tif_luv.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_luv.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h uvcode.h
tif_lzw.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_lzw.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_lzw.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_lzw.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_lzw.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_lzw.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_lzw.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_lzw.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_lzw.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_lzw.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_lzw.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_lzw.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_lzw.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_lzw.o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
tif_lzw.o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/libio.h
tif_lzw.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
tif_lzw.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_lzw.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_lzw.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_lzw.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_lzw.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_lzw.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_lzw.o: tif_predict.h
tif_next.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_next.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_next.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_next.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_next.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_next.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_next.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_next.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_next.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_next.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_next.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_next.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_next.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_next.o: /usr/include/gconv.h /usr/include/getopt.h
tif_next.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
tif_next.o: /usr/include/stdlib.h /usr/include/string.h
tif_next.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_next.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_next.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_next.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_next.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_next.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_open.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_open.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_open.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_open.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_open.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_open.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_open.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_open.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_open.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_open.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_open.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_open.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_open.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_open.o: /usr/include/gconv.h /usr/include/getopt.h
tif_open.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
tif_open.o: /usr/include/stdlib.h /usr/include/string.h
tif_open.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_open.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_open.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_open.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_open.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_open.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_packbits.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_packbits.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_packbits.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_packbits.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_packbits.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_packbits.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_packbits.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_packbits.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_packbits.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_packbits.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_packbits.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_packbits.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_packbits.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_packbits.o: /usr/include/fcntl.h /usr/include/features.h
tif_packbits.o: /usr/include/gconv.h /usr/include/getopt.h
tif_packbits.o: /usr/include/gnu/stubs.h /usr/include/libio.h
tif_packbits.o: /usr/include/stdio.h /usr/include/stdlib.h
tif_packbits.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_packbits.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_packbits.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_packbits.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_packbits.o: /usr/include/xlocale.h
tif_packbits.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_packbits.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_pixarlog.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_pixarlog.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_pixarlog.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_pixarlog.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_pixarlog.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_pixarlog.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_pixarlog.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_pixarlog.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_pixarlog.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_pixarlog.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_pixarlog.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_pixarlog.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_pixarlog.o: /usr/include/endian.h /usr/include/fcntl.h
tif_pixarlog.o: /usr/include/features.h /usr/include/gconv.h
tif_pixarlog.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_pixarlog.o: /usr/include/libio.h /usr/include/stdio.h
tif_pixarlog.o: /usr/include/stdlib.h /usr/include/string.h
tif_pixarlog.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_pixarlog.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_pixarlog.o: /usr/include/sys/types.h /usr/include/time.h
tif_pixarlog.o: /usr/include/unistd.h /usr/include/wchar.h
tif_pixarlog.o: /usr/include/xlocale.h
tif_pixarlog.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_pixarlog.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_predict.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_predict.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_predict.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_predict.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_predict.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_predict.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_predict.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_predict.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_predict.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_predict.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_predict.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_predict.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_predict.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_predict.o: /usr/include/fcntl.h /usr/include/features.h
tif_predict.o: /usr/include/gconv.h /usr/include/getopt.h
tif_predict.o: /usr/include/gnu/stubs.h /usr/include/libio.h
tif_predict.o: /usr/include/stdio.h /usr/include/stdlib.h
tif_predict.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_predict.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_predict.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_predict.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_predict.o: /usr/include/xlocale.h
tif_predict.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_predict.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_predict.o: tif_predict.h
tif_print.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_print.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_print.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_print.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_print.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_print.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_print.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_print.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_print.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_print.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_print.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_print.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_print.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/fcntl.h
tif_print.o: /usr/include/features.h /usr/include/gconv.h
tif_print.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_print.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_print.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_print.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_print.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_print.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_print.o: /usr/include/xlocale.h
tif_print.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_print.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_read.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_read.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_read.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_read.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_read.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_read.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_read.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_read.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_read.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_read.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_read.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_read.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_read.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_read.o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
tif_read.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_read.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_read.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_read.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_read.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_read.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_read.o: /usr/include/xlocale.h
tif_read.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_read.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_strip.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_strip.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_strip.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_strip.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_strip.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_strip.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_strip.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_strip.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_strip.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_strip.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_strip.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_strip.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_strip.o: /usr/include/endian.h /usr/include/fcntl.h
tif_strip.o: /usr/include/features.h /usr/include/gconv.h
tif_strip.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_strip.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_strip.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_strip.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_strip.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_strip.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_strip.o: /usr/include/xlocale.h
tif_strip.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_strip.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_swab.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_swab.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_swab.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_swab.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_swab.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_swab.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_swab.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_swab.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_swab.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_swab.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_swab.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_swab.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_swab.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_swab.o: /usr/include/gconv.h /usr/include/getopt.h
tif_swab.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
tif_swab.o: /usr/include/stdlib.h /usr/include/string.h
tif_swab.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_swab.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_swab.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_swab.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_swab.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_swab.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_thunder.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_thunder.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_thunder.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_thunder.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_thunder.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_thunder.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_thunder.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_thunder.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_thunder.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_thunder.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_thunder.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_thunder.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_thunder.o: /usr/include/endian.h /usr/include/fcntl.h
tif_thunder.o: /usr/include/features.h /usr/include/gconv.h
tif_thunder.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_thunder.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_thunder.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_thunder.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_thunder.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_thunder.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_thunder.o: /usr/include/xlocale.h
tif_thunder.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_thunder.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_tile.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_tile.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_tile.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_tile.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_tile.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_tile.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_tile.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_tile.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_tile.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_tile.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_tile.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_tile.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_tile.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_tile.o: /usr/include/gconv.h /usr/include/getopt.h
tif_tile.o: /usr/include/gnu/stubs.h /usr/include/libio.h /usr/include/stdio.h
tif_tile.o: /usr/include/stdlib.h /usr/include/string.h
tif_tile.o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
tif_tile.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_tile.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_tile.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_tile.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_tile.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_unix.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_unix.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_unix.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_unix.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_unix.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_unix.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_unix.o: /usr/include/bits/mman.h /usr/include/bits/posix_opt.h
tif_unix.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_unix.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_unix.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_unix.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_unix.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_unix.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_unix.o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
tif_unix.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_unix.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_unix.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_unix.o: /usr/include/sys/mman.h /usr/include/sys/select.h
tif_unix.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
tif_unix.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/unistd.h
tif_unix.o: /usr/include/wchar.h /usr/include/xlocale.h
tif_unix.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_unix.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_version.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_version.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_version.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_version.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_version.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_version.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_version.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_version.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_version.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_version.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_version.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_version.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_version.o: /usr/include/endian.h /usr/include/fcntl.h
tif_version.o: /usr/include/features.h /usr/include/gconv.h
tif_version.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_version.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_version.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_version.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_version.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_version.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_version.o: /usr/include/xlocale.h
tif_version.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_version.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_version.o: version.h
tif_warning.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_warning.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_warning.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_warning.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_warning.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_warning.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_warning.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_warning.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_warning.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_warning.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_warning.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_warning.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_warning.o: /usr/include/endian.h /usr/include/fcntl.h
tif_warning.o: /usr/include/features.h /usr/include/gconv.h
tif_warning.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_warning.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_warning.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_warning.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_warning.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_warning.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_warning.o: /usr/include/xlocale.h
tif_warning.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_warning.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_write.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_write.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_write.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_write.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_write.o: /usr/include/assert.h /usr/include/bits/confname.h
tif_write.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
tif_write.o: /usr/include/bits/fcntl.h /usr/include/bits/posix_opt.h
tif_write.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
tif_write.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tif_write.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
tif_write.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
tif_write.o: /usr/include/bits/waitstatus.h /usr/include/bits/wchar.h
tif_write.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tif_write.o: /usr/include/fcntl.h /usr/include/features.h /usr/include/gconv.h
tif_write.o: /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_write.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_write.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_write.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_write.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_write.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_write.o: /usr/include/xlocale.h
tif_write.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_write.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
tif_zip.o: /home/blz3/include/tif_dir.h /home/blz3/include/tiff.h
tif_zip.o: /home/blz3/include/tiffconf.h /home/blz3/include/tiffio.h
tif_zip.o: /home/blz3/include/tiffiop.h /home/blz3/include_unix/port.h
tif_zip.o: /usr/include/_G_config.h /usr/include/alloca.h
tif_zip.o: /usr/include/bits/confname.h /usr/include/bits/endian.h
tif_zip.o: /usr/include/bits/environments.h /usr/include/bits/fcntl.h
tif_zip.o: /usr/include/bits/posix_opt.h /usr/include/bits/pthreadtypes.h
tif_zip.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tif_zip.o: /usr/include/bits/sigset.h /usr/include/bits/stat.h
tif_zip.o: /usr/include/bits/stdio_lim.h /usr/include/bits/types.h
tif_zip.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tif_zip.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
tif_zip.o: /usr/include/endian.h /usr/include/fcntl.h /usr/include/features.h
tif_zip.o: /usr/include/gconv.h /usr/include/getopt.h /usr/include/gnu/stubs.h
tif_zip.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
tif_zip.o: /usr/include/string.h /usr/include/sys/cdefs.h
tif_zip.o: /usr/include/sys/select.h /usr/include/sys/stat.h
tif_zip.o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
tif_zip.o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
tif_zip.o: /usr/include/xlocale.h
tif_zip.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
tif_zip.o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
