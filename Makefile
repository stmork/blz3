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
	test -f data.tar.gz && rm -f data.tar.gz

autoconf:	
	autoconf
	rm -rf autom4te.cache

configure:	autoconf
	./configure --prefix=$(PWD)

clobber:
	bin/clobber.sh $(PWD)

tar:	clobber autoconf
	(cd ..; tar c blz3 | gzip -9 > /tmp/blz3.tar.gz; )

tar-rpm:	clobber autoconf data.tar.gz
	(cd ..; tar c blz3 | gzip -9 > /tmp/blz3.tar.gz; )

data.tar.gz:
	test -d $(HOME)/Blizzard && tar cf - -C $(HOME)/Blizzard Data Objects Materials Bumps Conditions Textures|gzip -9 >data.tar.gz
