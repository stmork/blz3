#
# basic makefile to create Blizzard II
#

CFLAGS		= $(OFLAGS)
CXXFLAGS	= $(CFLAGS)

ifeq ($(HOSTTYPE),m68k-amiga)
	SUB_DIRS := system_amiga
else
	SUB_DIRS := system_unix
endif

SUB_DIRS +=\
	tools\
	Btiff\
	Bjpeg\
	base\
	image\
	raytrace\
	test_unix\
	OpenGL

all:	$(BLZ3_LIB) $(BLZ3_BIN)
	+@for i in $(SUB_DIRS); do \
		echo ""; \
		echo "Building "$$i"..."; \
		make -C $$i; \
	done

clean:
	echo "Cleaning up..."
	find   $(BLZ3_HOME) -name "*.log" -exec rm {} \;
	find   $(BLZ3_HOME) -name "*.ncb" -exec rm {} \;
	find   $(BLZ3_HOME) -name "*.plg" -exec rm {} \;
	find   $(BLZ3_HOME) -name "*.dep" -exec rm {} \;
	find   $(BLZ3_HOME) -name "*.mak" -exec rm {} \;
	find   $(BLZ3_HOME) -name "*.bak" -exec rm {} \;
	find   $(BLZ3_HOME) -name "*~"    -exec rm {} \;
	find   $(BLZ3_HOME) -name ".#*"   -exec rm {} \;
	find   $(BLZ3_HOME) -name "core"  -exec rm {} \;
	@rm -rf $(BLZ3_HOME)/Debug $(BLZ3_HOME)/Release

	+@for i in $(SUB_DIRS); do \
		echo ""; \
		echo "Cleaning "$$i"..."; \
		make -C $$i clean; \
	done

win:
	make -C Btiff tif_fax3sm.c version.h
	cp Btiff/tif_fax3sm.c system_win32

count:
	wc -l `find $(BLZ3_HOME)/include* -name "*.h";find $(BLZ3_HOME)/src -name "*.cc";find $(BLZ3_HOME)/src -name "*.cpp"`

depend:	$(BLZ3_LIB) $(BLZ3_BIN)
	@for i in $(SUB_DIRS); do \
		echo ""; \
		make -C $$i depend; \
	done

remdepend:	clean
	find . -name ".dep" -exec rm {} \;

tar:
	@$(BLZ3_HOME)/bin/configure.sh
	@cd $(BLZ3_HOME)/.. && tar cvf /tmp/blz3.tar blz3 && gzip -f /tmp/blz3.tar

uncr:	$(BLZ3_BIN)/RemCR
	find $(BLZ3_HOME) -name "*.cc" -exec RemCR {} \;
	find $(BLZ3_HOME) -name "*.h"  -exec RemCR {} \;

$(BLZ3_LIB):
	mkdir -p $@

$(BLZ3_BIN):
	mkdir -p $@

$(BLZ3_LIB)/libB3System.a:	$(BLZ3_LIB)
	+@make -C system_unix

$(BLZ3_BIN)/RemDepend:	$(BLZ3_BIN) $(BLZ3_LIB) $(BLZ3_LIB)/libB3System.a
	+@make -C tools RemDepend

$(BLZ3_BIN)/RemCR:	$(BLZ3_BIN) $(BLZ3_LIB) $(BLZ3_LIB)/libB3System.a
	+@make -C tools RemCR
