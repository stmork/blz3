# Unit tests
This project directory contains unit tests of Blizzard III. You have to
prepare the project using the description in the base directory of this
project.

## Code Coverage
If you have prepared code coverage using gcov/lcov in conjunction with Qt
you cat simply call
```
./unittest
```

## Valgrind usage

Since there are some false positive caused in GLib there exists a
suppression file for valgrind names glib.supp downloaded from

https://gitlab.gnome.org/GNOME/glib/-/blob/master/glib.supp

The hint was given by an issue at the Gnome environment:

https://gitlab.gnome.org/GNOME/glib/-/issues/2076

Using qmake in this directory you can simply call
```
make valgrind
```
