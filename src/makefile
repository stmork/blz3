#
# basic makefile to create Blizzard II
#

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
	test_unix

all:	$(BLZ3_LIB) $(BLZ3_BIN)
	+@for i in $(SUB_DIRS); do \
		echo ""; \
		echo "Building "$$i"..."; \
		make -C $$i; \
	done

$(BLZ3_LIB):
	mkdir -p $@

$(BLZ3_BIN):
	mkdir -p $@

clean:
	echo "Cleaning up..."
	@find . -name "*.log" -exec rm {} \;
	@find . -name "*.ncb" -exec rm {} \;
	@find . -name "*.plg" -exec rm {} \;
	@find . -name "*.dep" -exec rm {} \;
	@find . -name "*.mak" -exec rm {} \;
	@find . -name "*.bak" -exec rm {} \;
	@find . -name "*~"    -exec rm {} \;
	@find . -name "core"  -exec rm {} \;
	@rm -rf */Debug */Release

	+@for i in $(SUB_DIRS); do \
		echo ""; \
		echo "Cleaning "$$i"..."; \
		make -C $$i clean; \
	done

win:
	make -C Btiff tif_fax3sm.c version.h
#	make -C BRaytrace version.h

count:
	wc -l `find $(BLZ3_HOME)/include* -name "*.h";find $(BLZ3_HOME)/src -name "*.cc"`

depend:	$(BLZ3_LIB) $(BLZ3_BIN)
	@for i in $(SUB_DIRS); do \
		echo ""; \
		make -C $$i depend; \
	done

$(BLZ3_BIN)/RemDepend:	$(BLZ3_BIN) $(BLZ3_LIB)/libB3System.a
	+@make -C tools RemDepend

$(BLZ3_BIN)/RemCR:	$(BLZ3_BIN) $(BLZ3_LIB)/libB3System.a
	+@make -C tools RemCR

$(BLZ3_LIB)/libB3System.a:	$(BLZ3_LIB)
	+@make -C system_unix

remdepend:	$(BLZ3_BIN)/RemDepend clean
	RemDepend */makefile

tar:
	@$(BLZ3_HOME)/bin/configure.sh
	@cd $(BLZ3_HOME)/.. && tar cvf /tmp/blz3.tar blz3 && gzip -f /tmp/blz3.tar

uncr:	$(BLZ3_BIN)/RemCR
	find $(BLZ3_HOME) -name "*.cc" -exec RemCR {} \;
	find $(BLZ3_HOME) -name "*.h"  -exec RemCR {} \;
