#
# basic makefile to create Blizzard III
#

CFLAGS		= $(OFLAGS) -D__$(OSTYPE)__
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
	OpenGL\
	brt3\
	check

PRGS	=\
	test_unix\
	OpenGL\
	brt3\
	check

all:	$(BLZ3_LIB) $(BLZ3_BIN) libs
	+@for i in $(PRGS); do \
		echo ""; \
		echo "Building "$$i"..."; \
		make -C $$i; \
	done

clean:	remdepend
	echo "Cleaning up..."
	find  $(BLZ3_HOME) -name "*.log" -exec rm {} \;
	find  $(BLZ3_HOME) -name "core"  -exec rm {} \;

	+@for i in $(SUB_DIRS); do \
		echo ""; \
		echo "Cleaning "$$i"..."; \
		make -C $$i clean; \
	done

win:
	make -C Btiff tif_fax3sm.c version.h
	cp Btiff/tif_fax3sm.c system_win32

count_all:
	wc -l `find $(BLZ3_HOME) -name "*.h";find $(BLZ3_HOME)/src -name "*.c";find $(BLZ3_HOME)/src -name "*.cc";find $(BLZ3_HOME)/src -name "*.cpp"`

count:
	wc -l `find $(BLZ3_HOME)/include* -name "*.h";ls $(BLZ3_HOME)/src/base/*.[c\|h]* $(BLZ3_HOME)/src/image/*.[c\|h]* $(BLZ3_HOME)/src/raytrace/*.[c\|h]* $(BLZ3_HOME)/src/AppLines/*.h $(BLZ3_HOME)/src/AppLines/*.cpp $(BLZ3_HOME)/src/system*/*.[c\|h]* $(BLZ3_HOME)/src/test*/*.[c\|h]*`

depend:	$(BLZ3_LIB) $(BLZ3_BIN)
	@for i in $(SUB_DIRS); do \
		echo ""; \
		make -C $$i depend; \
	done

#########################################################
#                                                       #
#  Blizzard III tools                                   #
#                                                       #
#########################################################

remdepend:
	find . -name ".dep" -exec rm {} \;

tar:
	@$(BLZ3_HOME)/bin/configure.sh
	@cd $(BLZ3_HOME)/.. && tar cv blz3 | gzip > /tmp/blz3.tar.gz

uncr:	$(BLZ3_BIN)/RemCR
	find $(BLZ3_HOME) -name "*.cc" -exec RemCR {} \;
	find $(BLZ3_HOME)/include $(BLZ3_HOME)/include_unix -name "*.h" -exec RemCR {} \;
	@for i in $(SUB_DIRS); do \
		find $$i -name "*.h" -exec RemCR {} \;;\
	done

$(BLZ3_BIN)/RemDepend:	$(BLZ3_BIN) $(BLZ3_LIB) $(BLZ3_LIB)/libB3System.a
	+@make -C tools RemDepend

$(BLZ3_BIN)/RemCR:	$(BLZ3_BIN) $(BLZ3_LIB) $(BLZ3_LIB)/libB3System.a
	+@make -C tools RemCR

#########################################################
#                                                       #
#  Blizzard III libraries                               #
#                                                       #
#########################################################

$(BLZ3_LIB):
	mkdir -p $@

$(BLZ3_BIN):
	mkdir -p $@

$(BLZ3_LIB)/libB3System.a:	$(BLZ3_LIB)
	+@make -C system_unix

$(BLZ3_LIB)/libB3Raytrace.a:	$(BLZ3_LIB)
	+@make -C raytrace

$(BLZ3_LIB)/libB3Base.a:	$(BLZ3_LIB) lib_base lib_image lib_tiff lib_jpeg $(BLZ3_LIB)/libB3System.a
	$(AR) $(ARFLAGS) $@ Bjpeg/*.o Btiff/*.o image/*.o base/*.o system_unix/*.o
	$(RANLIB) $@

lib_base:
	+@make -C base

lib_image:
	+@make -C image

lib_tiff:
	+@make -C Btiff

lib_jpeg:
	+@make -C Bjpeg

libs:	$(BLZ3_LIB)/libB3System.a $(BLZ3_LIB)/libB3Raytrace.a $(BLZ3_LIB)/libB3Base.a
