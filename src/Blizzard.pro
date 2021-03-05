TEMPLATE = subdirs
SUBDIRS  = system_unix system base image raytrace unittest brt3 render qrender
CONFIG   = ordered

include(common.pri)

message("*** Blizzard III Qt version $$VERSION ***")

system_unix.file = system_unix/system_unix.pro
system.file      = system/system.pro
base.file        = base/base.pro
image.file       = image/image.pro
raytrace.file    = raytrace/raytrace.pro
unittest.file    = unittest/unittest.pro
brt3.file        = brt3/brt3.pro
render.file      = OpenGL/render.pro
qrender.file     = OpenGL/qrender.pro

system.depends   = system_unix
image.depends    = system
base.depends     = system image
raytrace.depends = system base image
unittest.depends = system base image
brt3.depends     = raytrace
render.depends   = raytrace
qrender.depends  = raytrace

QMAKE_EXTRA_TARGETS += cppcheck

cppcheck.commands = cppcheck\
	--enable=style,warning,performance,portability\
	--inline-suppr\
	--suppress=unusedStructMember\
	--suppress=useStlAlgorithm\
	--suppress=noCopyConstructor\
	--suppress=noOperatorEq\
	-I$$[QT_INSTALL_HEADERS]\
	-I$$BLZ3_INCLUDE -I$$BLZ3_HOME/include_unix\
	--language=c++ --std=c++14\
	--xml-version=2 --force -q -j 3\
	system/*.cc system_unix/*.cc base/*.cc image/*.cc raytrace/*.cc 2>cppcheck.xml

QMAKE_CLEAN += cppcheck.xml test-results*.xml *.wav *.dot *.mp4 *.deb *.qch
