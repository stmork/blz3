# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
CFLAGS			= -g
CXXFLAGS		= -g
LDLIBS			= -lB3System -lpthread
PRGS			= RemDepend
REMDEPEND_OBJS		= RemDepend.o
PRG_SOURCE		= RemDepend.cc
PRG_OBJS		= $(REMDEPEND_OBJS)

all:	$(PRGS)

depend:
	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)

clean:
	rm -f *.o *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

RemDepend:	$(REMDEPEND_OBJS)
	$(LINK.cc) -o $@ $(REMDEPEND_OBJS) $(LDLIBS)
	chmod 775 $@
	cp $@ $(BLZ3_BIN)

# DO NOT DELETE


RemDepend..o: /home/blz3/include/blz3/b3Types.h
RemDepend..o: /home/blz3/include_unix/blz3/system/b3File.h
RemDepend..o: /home/blz3/include_unix/blz3/system/b3Mem.h
RemDepend..o: /home/blz3/include_unix/blz3/system/b3Thread.h
RemDepend..o: /usr/include/_G_config.h /usr/include/bits/confname.h
RemDepend..o: /usr/include/bits/endian.h /usr/include/bits/environments.h
RemDepend..o: /usr/include/bits/initspin.h /usr/include/bits/posix_opt.h
RemDepend..o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
RemDepend..o: /usr/include/bits/select.h /usr/include/bits/sigset.h
RemDepend..o: /usr/include/bits/sigthread.h /usr/include/bits/stdio_lim.h
RemDepend..o: /usr/include/bits/time.h /usr/include/bits/types.h
RemDepend..o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
RemDepend..o: /usr/include/ctype.h /usr/include/endian.h
RemDepend..o: /usr/include/features.h /usr/include/gconv.h
RemDepend..o: /usr/include/getopt.h /usr/include/gnu/stubs.h
RemDepend..o: /usr/include/libio.h /usr/include/pthread.h /usr/include/sched.h
RemDepend..o: /usr/include/signal.h /usr/include/stdio.h /usr/include/string.h
RemDepend..o: /usr/include/sys/cdefs.h /usr/include/sys/select.h
RemDepend..o: /usr/include/sys/sysmacros.h /usr/include/sys/types.h
RemDepend..o: /usr/include/time.h /usr/include/unistd.h /usr/include/wchar.h
RemDepend..o: /usr/include/xlocale.h
RemDepend..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stdarg.h
RemDepend..o: /usr/lib/gcc-lib/i486-suse-linux/2.95.2/include/stddef.h
