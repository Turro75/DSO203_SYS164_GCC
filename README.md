# DSO203_SYS164_GCC

Port to GCC of SYS ver 1.64 of DSO203.
tested only on DSO203 v2.72 8Mb.

this version GCCA0 includes Alterbios as part of SYS.
To use quadpawn with this release You need the latest quadpawn built for this new memory mapping.

Todo:

- check and assign a license.
- write credits at the end of the readme.

Compiled by arm-none-eabi-gcc 9.2.1 system arm compiler available in ubuntu 21.04

to compile do a
make clean all

this produces a SYSGCC.HEX and SYSGCCA.BIN which can be uploaded through dfu as any other SYS file.
