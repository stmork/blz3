TEMPLATE = subdirs
SUBDIRS  = system_unix system base image raytrace unittest brt3 bimg3 render qrender
CONFIG  += ordered

include(common.pri)

message("*** Blizzard III Qt version $$VERSION ***")

system_unix.file = system_unix/system_unix.pro
system.file      = system/system.pro
base.file        = base/base.pro
image.file       = image/image.pro
raytrace.file    = raytrace/raytrace.pro
unittest.file    = unittest/unittest.pro
brt3.file        = brt3/brt3.pro
bimg3.file       = brt3/bimg3.pro
render.file      = OpenGL/render.pro
qrender.file     = OpenGL/qrender.pro

system.depends   = system_unix
image.depends    = system
base.depends     = system image
raytrace.depends = system base image
unittest.depends = system base image

brt3.depends     = raytrace
bimg3.depends    = image
render.depends   = raytrace
qrender.depends  = raytrace

#####################################################################
#
# Extra target cppcheck.
#
#####################################################################

cppcheck.commands = cppcheck\
	--enable=style,warning,performance,portability\
	--inline-suppr\
	--suppress=preprocessorErrorDirective\
	--suppress=syntaxError\
	--suppress=unusedStructMember\
	--suppress=useStlAlgorithm\
	--suppress=noCopyConstructor\
	--suppress=noOperatorEq\
	-I$$[QT_INSTALL_HEADERS]\
	-I$$BLZ3_INCLUDE -I$$BLZ3_HOME/include_unix\
	--language=c++ --std=c++14\
	--xml-version=2 --force -q -j 8\
	system/*.cc system_unix/*.cc base/*.cc image/*.cc raytrace/*.cc\
	OpenGL/*.c* OpenGL/*.h brt3/*.cc 2>cppcheck.xml

QMAKE_EXTRA_TARGETS += cppcheck
QMAKE_CLEAN         += cppcheck.xml test-results*.xml *.wav *.dot *.mp4 *.deb *.qch

#####################################################################
#
# Extra target lcov
#
#####################################################################

lcov.commands += lcov --no-external -c --directory $$BLZ3_HOME -o coverage.info;
lcov.commands += genhtml coverage.info --legend --title=\"Blizzard III base libraries\" --output-directory $$BLZ3_LCOV --rc genhtml_med_limit=50

QMAKE_EXTRA_TARGETS += lcov
QMAKE_CLEAN         += $$BLZ3_LCOV
