# DSO203_SYS164_GCC

Port to GCC of SYS ver 1.64 of DSO203.

base on the initial work of https://github.com/neilstockbridge/dsoquad-BIOS

tested only on DSO203 v2.72 8Mb and v2.82.

What is not working:

/_- cannot be patched by alterbios _/ FIXED

- has the same bug in 8Mb handling of the stock SYS, but You can always patch it by Alterbios.

Files created without Alterbios are not readable once Alterbios is activated, in case You remove Alterbios most of the files created when Alterbios was active won't be readable.

Todo:

- implementing alterbios patches in the code -> Use the insertAlterbios branch or download SYSGCCA.zip in the relases

- check and assign a license.

Compiled by arm-none-eabi-gcc 9.2.1 system arm compiler available in ubuntu 21.04
It seems only this version of gcc is able to produce a running binary (even on windows), I'll investigate later.

to compile do a
make clean all

this produces a SYSGCC.HEX and SYSGCC.BIN which can be uploaded through dfu as any other SYS file.

Binaries available in releases:
SYSGCC is the SYS_B164 ported and compiled by gcc
SYSGCCA is the Alterbios patched version.

Both have been tested on DSO203 HW2.72 and HW2.82
