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
	./configure --prefix=$(PWD)

clobber:
	bin/clobber.sh $(PWD)

tar:	clobber autoconf
	(cd ..; tar c blz3 | gzip -9 > /tmp/blz3.tar.gz; )
