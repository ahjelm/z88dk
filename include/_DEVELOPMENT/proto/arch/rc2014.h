include(__link__.m4)

#ifndef __ARCH_RC2014_H__
#define __ARCH_RC2014_H__

#include <arch.h>

// GLOBAL VARIABLES


// IO MAPPED REGISTERS

#ifdef __CLANG

extern unsigned char io_dio;

extern unsigned char io_pio_port_a;
extern unsigned char io_pio_port_b;
extern unsigned char io_pio_port_b;
extern unsigned char io_pio_control;

extern unsigned char io_pio_ide_lsb;
extern unsigned char io_pio_ide_msb;
extern unsigned char io_pio_ide_ctl;
extern unsigned char io_pio_ide_config;

extern unsigned char io_prom_reset;
extern unsigned char io_prom_toggle;

#else

__sfr __at __IO_DIO_PORT        io_dio;

__sfr __at __IO_PIO_PORT_A      io_pio_port_a;
__sfr __at __IO_PIO_PORT_B      io_pio_port_b;
__sfr __at __IO_PIO_PORT_C      io_pio_port_c;
__sfr __at __IO_PIO_CONTROL     io_pio_control;

__sfr __at __IO_PIO_IDE_LSB     io_pio_ide_lsb;
__sfr __at __IO_PIO_IDE_MSB     io_pio_ide_msb;
__sfr __at __IO_PIO_IDE_CTL     io_pio_ide_ctl;
__sfr __at __IO_PIO_IDE_CONFIG  io_pio_ide_config;

__sfr __at __IO_PROM_RESET      io_prom_reset;
__sfr __at __IO_PROM_TOGGLE     io_prom_toggle;

#endif

#endif

// SYSTEM FUNCTIONS


