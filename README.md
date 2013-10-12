# tcptunnel [![Build Status](https://api.travis-ci.org/vakuum/tcptunnel.png?branch=master)](https://travis-ci.org/vakuum/tcptunnel)

Tcptunnel is a simple TCP port forwarder. Please see http://www.vakuumverpackt.de/tcptunnel/ for details and binary downloads.

## Building

### For Unix

```
$ git clone https://github.com/vakuum/tcptunnel.git
$ cd tcptunnel
$ ./configure
$ make
$ ./tcptunnel --version
$ ./tcptunnel --help
```

### For Mac OS X

You will need the [Command Line Tools for Xcode](https://developer.apple.com/xcode/) to build tcptunnel under Mac OS X.

```
$ uname -mrs
Darwin 12.5.0 x86_64

$ git clone https://github.com/vakuum/tcptunnel.git
$ cd tcptunnel
$ ./configure --prefix=/usr/bin
$ make
$ ./tcptunnel --version
$ ./tcptunnel --help
$ file tcptunnel
tcptunnel: Mach-O 64-bit executable x86_64
```

### For Windows (MinGW32)

You will need MinGW32 to cross-compile tcptunnel. Please see http://www.mingw.org/ for more details. If you are using a Debian-based distribution then you will need to install the following packages:

* mingw32
* mingw32-binutils
* mingw32-runtime

```
$ apt-get install mingw32 mingw32-binutils mingw32-runtime
$ git clone https://github.com/vakuum/tcptunnel.git
$ cd tcptunnel
$ ./configure
$ make -f Makefile.MinGW32
$ file tcptunnel.exe
tcptunnel.exe: PE32 executable (console) Intel 80386, for MS Windows
```

Note: The MinGW32-based version does not support the fork-based concurrent client handling. If you need this feature under Windows, then you should use the Cygwin-based version.

### For Windows (Cygwin)

You will need the Cygwin environment for Windows from http://www.cygwin.com/ with the following additional packages installed:

* gcc
* git
* make

```
$ uname -a
CYGWIN_NT-6.1-WOW64 computer 1.7.17(0.262/5/3) 2012-10-19 14:39 i686 Cygwin

$ git clone git://github.com/vakuum/tcptunnel.git
$ cd tcptunnel
$ ./configure
$ make
$ file tcptunnel.exe
tcptunnel.exe: PE32 executable (console) Intel 80386, for MS Windows
```

## Help

````
$ tcptunnel --help
Usage: tcptunnel [options]

Options:
  --version
  --help

  --local-port=PORT    local port
  --remote-port=PORT   remote port
  --remote-host=HOST   remote host
  --bind-address=IP    bind address
  --buffer-size=BYTES  buffer size
  --fork               fork-based concurrency
  --log
  --stay-alive
````

## ChangeLog

See [ChangeLog](https://raw.github.com/vakuum/tcptunnel/master/ChangeLog).

## License

Copyright (c) 2000-2013 Clemens Fuchslocher, released under the GPL.

