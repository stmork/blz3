all:	source

dep:	depend

depend:
	+make -C src depend

source:
	+make -C src

install:
	+make -C src install

clean:
	+make -C src clean

autoconf:	
	autoconf
	rm -rf autom4te.cache

configure:	autoconf
	./configure

distclean:
	rm -rf `find src -name Makefile` include_unix/blz3/autoconf.h config.* autom4te.cache
	test -d Debug && rm -rf Debug
	test -d Release && rm -rf Release

autoclean:	distclean
	rm configure

tar:
	source bin/clobber.sh
