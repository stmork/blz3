TEMPLATE = subdirs
SUBDIRS  = system_unix system base image raytrace
CONFIG   = ordered

system_unix.file = system_unix/system_unix.pro
system.file      = system/system.pro
base.file        = base/base.pro
image.file       = image/image.pro
raytrace.file    = raytrace/raytrace.pro

system.depends = system_unix
image.depends = system
base.depends  = system image
raytrace.depends = system base image
