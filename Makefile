all:	source

dep:	depend

depend:
	+make -C src $@

source:
	+make -C src

count:
	test -f src/Makefile && make -C src $@

count_all:
	test -f src/Makefile && make -C src $@

uncr:
	test -f src/Makefile && make -C src $@

test:
	+test -f src/Makefile && make -C src $@

install:
	+make -C src $@

clean:
	test -f src/Makefile && make -C src $@
	rm -rf doc/html doc/latex Doxyfile

configure:	configure.ac
	autoconf
	rm -rf autom4te.cache

config:	configure
	./configure BLZ3_DOC=$(PWD)/doc --prefix=$(PWD)

distclean:	clean
	bin/clobber.sh $(PWD)

dist:	configure
	(cd ..; tar c blz3 | gzip -9 > /tmp/blz3.tar.gz; )
	cp blz3.spec /tmp/

documentation:	Doxyfile
	doxygen Doxyfile

Doxyfile:	Doxyfile.in configure
	./configure BLZ3_DOC=$(PWD)/doc --prefix=$(PWD)

# DO NOT DELETE
