# Welcome to Blizzard III - a fast multiplatform raytracer.

This is ongoing work to migrate the raytracing framework to Qt. There are
two ways to compile:
1. Using the standard configure mechanism
2. Using Qt with ```qmake```

## configure

First you have to configure the framework using the configure script. The
configure script respects the ```CXXFLAGS``` environment variable so setting
to ```-O3``` would be what you want.

```
make config depend
```

Then you can compile simply using ```make```. Of cource you can use the
```-j``` option for faster compilation. After that you may call the test
target for unit testing:
```
make -j 4 test
```

After compilation and testing you can use the ```install``` target to install to
the configured prefix location. Also you kann build the documentation using
the ```documentation``` target.

Example:
```
make install documentation
```

### Cross Compiling
If you want to cross compile you have to specify the sysroot directory to
build against. You should give this by using the ```CXXFLAGS``` environment
variale and setting the ```--sysroot=<DIR>``` option. After that you have to
add the ```--host=<TARGET_ARCH>``` option to the configure script.

Example:
```
export CXXFLAGS="-O3 --sysroot=/opt/rootfs/armhf/develop/focal"
./configure --host=arm-linux-gnueabihf
```

Look to your requested environment to cross compile against other CPU
architectures.

## Qt with qmake
After configuring like described you have to remove the Makefile generated
during ```configure```:

```
make config depend
cd src
qmake
rm */Makefile
cd ..

make install
```

With ```qmake``` you have the following ```CONFIG``` options:
1. ```qmake CONFIG+=release``` for optimized release build.
2. ```qmake CONFIG+=debug``` for debug compilation.
3. ```qmake CONFIG+=debug CONFIG+=gcov``` for debug compilation and extra
preparation for code coverage.

## Debian
If you use the ```bin/jenkins.sh``` script the framework compiles, tests and
installs for packaging into Debian packages.
