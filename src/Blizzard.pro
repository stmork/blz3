TEMPLATE = subdirs
SUBDIRS  = system_unix system base image raytrace unittest brt3 render
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

system.depends   = system_unix
image.depends    = system
base.depends     = system image
raytrace.depends = system base image
unittest.depends = system base image
brt3.depends     = raytrace
render.depends   = raytrace
