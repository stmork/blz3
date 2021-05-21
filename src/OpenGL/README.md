# YAKINDU SCT
If you want to generate YAKINDU SCT classes inside the _src-gen_ and
_src-lib_ folders you have to install that tool. Note that you have to
install Java SDK 11 prior running eclipse.

## Generating using ant
To run headless you have to set the variable _${eclipse.home}_ inside the
file _$HOME/.sct.properties_ to your eclipse installation folder. Then you
can generate by simply caling
```
ant
```

## Generating using Qt
If you have _ant_ running you can also generate by simple calling
```
make generate
```
