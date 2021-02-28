TEMPLATE = subdirs
SUBDIRS  = system_unix system base image raytrace unittest
CONFIG   = ordered

include(common.pri)

message("*** Blizzard III Qt version $$VERSION ***")

system_unix.file = system_unix/system_unix.pro
system.file      = system/system.pro
base.file        = base/base.pro
image.file       = image/image.pro
raytrace.file    = raytrace/raytrace.pro
unittest.file    = unittest/unittest.pro

system.depends   = system_unix
image.depends    = system
base.depends     = system image
raytrace.depends = system base image
unittest.depends = system base image
