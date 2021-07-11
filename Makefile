
NAME = SYSGCC

OUTDIR=build
#PARTS += core_cm3.o
#PARTS += misc.o stm32f10x_dma.o stm32f10x_gpio.o stm32f10x_rcc.o stm32f10x_tim.o stm32f10x_adc.o stm32f10x_usart.o stm32f10x_dac.o stm32f10x_flash.o stm32f10x_fsmc.o stm32f10x_spi.o
#PARTS += USB_core.o USB_init.o USB_int.o USB_mem.o USB_regs.o
## Left out: Authenticate.o 2FAT12.o
#PARTS += ASM.o Config.o FAT12.o LCD.o Ident.o Memory.o USB_bot.o USB_endp.o \
#  USB_prop.o USB_scsi.o BIOS.o Ext_Flash.o Function.o Interrupt.o Main.o \
#  scsi_data.o USB_desc.o USB_istr.o USB_pwr.o serial.o
#PARTS += startup.o

CPARTS = Main.o Ident.o  BIOS.o Config.o Ext_Flash.o Interrupt.o 	\
	FAT12.o  Function.o startup.o LCD.o  \
	stm32f10x_lib.o stm32f10x_spi.o stm32f10x_flash.o  \
	stm32f10x_nvic.o stm32f10x_dma.o stm32f10x_fsmc.o stm32f10x_rcc.o stm32f10x_gpio.o  \
	Memory.o USB_scsi.o usb_int.o USB_pwr.o usb_init.o USB_endp.o USB_istr.o \
	usb_core.o USB_prop.o usb_mem.o scsi_data.o USB_regs.o USB_desc.o USB_bot.o 

SPARTS = ASM.o cortexm3_macro.o

BASENAME = $(basename $(CPARTS))
CSRCS = $(addsuffix .c , $(BASENAME))
COBJS = $(addprefix $(OUTDIR)/, $(CPARTS) )

BASENAME = $(basename $(SPARTS))
SSRCS = $(addsuffix .s , $(BASENAME))
SOBJS = $(addprefix $(OUTDIR)/, $(SPARTS) )



DELIVERABLES = $(NAME).HEX $(NAME)A.BIN

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

# How to make .HEX files from .elf files:
$(NAME).HEX:  $(NAME).elf 
	$(OBJCOPY) -O ihex $< $@

.PHONY: default
default: all ;


build:	
	mkdir build

$(NAME)A.BIN: $(NAME).elf
	echo 0x08004000 > $(NAME)A.ADR
	$(OBJCOPY) -O binary $< $@

$(NAME).elf: SYS.lds $(COBJS) $(SOBJS)
	$(CC) $(CFLAGS) -o $@ $(COBJS) $(SOBJS) $(LFLAGS) $(LIBS) -T $<

# Rebuild all parts if any header or the Makefile changes:
build/%.o: %.c build
	$(CC) $(CFLAGS) -c -o $(@) $<

build/%.o: %.s build
	$(CC) $(AFLAGS) -c -o $(@) $<

all: $(DELIVERABLES) $(NAME).elf
	$(SIZE) $(NAME).elf

clean:
	rm -fR build $(NAME)* 

