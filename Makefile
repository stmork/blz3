all:	source

dep:	depend

depend:
	+make -C src $@

source:
	+make -C src

count:
	+make -C src $@

install:
	+make -C src install

clean:
	test -f src/Makefile && make -C src clean

configure:	configure.ac
	autoconf
	rm -rf autom4te.cache

config:	configure
	./configure --prefix=$(PWD)

clobber:
	bin/clobber.sh $(PWD)

tar:	clobber configure
	(cd ..; tar c blz3 | gzip -9 > /tmp/blz3.tar.gz; )
	cp blz3.spec /tmp/
# DO NOT DELETE
