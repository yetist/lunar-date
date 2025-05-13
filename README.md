# LunarDate

## General Information

LunarDate is a chinese lunar library.

The official web site is: <https://lunar-date.zhcn.cc>

You can download the latest lunar-date tarball from the github.com:

<https://github.com/yetist/lunar-date/releases>

## Build

**For Linux:**

Build and install into the system:

```sh
$ meson setup _build --prefix=/usr -Ddocs=true -Dintrospection=true -Dvapi=true -Dtests=true
$ meson compile -C _build/
$ sudo meson install -C _build
```

**For Windows:**

Building dynamic link libraries for Windows on Linux is now supported:

```sh
$ PKG_CONFIG_PATH=/mingw64/lib/pkgconfig/ meson setup _build.mingw/ --cross-file cross/mingw.txt --prefix=/usr -Ddocs=true -Dtests=true
$ meson compile -C _build.mingw/
$ file _build.mingw/lunar-date/liblunar-date-3.0-1.dll
_build.mingw/lunar-date/liblunar-date-3.0-1.dll: PE32+ executable for MS Windows 5.02 (DLL), x86-64, 20 sections
$ file _build.mingw/service/*.exe
_build.mingw/service/lunar-date-daemon.exe: PE32+ executable for MS Windows 5.02 (console), x86-64, 19 sections
_build.mingw/service/lunar-date.exe:        PE32+ executable for MS Windows 5.02 (console), x86-64, 19 sections
```
## Linux distribution support

Binary packages for the following systems are currently available.

[![Packaging status](https://repology.org/badge/vertical-allrepos/lunar-date.svg)](https://repology.org/project/lunar-date/versions)

## How to report bugs

Bugs should be reported to the [github issue](https://github.com/yetist/lunar-date/issues) bug tracking system.
