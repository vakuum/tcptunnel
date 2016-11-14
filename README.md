# tcptunnel [![Build Status](https://api.travis-ci.org/vakuum/tcptunnel.png?branch=master)](https://travis-ci.org/vakuum/tcptunnel)

Tcptunnel is a simple TCP port forwarder. Please see http://www.vakuumverpackt.de/tcptunnel/ for details and binary downloads.

## Help

```
$ tcptunnel --help
Usage: tcptunnel [options]

Options:
  --version
  --help

  --local-port=PORT    local port
  --remote-port=PORT   remote port
  --remote-host=HOST   remote host
  --bind-address=IP    bind address
  --client-address=IP  only accept connections from this IP address
  --buffer-size=BYTES  buffer size
  --fork               fork-based concurrency
  --log
  --stay-alive
```

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

### For Mac OS X (Homebrew)

Thanks to [Edward Samson's](https://github.com/esamson) [pull request](https://github.com/Homebrew/homebrew/pull/23977), you can also use [Homebrew](http://brew.sh/) to install tcptunnel under Mac OS X.

```
$ uname -mrs
Darwin 12.5.0 x86_64

$ brew --version
0.9.5

$ brew update
$ brew info tcptunnel
$ brew install tcptunnel
$ tcptunnel --version
$ tcptunnel --help
$ file $(which tcptunnel)
/usr/local/bin/tcptunnel: Mach-O 64-bit executable x86_64
```

### For Windows (Cygwin)

You will need the Cygwin environment for Windows from http://www.cygwin.com/ with the following additional packages installed:

* gcc
* git
* make

```
$ uname -a
CYGWIN_NT-6.1-WOW64 computer 1.7.25(0.270/5/3) 2013-08-31 20:39 i686 Cygwin

$ git clone git://github.com/vakuum/tcptunnel.git
$ cd tcptunnel
$ ./configure
$ make
$ ./tcptunnel --version
$ ./tcptunnel --help
$ file tcptunnel.exe
tcptunnel.exe: PE32 executable (console) Intel 80386, for MS Windows
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

## ChangeLog

See [ChangeLog](https://raw.github.com/vakuum/tcptunnel/master/ChangeLog).

## License

Copyright (c) 2000-2016 Clemens Fuchslocher, released under the GPL.

