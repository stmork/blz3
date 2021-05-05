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
	rm -rf doc/html doc/latex Doxyfile lcov-out BlizzardIII.qch *.deb

configure:	configure.ac
	autoreconf
	rm -rf autom4te.cache

config:	configure
	./configure BLZ3_DOC=$(PWD)/doc --prefix=$(PWD)

distclean:	clean
	+make -C src $@
	bin/clobber.sh $(PWD)

dist:	configure
	(cd ..; tar c blz3 | gzip -9 > /tmp/blz3.tar.gz; )
	cp blz3.spec /tmp/

astyle:
	+find . -name "*.h" | xargs astyle
	+find . -name "*.cc" | xargs astyle
	+astyle src/OpenGL/*.cpp

cppcheck:
	+make -C src $@

lcov:
	+make -C src $@

documentation:	Doxyfile
	doxygen Doxyfile

Doxyfile:	Doxyfile.in configure
	./configure BLZ3_DOC=$(PWD)/doc --prefix=$(PWD)

# DO NOT DELETE
