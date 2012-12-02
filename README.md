# tcptunnel

Tcptunnel is a simple TCP port forwarder. Please see http://www.vakuumverpackt.de/tcptunnel/ for full details.

## Building

### For Unix

	$ git clone https://github.com/vakuum/tcptunnel.git
	$ cd tcptunnel
	$ ./configure
	$ make
	$ ./tcptunnel --version
	$ ./tcptunnel --help

### For Windows

You will need MinGW32 to cross-compile tcptunnel. Please see http://www.mingw.org/ for more details. If you are using a Debian-based distribution then you will need to install the following packages:

* mingw32
* mingw32-binutils
* mingw32-runtime

	$ apt-get install mingw32 mingw32-binutils mingw32-runtime
	$ git clone https://github.com/vakuum/tcptunnel.git
	$ cd tcptunnel
	$ ./configure
	$ make -f Makefile.MinGW32
	$ file tcptunnel
	tcptunnel.exe: PE32 executable (console) Intel 80386, for MS Windows

## Help

	$ tcptunnel --help
	Usage: tcptunnel [options]

	Options:
	  --version
	  --help

	  --local-port=PORT    port to redirect
	  --remote-port=PORT   target port
	  --remote-host=HOST   target host
	  --bind-address=IP    bind IP address
	  --fork               fork-based concurrency
	  --log-to-stdout
	  --stay-alive

## ChangeLog

* [ChangeLog](https://raw.github.com/vakuum/tcptunnel/master/ChangeLog)

## License

Copyright (c) 2000-2012 Clemens Fuchslocher, released under the GPL.

