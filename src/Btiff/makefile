# Beispiel Makefile

# Optionen

CPPFLAGS        = $(STDINC)
LIBS		= libB3tiff.o
PRGS		= mkversion mkg3states
AUTO		= tif_fax3sm.c version.h

ALPHA	= tiff.alpha
VERSION	= VERSION

LIB_SOURCE	= \
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

LIB_OBJ	= \
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

all:	$(LIBS)

depend:	$(AUTO)
#	makedepend -- $(CPPFLAGS) -- $(SRCS)
	gcc -M  $(LIB_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.a *.bak *~ core libB*.a $(PRGS) $(LIBS) $(AUTO) ALL QUIET version.h

libB3tiff.o: $(LIB_OBJ)
	rm -f core
	$(LD) -r -o $@ $^

libB3tiff.a:	$(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^
	cp $@ $(BLZ3_LIB)

tif_fax3sm.c:	mkg3states
	mkg3states -c const tif_fax3sm.c

tif_version.c:	version.h

version.h:	mkversion
	mkversion -v ${VERSION} -a ${ALPHA} version.h

mkversion:	mkversion.o
	$(LINK.c) -o $@ mkversion.o $(LIB_S)
	cp mkversion $(BLZ3_BIN)
	chmod 775 $@ $(BLZ3_BIN)/$@

mkg3states:	mkg3states.o
	$(LINK.c) -o $@ mkg3states.o $(LIB_S)
	cp mkg3states $(BLZ3_BIN)
	chmod 775 $@ $(BLZ3_BIN)/$@

-include .dep
