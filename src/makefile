#
# basic makefile to create Blizzard III
#

CFLAGS		= $(OFLAGS) -D__$(OSTYPE)__
CXXFLAGS	= $(CFLAGS) -Wall -Wno-multichar

ifeq ($(HOSTTYPE),m68k-amiga)
	SUB_DIRS := system_amiga
else
	SUB_DIRS := system_unix
endif

SUB_DIRS +=\
	system\
	tools\
	Btiff\
	Bjpeg\
	base\
	image\
	raytrace\
	OpenGL\
	brt3\
	test_unix

PRGS	=\
	OpenGL\
	brt3\
	test_unix

all:	$(BLZ3_LIB) $(BLZ3_BIN) libs
	+@for i in $(PRGS); do \
		echo ""; \
		echo "Building "$$i"..."; \
		make -C $$i; \
	done

clean:	remdepend $(BLZ3_LIB)
	echo "Cleaning up..."
	find  $(BLZ3_LIB)  -name "*.a"   -exec rm {} \;
	find  $(BLZ3_HOME) -name "*.log" -exec rm {} \;
	find  $(BLZ3_HOME) -name "*.s"   -exec rm {} \;
	find  $(BLZ3_HOME) -name "*.il"  -exec rm {} \;
	find  $(BLZ3_HOME) -name "core*" -exec rm {} \;

	+@for i in $(SUB_DIRS); do \
		echo ""; \
		echo "Cleaning "$$i"..."; \
		make -C $$i clean; \
	done

remdepend:
	find . -name ".dep" -exec rm {} \;

tar:
	@$(BLZ3_HOME)/bin/configure.sh
	@cd $(BLZ3_HOME)/.. && tar cv blz3 | gzip > /tmp/blz3.tar.gz

win:	$(BLZ3_BIN)/RemCR
	make -C Btiff tif_fax3sm.c version.h
	cp Btiff/tif_fax3sm.c system_win32

count_all:
	wc -l `find $(BLZ3_HOME) -name "*.h";find $(BLZ3_HOME)/src -name "*.c";find $(BLZ3_HOME)/src -name "*.cc";find $(BLZ3_HOME)/src -name "*.cpp"`

count:
	wc -l `find $(BLZ3_HOME)/include* -name "*.h";ls $(BLZ3_HOME)/src/base/*.[c\|h]* $(BLZ3_HOME)/src/image/*.[c\|h]* $(BLZ3_HOME)/src/raytrace/*.[c\|h]* $(BLZ3_HOME)/src/AppLines/*.h $(BLZ3_HOME)/src/AppLines/*.cpp $(BLZ3_HOME)/src/system*/*.[c\|h]* $(BLZ3_HOME)/src/t*/*.[c\|h]*`

depend:	$(BLZ3_LIB) $(BLZ3_BIN)
	@for i in $(SUB_DIRS); do \
		echo ""; \
		make -C $$i depend; \
	done

install:	all
	@$(BLZ3_HOME)/bin/install.sh

#########################################################
#                                                       #
#  Blizzard III tools                                   #
#                                                       #
#########################################################

uncr:	$(BLZ3_BIN)/RemCR
	find $(BLZ3_HOME) -name "*.cc" -exec RemCR {} \;
	find $(BLZ3_HOME)/include $(BLZ3_HOME)/include_unix -name "*.h" -exec RemCR {} \;
	@for i in $(SUB_DIRS); do \
		find $$i -name "*.h" -exec RemCR {} \;;\
	done

$(BLZ3_BIN)/RemCR:	tools

resource.h:	$(BLZ3_BIN)/CleanRes tools
	$(BLZ3_BIN)/CleanRes AppLines/Resource.h >/tmp/Resource.h
	mv /tmp/Resource.h AppLines/Resource.h

$(BLZ3_BIN)/CleanRes:	tools

tools:	$(BLZ3_BIN) $(BLZ3_LIB) $(BLZ3_LIB)/libB3System.a
	+@make -C tools

#########################################################
#                                                       #
#  Blizzard III libraries                               #
#                                                       #
#########################################################

$(BLZ3_LIB):
	mkdir -p $@

$(BLZ3_BIN):
	mkdir -p $@

$(BLZ3_LIB)/libB3System.a:	$(BLZ3_LIB) lib_system lib_system_ind
	$(AR) $(ARFLAGS) $@ system_unix/*.o system/*.o
	$(RANLIB) $@

$(BLZ3_LIB)/libB3Raytrace.a:	$(BLZ3_LIB) lib_raytrace
	$(AR) $(ARFLAGS) $@ raytrace/libB3Raytrace.o
	$(RANLIB) $@

$(BLZ3_LIB)/libB3Base.a:	$(BLZ3_LIB) lib_jpeg lib_tiff lib_image lib_base lib_system lib_system_ind
	$(AR) $(ARFLAGS) $@ Bjpeg/libB3jpeg.o Btiff/libB3tiff.o image/libB3Image.o base/libB3Base.o system_unix/*.o system/*.o
	$(RANLIB) $@

lib_system:
	+@make -C system_unix
	
lib_system_ind:
	+@make -C system
	
lib_base:
	+@make -C base

lib_image:
	+@make -C image

lib_raytrace:
	+@make -C raytrace

lib_tiff:
	+@make -C Btiff

lib_jpeg:
	+@make -C Bjpeg

libs:	$(BLZ3_LIB)/libB3System.a $(BLZ3_LIB)/libB3Raytrace.a $(BLZ3_LIB)/libB3Base.a
