# DSO203_SYS164_GCC

Port to GCC of SYS ver 1.64 of DSO203.
tested only on DSO203 v2.72 8Mb.

What is not working:

- has the same bug in 8Mb handling on 2.72 hw of the stock SYS, but You can always patch it by Alterbios.

Files created without Alterbios are not readable once Alterbios is activated, in case You remove Alterbios most of the files created when Alterbios was active won't be readable.

Todo:
- implementing alterbios patches in the code
- check and assign a license.
- write credits at the end of the readme.

Compiled by arm-none-eabi-gcc 9.2.1 system arm compiler available in ubuntu 21.04

to compile do a 
make clean all

this produces a SYSGCC.HEX and SYSGCC.BIN which can be uploaded through dfu as any other SYS file.
