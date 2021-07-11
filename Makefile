
NAME = SYSGCC

OUTDIR=build/
#PARTS += core_cm3.o
#PARTS += misc.o stm32f10x_dma.o stm32f10x_gpio.o stm32f10x_rcc.o stm32f10x_tim.o stm32f10x_adc.o stm32f10x_usart.o stm32f10x_dac.o stm32f10x_flash.o stm32f10x_fsmc.o stm32f10x_spi.o
#PARTS += USB_core.o USB_init.o USB_int.o USB_mem.o USB_regs.o
## Left out: Authenticate.o 2FAT12.o
#PARTS += ASM.o Config.o FAT12.o LCD.o Ident.o Memory.o USB_bot.o USB_endp.o \
#  USB_prop.o USB_scsi.o BIOS.o Ext_Flash.o Function.o Interrupt.o Main.o \
#  scsi_data.o USB_desc.o USB_istr.o USB_pwr.o serial.o
#PARTS += startup.o

PARTS = Ident.o stm32f10x_nvic.o	USB_desc.o BIOS.o  \
	USB_endp.o Config.o usb_init.o Ext_Flash.o usb_int.o stm32f10x_lib.o	usb_core.o\
	FAT12.o USB_istr.o Function.o startup.o stm32f10x_dma.o usb_mem.o \
	stm32f10x_spi.o USB_prop.o Interrupt.o stm32f10x_flash.o USB_pwr.o \
	LCD.o stm32f10x_fsmc.o USB_regs.o Main.o  stm32f10x_rcc.o stm32f10x_gpio.o USB_scsi.o \
	Memory.o scsi_data.o USB_bot.o ASM.o cortexm3_macro.o 


DELIVERABLES = $(NAME).HEX $(NAME)A.BIN A$(NAME).HEX A$(NAME)A.BIN

CFLAGS += -Iinc -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER  -Iinc/usbinc -Iinc/stm32inc

# Processor type
CFLAGS += -mcpu=cortex-m3 -mthumb -mno-thumb-interwork

# Optimization & debug settings
CFLAGS += -fno-common -Os -std=gnu99 -ffunction-sections

# Compiler warnings
CFLAGS += -Wall -Wno-unused -Wno-error

# Default linker arguments (disables GCC-provided startup.c, creates .map file)
LFLAGS += -mthumb -march=armv7 -mfix-cortex-m3-ldrd -nostartfiles -Wl,-Map=$(NAME).map -eReset_Handler -Wl,-gc-sections

AFLAGS = -mcpu=cortex-m3


# Names of the toolchain programs
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# Tell make where to find transitional files:
VPATH = src:src/stm32src:src/usbsrc

.PHONY: default
default: all ;

# How to make .HEX files from .elf files:
$(NAME).HEX: $(NAME).elf
	$(OBJCOPY) -O ihex $< $@

$(NAME)A.BIN: $(NAME).elf
	$(OBJCOPY) -O binary $< $@
	echo 0x08004000 > $(NAME)A.ADR 

A$(NAME).HEX: $(NAME).HEX  $(NAME)A.BIN
	make -C alterbios

$(NAME).elf: SYS.lds $(PARTS)
	$(CC) $(CFLAGS) -o $@ $(PARTS) $(LFLAGS) $(LIBS) -T $<

# Rebuild all parts if any header or the Makefile changes:
.c.o: *.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	$(CC) $(AFLAGS) -c -o $@ $<

all: $(DELIVERABLES) $(NAME).elf
	$(SIZE) $(NAME).elf

clean:
	make -C alterbios clean
	rm -f $(DELIVERABLES) $(NAME).elf $(NAME).map $(PARTS) $(NAME)A.ADR
	
loadalter:	A$(NAME).HEX
	sudo ./dfuload.sh A$(NAME)

load:	$(NAME).HEX $(NAME)A.BIN
	sudo ./dfuload.sh $(NAME)