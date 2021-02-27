TEMPLATE = subdirs
SUBDIRS  = system_unix system base image
CONFIG   = ordered

system_unix.file = system_unix/system_unix.pro
system.file      = system/system.pro
base.file        = base/base.pro
image.file       = image/image.pro

image.depends = system
base.depends  = system image
