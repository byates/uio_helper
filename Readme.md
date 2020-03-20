## uio_helper - light-wight library to access UIO devices

libuio is a light-wight C programming library to handle UIO device
discovery and binding task. The library is based on the UIO example
code lsuio v0.2.0 written by Hans J. Koch <hjk@linutronix.de>. Hans
J. Koch maintains UIO in the Linux kernel.

## buildroot package files

The files `external.mk` and `Config.In` are not directly used by the
makefile but are in the repository as references for the package
files needed for buildroot. These files should be placed in the
`package` directory in your buildroot tree.

## References

This repository is a copy of Stephan Linz version but setup to build
within the buildroot environment as a cross compile for ARM.

* Copyright (C) 2007 Hans J. Koch
* Copyright (C) 2010 Stephan Linz


