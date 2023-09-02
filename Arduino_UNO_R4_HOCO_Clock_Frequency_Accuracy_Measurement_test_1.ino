/*  Arduino UNO R4 code:
 *    A general method to attach an interrupt to ANY event, with Interrupt Priority changes.
 *    CAC - Clock Frequency Accuracy Measurement Circuit
 *
 *  Attach CAC External 1kHz timing-clock to Heart pad on underside of R4 Minima board
 *
 *  NOTE: Pin allocation is as per R4 Minima, the R4 WiFi has many board-pins with different connections.
 *
 *  Susan Parker - 2nd September 2023.
 *
 *
 * This code is "AS IS" without warranty or liability. 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/


#include "Arduino.h"

/* The Folowing #defines for registers would normally go into a seperate .H file */

// ARM-developer - Accessing memory-mapped peripherals
// https://developer.arm.com/documentation/102618/0100

#define ICUBASE 0x40000000 // ICU Base - See 13.2.6 page 233
// 32 bits - 
#define IELSR 0x6300 // ICU Event Link Setting Register n
#define ICU_IELSR00 ((volatile unsigned int *)(ICUBASE + IELSR))            //
#define ICU_IELSR01 ((volatile unsigned int *)(ICUBASE + IELSR + ( 1 * 4))) // 
#define ICU_IELSR02 ((volatile unsigned int *)(ICUBASE + IELSR + ( 2 * 4))) // 
#define ICU_IELSR03 ((volatile unsigned int *)(ICUBASE + IELSR + ( 3 * 4))) // 
#define ICU_IELSR04 ((volatile unsigned int *)(ICUBASE + IELSR + ( 4 * 4))) // 
#define ICU_IELSR05 ((volatile unsigned int *)(ICUBASE + IELSR + ( 5 * 4))) // 
#define ICU_IELSR06 ((volatile unsigned int *)(ICUBASE + IELSR + ( 6 * 4))) // 
#define ICU_IELSR07 ((volatile unsigned int *)(ICUBASE + IELSR + ( 7 * 4))) // 
#define ICU_IELSR08 ((volatile unsigned int *)(ICUBASE + IELSR + ( 8 * 4))) // 
#define ICU_IELSR09 ((volatile unsigned int *)(ICUBASE + IELSR + ( 9 * 4))) // 
#define ICU_IELSR10 ((volatile unsigned int *)(ICUBASE + IELSR + (10 * 4))) // 
#define ICU_IELSR11 ((volatile unsigned int *)(ICUBASE + IELSR + (11 * 4))) // 
#define ICU_IELSR12 ((volatile unsigned int *)(ICUBASE + IELSR + (12 * 4))) // 
#define ICU_IELSR13 ((volatile unsigned int *)(ICUBASE + IELSR + (13 * 4))) // 
#define ICU_IELSR14 ((volatile unsigned int *)(ICUBASE + IELSR + (14 * 4))) // 
#define ICU_IELSR15 ((volatile unsigned int *)(ICUBASE + IELSR + (15 * 4))) // 
#define ICU_IELSR16 ((volatile unsigned int *)(ICUBASE + IELSR + (16 * 4))) // 
#define ICU_IELSR17 ((volatile unsigned int *)(ICUBASE + IELSR + (17 * 4))) // 
#define ICU_IELSR18 ((volatile unsigned int *)(ICUBASE + IELSR + (18 * 4))) // 
#define ICU_IELSR19 ((volatile unsigned int *)(ICUBASE + IELSR + (19 * 4))) // 
#define ICU_IELSR20 ((volatile unsigned int *)(ICUBASE + IELSR + (20 * 4))) // 
#define ICU_IELSR21 ((volatile unsigned int *)(ICUBASE + IELSR + (21 * 4))) // 
#define ICU_IELSR22 ((volatile unsigned int *)(ICUBASE + IELSR + (22 * 4))) // 
#define ICU_IELSR23 ((volatile unsigned int *)(ICUBASE + IELSR + (23 * 4))) // 
#define ICU_IELSR24 ((volatile unsigned int *)(ICUBASE + IELSR + (24 * 4))) // 
#define ICU_IELSR25 ((volatile unsigned int *)(ICUBASE + IELSR + (25 * 4))) // 
#define ICU_IELSR26 ((volatile unsigned int *)(ICUBASE + IELSR + (26 * 4))) // 
#define ICU_IELSR27 ((volatile unsigned int *)(ICUBASE + IELSR + (27 * 4))) // 
#define ICU_IELSR28 ((volatile unsigned int *)(ICUBASE + IELSR + (28 * 4))) // 
#define ICU_IELSR29 ((volatile unsigned int *)(ICUBASE + IELSR + (29 * 4))) // 
#define ICU_IELSR30 ((volatile unsigned int *)(ICUBASE + IELSR + (30 * 4))) // 
#define ICU_IELSR31 ((volatile unsigned int *)(ICUBASE + IELSR + (31 * 4))) // 

// IRQ Event Numbers
#define IRQ_NO_EVENT         0x00
#define IRQ_PORT_IRQ0        0x01
#define IRQ_PORT_IRQ1        0x02
#define IRQ_PORT_IRQ2        0x03
#define IRQ_PORT_IRQ3        0x04
#define IRQ_PORT_IRQ4        0x05
#define IRQ_PORT_IRQ5        0x06
#define IRQ_PORT_IRQ6        0x07
#define IRQ_PORT_IRQ7        0x08
#define IRQ_PORT_IRQ8        0x09
#define IRQ_PORT_IRQ9        0x0A
#define IRQ_PORT_IRQ10       0x0B
#define IRQ_PORT_IRQ11       0x0C
#define IRQ_PORT_IRQ12       0x0D
//      IRQ_PORT_UNUSED      0x0E
#define IRQ_PORT_IRQ14       0x0F
#define IRQ_PORT_IRQ15       0x10
#define IRQ_DMAC0_INT        0x11
#define IRQ_DMAC1_INT        0x12
#define IRQ_DMAC2_INT        0x13
#define IRQ_DMAC3_INT        0x14
#define IRQ_DTC_COMPLETE     0x15
//      IRQ_UNUSED           0x16
#define IRQ_ICU_SNZCANCEL    0x17
#define IRQ_FCU_FRDYI        0x18
#define IRQ_LVD_LVD1         0x19
#define IRQ_LVD_LVD2         0x1A
#define IRQ_VBATT_LVD        0x1B
#define IRQ_MOSC_STOP        0x1C
#define IRQ_SYSTEM_SNZREQ    0x1D
#define IRQ_AGT0_AGTI        0x1E
#define IRQ_AGT0_AGTCMAI     0x1F
#define IRQ_AGT0_AGTCMBI     0x20
#define IRQ_AGT1_AGTI        0x21
#define IRQ_AGT1_AGTCMAI     0x22
#define IRQ_AGT1_AGTCMBI     0x23
#define IRQ_IWDT_NMIUNDF     0x24
#define IRQ_WDT_NMIUNDF      0x25
#define IRQ_RTC_ALM          0x26
#define IRQ_RTC_PRD          0x27
#define IRQ_RTC_CUP          0x28
#define IRQ_ADC140_ADI       0x29
#define IRQ_ADC140_GBADI     0x2A
#define IRQ_ADC140_CMPAI     0x2B
#define IRQ_ADC140_CMPBI     0x2C
#define IRQ_ADC140_WCMPM     0x2D
#define IRQ_ADC140_WCMPUM    0x2E
#define IRQ_ACMP_LP0         0x2F
#define IRQ_ACMP_LP1         0x30
#define IRQ_USBFS_D0FIFO     0x31
#define IRQ_USBFS_D1FIFO     0x32
#define IRQ_USBFS_USBI       0x33
#define IRQ_USBFS_USBR       0x34
#define IRQ_IIC0_RXI         0x35
#define IRQ_IIC0_TXI         0x36
#define IRQ_IIC0_TEI         0x37
#define IRQ_IIC0_EEI         0x38
#define IRQ_IIC0_WUI         0x39
#define IRQ_IIC1_RXI         0x3A
#define IRQ_IIC1_TXI         0x3B
#define IRQ_IIC1_TEI         0x3C
#define IRQ_IIC1_EEI         0x3D
#define IRQ_SSIE0_SSITXI     0x3E
#define IRQ_SSIE0_SSIRXI     0x3F
//      IRQ_UNUSED           0x40
#define IRQ_SSIE0_SSIF       0x41
#define IRQ_CTSU_CTSUWR      0x42
#define IRQ_CTSU_CTSURD      0x43
#define IRQ_CTSU_CTSUFN      0x44
#define IRQ_KEY_INTKR        0x45
#define IRQ_DOC_DOPCI        0x46
#define IRQ_CAC_FERRI        0x47
#define IRQ_CAC_MENDI        0x48
#define IRQ_CAC_OVFI         0x49
#define IRQ_CAN0_ERS         0x4A
#define IRQ_CAN0_RXF         0x4B
#define IRQ_CAN0_TXF         0x4C
#define IRQ_CAN0_RXM         0x4D
#define IRQ_CAN0_TXM         0x4E
#define IRQ_IOPORT_GROUP1    0x4F
#define IRQ_IOPORT_GROUP2    0x50
#define IRQ_IOPORT_GROUP3    0x51
#define IRQ_IOPORT_GROUP4    0x52
#define IRQ_ELC_SWEVT0       0x53
#define IRQ_ELC_SWEVT1       0x54
#define IRQ_POEG_GROUP0      0x55
#define IRQ_POEG_GROUP1      0x56
#define IRQ_GPT0_CCMPA       0x57
#define IRQ_GPT0_CCMPB       0x58
#define IRQ_GPT0_CMPC        0x59
#define IRQ_GPT0_CMPD        0x5A
#define IRQ_GPT0_CMPE        0x5B
#define IRQ_GPT0_CMPF        0x5C
#define IRQ_GPT0_OVF         0x5D
#define IRQ_GPT0_UDF         0x5E
#define IRQ_GPT1_CCMPA       0x5F
#define IRQ_GPT1_CCMPB       0x60
#define IRQ_GPT1_CMPC        0x61
#define IRQ_GPT1_CMPD        0x62
#define IRQ_GPT1_CMPE        0x63
#define IRQ_GPT1_CMPF        0x64
#define IRQ_GPT1_OVF         0x65
#define IRQ_GPT1_UDF         0x66
#define IRQ_GPT2_CCMPA       0x67
#define IRQ_GPT2_CCMPB       0x68
#define IRQ_GPT2_CMPC        0x69
#define IRQ_GPT2_CMPD        0x6A
#define IRQ_GPT2_CMPE        0x6B
#define IRQ_GPT2_CMPF        0x6C
#define IRQ_GPT2_OVF         0x6D
#define IRQ_GPT2_UDF         0x6E
#define IRQ_GPT3_CCMPA       0x6F
#define IRQ_GPT3_CCMPB       0x70
#define IRQ_GPT3_CMPC        0x71
#define IRQ_GPT3_CMPD        0x72
#define IRQ_GPT3_CMPE        0x73
#define IRQ_GPT3_CMPF        0x74
#define IRQ_GPT3_OVF         0x75
#define IRQ_GPT3_UDF         0x76
#define IRQ_GPT4_CCMPA       0x77
#define IRQ_GPT4_CCMPB       0x78
#define IRQ_GPT4_CMPC        0x79
#define IRQ_GPT4_CMPD        0x7A
#define IRQ_GPT4_CMPE        0x7B
#define IRQ_GPT4_CMPF        0x7C
#define IRQ_GPT4_OVF         0x7D
#define IRQ_GPT4_UDF         0x7E
#define IRQ_GPT5_CCMPA       0x7F
#define IRQ_GPT5_CCMPB       0x80
#define IRQ_GPT5_CMPC        0x81
#define IRQ_GPT5_CMPD        0x82
#define IRQ_GPT5_CMPE        0x83
#define IRQ_GPT5_CMPF        0x84
#define IRQ_GPT5_OVF         0x85
#define IRQ_GPT5_UDF         0x86
#define IRQ_GPT6_CCMPA       0x87
#define IRQ_GPT6_CCMPB       0x88
#define IRQ_GPT6_CMPC        0x89
#define IRQ_GPT6_CMPD        0x8A
#define IRQ_GPT6_CMPE        0x8B
#define IRQ_GPT6_CMPF        0x8C
#define IRQ_GPT6_OVF         0x8D
#define IRQ_GPT6_UDF         0x8E
#define IRQ_GPT7_CCMPA       0x8F
#define IRQ_GPT7_CCMPB       0x90
#define IRQ_GPT7_CMPC        0x91
#define IRQ_GPT7_CMPD        0x92
#define IRQ_GPT7_CMPE        0x93
#define IRQ_GPT7_CMPF        0x94
#define IRQ_GPT7_OVF         0x95
#define IRQ_GPT7_UDF         0x96
#define IRQ_GPT_UVWEDGE      0x97
#define IRQ_SCI0_RXI         0x98
#define IRQ_SCI0_TXI         0x99
#define IRQ_SCI0_TEI         0x9A
#define IRQ_SCI0_ERI         0x9B
#define IRQ_SCI0_AM          0x9C
#define IRQ_SCI0_RXI_OR_ERI  0x9D
#define IRQ_SCI1_RXI         0x9E
#define IRQ_SCI1_TXI         0x9F
#define IRQ_SCI1_TEI         0xA0
#define IRQ_SCI1_ERI         0xA1
#define IRQ_SCI1_AM          0xA2
#define IRQ_SCI2_RXI         0xA3
#define IRQ_SCI2_TXI         0xA4
#define IRQ_SCI2_TEI         0xA5
#define IRQ_SCI2_ERI         0xA6
#define IRQ_SCI2_AM          0xA7
#define IRQ_SCI9_RXI         0xA8
#define IRQ_SCI9_TXI         0xA9
#define IRQ_SCI9_TEI         0xAA
#define IRQ_SCI9_ERI         0xAB
#define IRQ_SCI9_AM          0xAC
#define IRQ_SPI0_SPRI        0xAD
#define IRQ_SPI0_SPTI        0xAE
#define IRQ_SPI0_SPII        0xAF
#define IRQ_SPI0_SPEI        0xB0
#define IRQ_SPI0_SPTEND      0xB1
#define IRQ_SPI1_SPRI        0xB2
#define IRQ_SPI1_SPTI        0xB3
#define IRQ_SPI1_SPII        0xB4
#define IRQ_SPI1_SPEI        0xB5
#define IRQ_SPI1_SPTEND      0xB6

// ==== IRQ Control Register ====
#define IRQCR 0x6000        // IRQ Control Register offset
#define ICU_IRQCR00  ((volatile unsigned char  *)(ICUBASE + IRQCR +  0 ))     // Pin IRQ Control Register 0
#define ICU_IRQCR01  ((volatile unsigned char  *)(ICUBASE + IRQCR +  1 ))     // Pin IRQ Control Register 1
#define ICU_IRQCR02  ((volatile unsigned char  *)(ICUBASE + IRQCR +  2 ))     // Pin IRQ Control Register 2
#define ICU_IRQCR03  ((volatile unsigned char  *)(ICUBASE + IRQCR +  3 ))     // Pin IRQ Control Register 3
#define ICU_IRQCR04  ((volatile unsigned char  *)(ICUBASE + IRQCR +  4 ))     // Pin IRQ Control Register 4
#define ICU_IRQCR05  ((volatile unsigned char  *)(ICUBASE + IRQCR +  5 ))     // Pin IRQ Control Register 5
#define ICU_IRQCR06  ((volatile unsigned char  *)(ICUBASE + IRQCR +  6 ))     // Pin IRQ Control Register 6
#define ICU_IRQCR07  ((volatile unsigned char  *)(ICUBASE + IRQCR +  7 ))     // Pin IRQ Control Register 7
#define ICU_IRQCR08  ((volatile unsigned char  *)(ICUBASE + IRQCR +  8 ))     // Pin IRQ Control Register 8
#define ICU_IRQCR09  ((volatile unsigned char  *)(ICUBASE + IRQCR +  9 ))     // Pin IRQ Control Register 9
#define ICU_IRQCR10  ((volatile unsigned char  *)(ICUBASE + IRQCR + 10 ))     // Pin IRQ Control Register 10
#define ICU_IRQCR11  ((volatile unsigned char  *)(ICUBASE + IRQCR + 11 ))     // Pin IRQ Control Register 11
#define ICU_IRQCR12  ((volatile unsigned char  *)(ICUBASE + IRQCR + 12 ))     // Pin IRQ Control Register 12
#define ICU_IRQCR14  ((volatile unsigned char  *)(ICUBASE + IRQCR + 14 ))     // Pin IRQ Control Register 14
#define ICU_IRQCR15  ((volatile unsigned char  *)(ICUBASE + IRQCR + 15 ))     // Pin IRQ Control Register 15


// ==== NVIC Interrupt Controller ====
#define NVICBASE 0xE0000000 // NVIC Interrupt Controller

// asm volatile("dsb"); // <<<< use a DSB instruction to ensure bus-synchronisition for NVIC write operations

#define NVIC_ICTR    ((volatile unsigned int  *)(NVICBASE + 0xE004))              // Interrupt Controller Type Register
// Total number of interrupt lines in groups of 32: [3:0]	INTLINESNUM
// b0000 = 0...32   << RA4M1 returns 0 - Has only 32 Interrupts - See Table 13.3 Interrupt vector table p.237
// b0001 = 33...64
// b0010 = 65...96
// b0011 = 97...128
// b0100 = 129...160
// b0101 = 161...192
// b0110 = 193...224
// b0111 = 225...256

// NVIC IRQ registers - Generic ARM Cortex-M4F; not all are necessiraly present in the RA4M1
// x = 0..7 for each register type, with 32 bits per register, to support up to 240 IRQs
// * Each bit controls one interrupt, identified by its IRQ# (0..239)
// * Register# x = IRQ# DIV 32
// * Bit n in the register = IRQ# MOD 32

#define NVICISER 0xE100     // Interrupt Set-Enable Registers
#define NVIC_ISER0  ((volatile unsigned int  *)(NVICBASE + NVICISER +(0 * 4)))     // Register 0
#define NVICICER 0xE180     // Interrupt Clear-Enable Registers
#define NVIC_ICER0  ((volatile unsigned int  *)(NVICBASE + NVICICER +(0 * 4)))     // Register 0
#define NVICISPR 0xE200     // Interrupt Set-Pending Registers
#define NVIC_ISPR0  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(0 * 4)))     // Register 0
#define NVICICPR 0xE280     // Interrupt Clear-Pending Registers
#define NVIC_ICPR0  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(0 * 4)))     // Register 0
#define NVICIABR 0xE300     // Interrupt Active Bit Register - Read Only: Read 1 if interrupt in active state
#define NVIC_IABR0  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(0 * 4)))     // Register 0

// NVIC_IPRx (x=0..59) – Interrupt Priority Registers
// * Supports up to 240 interrupts: 0..239 (32 in RA4M1)
// *  8-bit priority field for each interrupts (4-bit field for RA4M1)
// *  4 priority values per register (MSB aligned bits of each byte for prioity i.e. 0x10 thru 0xF0)
// *  0 = highest priority, F = lowest priority (USBs are all at 0xC0, mS Timer on AGT is 0xE0)
// *  Register# x = IRQ# DIV 4
// *  Byte offset within the register = IRQ# MOD 4
// *  Ex. IRQ85:
// *  85/4 = 21 with remainder 1 (register 21, byte offset 1): Write priority<<8 to NVIC_IPR2
// *  85/32 = 2 with remainder 21: write 1<<21 to NVIC_SER2

#define NVICIPR  0xE400     // Interrupt Priority Register
#define NVIC_IPR00  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(0 * 4)))     // Register 0: NVIC_IPR0 TO 3
#define NVIC_IPR04  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(1 * 4)))     // Register 1
#define NVIC_IPR08  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(2 * 4)))     // Register 2
#define NVIC_IPR12  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(3 * 4)))     // Register 3
#define NVIC_IPR16  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(4 * 4)))     // Register 4
#define NVIC_IPR20  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(5 * 4)))     // Register 5
#define NVIC_IPR24  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(6 * 4)))     // Register 6
#define NVIC_IPR28  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(7 * 4)))     // Register 7

// Also byte accessible:
#define NVIC_IPR00_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  0 ))     // USB
#define NVIC_IPR01_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  1 ))     // USB
#define NVIC_IPR02_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  2 ))     // USB
#define NVIC_IPR03_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  3 ))     // USB
#define NVIC_IPR04_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  4 ))     // AGT for millis() etc
#define NVIC_IPR05_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  5 ))     // 
#define NVIC_IPR06_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  6 ))     // 
#define NVIC_IPR07_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  7 ))     // 
#define NVIC_IPR08_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  8 ))     // 
#define NVIC_IPR09_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  9 ))     // 
#define NVIC_IPR10_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 10 ))     // 
#define NVIC_IPR11_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 11 ))     // 
#define NVIC_IPR12_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 12 ))     // 
#define NVIC_IPR13_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 13 ))     // 
#define NVIC_IPR14_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 14 ))     // 
#define NVIC_IPR15_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 15 ))     // 
#define NVIC_IPR16_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 16 ))     // 
#define NVIC_IPR17_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 17 ))     // 
#define NVIC_IPR18_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 18 ))     // 
#define NVIC_IPR19_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 19 ))     // 
#define NVIC_IPR20_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 20 ))     // 
#define NVIC_IPR21_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 21 ))     // 
#define NVIC_IPR22_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 22 ))     // 
#define NVIC_IPR23_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 23 ))     // 
#define NVIC_IPR24_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 24 ))     // 
#define NVIC_IPR25_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 25 ))     // 
#define NVIC_IPR26_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 26 ))     // 
#define NVIC_IPR27_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 27 ))     // 
#define NVIC_IPR28_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 28 ))     // 
#define NVIC_IPR29_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 29 ))     // 
#define NVIC_IPR30_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 30 ))     // 
#define NVIC_IPR31_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 31 ))     // 


// Low Power Mode Control
#define SYSTEM 0x40010000 // System Registers
#define SYSTEM_SBYCR   ((volatile unsigned short *)(SYSTEM + 0xE00C))      // Standby Control Register
#define SYSTEM_MSTPCRA ((volatile unsigned int   *)(SYSTEM + 0xE01C))      // Module Stop Control Register A

#define MSTP 0x40040000 // Module Registers
#define MSTP_MSTPCRB   ((volatile unsigned int   *)(MSTP + 0x7000))      // Module Stop Control Register B
#define MSTPB2   2 // CAN0
#define MSTPB8   8 // IIC1
#define MSTPB9   9 // IIC0
#define MSTPB18 18 // SPI1
#define MSTPB19 19 // SPI0
#define MSTPB22 22 // SCI9
#define MSTPB29 29 // SCI2
#define MSTPB30 30 // SCI1
#define MSTPB31 31 // SCI0

#define MSTP_MSTPCRC   ((volatile unsigned int   *)(MSTP + 0x7004))      // Module Stop Control Register C
#define MSTPC0   0 // CAC   - Clock Frequency Accuracy Measurement Circuit Module
#define MSTPC1   1 // CRC   - Cyclic Redundancy Check Calculator Module
#define MSTPC3   3 // CTSU  - Capacitive Touch Sensing Unit Module
#define MSTPC4   4 // SLCDC - Segment LCD Controller Module
#define MSTPC8   8 // SSIE0 - Serial Sound Interface Enhanced Module
#define MSTPC13 13 // DOC   - Data Operation Circuit Module
#define MSTPC14 14 // ELC   - Event Link Controller Module
#define MSTPC31 31 // SCE5  - SCE5 Module

#define MSTP_MSTPCRD   ((volatile unsigned int   *)(MSTP + 0x7008))      // Module Stop Control Register D
#define MSTPD2   2 // AGT1   - Asynchronous General Purpose Timer 1 Module
#define MSTPD3   3 // AGT0   - Asynchronous General Purpose Timer 0 Module
#define MSTPD5   5 // GPT320 and GPT321 General 32 bit PWM Timer Module
#define MSTPD6   6 // GPT162 to GPT167 General 16 bit PWM Timer Module
#define MSTPD14 14 // POEG   - Port Output Enable for GPT Module Stop
#define MSTPD16 16 // ADC140 - 14-Bit A/D Converter Module
#define MSTPD19 19 // DAC8   -  8-Bit D/A Converter Module
#define MSTPD20 20 // DAC12  - 12-Bit D/A Converter Module
#define MSTPD29 29 // ACMPLP - Low-Power Analog Comparator Module
#define MSTPD31 31 // OPAMP  - Operational Amplifier Module

// These bits are read as 1, the write value should be 1.
// Bit value 0: Cancel the module-stop state 
// Bit value 1: Enter the module-stop state.
//
// Example:   *MSTP_MSTPCRD &= ~(0x01 << MSTPD16));  // Enable ADC140 module

// ==== Clock Frequency Accuracy Measurement Circuit (CAC) ====
#define CACBASE 0x40040000 //
#define CAC_CACR0       ((volatile unsigned char *)(CACBASE + 0x4600))  // CAC Control Register 0
#define CACR0_CFME          0   // Clock Frequency Measurement Enable; 0: Disable, 1: Enable
#define CAC_CACR1       ((volatile unsigned char *)(CACBASE + 0x4601))  // CAC Control Register 1
#define CACR1_CACREFE       0   // CACREF Pin Input Enable; 0: Disable, 1: Enable
#define CACR1_FMCS_2_0      1   // Measurement Target Clock (MTC) Select; 0b010: HOCO clock
#define CACR1_TCSS_1_0      4   // MTC Freq Div Ratio Select; 0b00: None, 0b01: ×1/4 clock, 0b10: ×1/8 clock, 0b11: ×1/32 clock
#define CACR1_EDGES_1_0     6   // Valid Edge Select; 0b00: Rising edge, 0b01: Falling edge, 0b10: Both rising and falling edges
#define CAC_CACR2       ((volatile unsigned char *)(CACBASE + 0x4602))  // CAC Control Register 2
#define CACR2_RPS           0   // Reference Signal Select; 0: CACREF pin input, 1: Internal clock (internally generated signal)
#define CACR2_RSCS_2_0      1   // Measurement Reference Clock (MRC) Select;
#define CACR2_RCDS_1_0      4   // MRC Freq Div Ratio Select; 0b00: ×1/32, 0b01: ×1/128, 0b10: ×1/1024, 0b11: ×1/8192
#define CACR2_DFS_1_0       6   // Digital Filter Select; 0b00: Disable digital filtering, etc.
#define CAC_CAICR       ((volatile unsigned char *)(CACBASE + 0x4603))  // CAC Interrupt Control Register
#define CAICR_FERRIE        0   // Frequency Error Interrupt Request Enable; 1: Enabled
#define CAICR_MENDIE        1   // Measurement End Interrupt Request Enable; 1: Enabled
#define CAICR_OVFIE         2   // Overflow Interrupt Request Enable; 1: Enabled
#define CAICR_FERRFCL       4   // FERRF Clear; When 1 is written to this bit, the FERRF flag is cleared. This bit is read as 0.
#define CAICR_MENDFCL       5   // MENDF Clear;  ditto
#define CAICR_OVFFCL        6   // OVFF Clear;   ditto
#define CAC_CASTR       ((volatile unsigned char *)(CACBASE + 0x4604))  // CAC Status Register
#define CASTR_FERRF         0   // Frequency Error Flag; 0: Clock frequency is within the allowable range, 1: ... frequency error
#define CASTR_MENDF         1   // Measurement End Flag; 0: Measurement is in progress, 1: Measurement ended
#define CASTR_OVFF          2   // Overflow Flag; 0: The counter has not overflowed, 1: The counter overflowed
#define CAC_CAULVR      ((volatile unsigned short *)(CACBASE + 0x4606))  // CAC Upper-Limit Value Setting Register
#define CAC_CALLVR      ((volatile unsigned short *)(CACBASE + 0x4608))  // CAC Lower-Limit Value Setting Register
#define CAC_CACNTBR     ((volatile unsigned short *)(CACBASE + 0x460A))  // CAC Counter Buffer Register


// =========== Ports ============
// 19.2.5 Port mn Pin Function Select Register (PmnPFS/PmnPFS_HA/PmnPFS_BY) (m = 0 to 9; n = 00 to 15)

// 32 bits - Use for setting pin functions to other than default pin I/O

#define P000PFS 0x0800  // Port 0 Pin Function Select Register
#define PFS_P000PFS ((volatile unsigned int *)(PORTBASE + P000PFS))            // A1 / D15 - AN00 - AMP0+ - IRQ6
#define PFS_P001PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 1 * 4))) // A2 / D16 - AN01 - AMP0- - IRQ7
#define PFS_P002PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 2 * 4))) // A3 / D17 - AN03 - AMP0O - IRQ2
#define PFS_P003PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 3 * 4))) // N/C - AN03
#define PFS_P004PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 4 * 4))) // N/C - AN04 - IRQ3
#define PFS_P005PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 5 * 4))) // N/A - AN11 - IRQ10
#define PFS_P006PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 6 * 4))) // N/A - AN12
#define PFS_P007PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 7 * 4))) // N/A - AN13
#define PFS_P008PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 8 * 4))) // N/A - AN14
// #define PFS_P009PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 9 * 4))) // Does not exist
#define PFS_P010PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (10 * 4))) // N/A - AN05
#define PFS_P011PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (11 * 4))) // N/C - AN06 - IRQ15
#define PFS_P012PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (12 * 4))) // TxLED - AN07
#define PFS_P013PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (13 * 4))) // RxLED - AN08
#define PFS_P014PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (14 * 4))) // A0 / D14 - AN09 - DAC0
#define PFS_P015PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (15 * 4))) // N/C - AN10 - IRQ7

#define P100PFS 0x0840  // Port 1 Pin Function Select Register
#define PFS_P100PFS ((volatile unsigned int *)(PORTBASE + P100PFS))            // A5 / D19 - MISOA - SCL1 - IRQ2
#define PFS_P101PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 1 * 4))) // A4 / D18 - MOSIA - SDA1 - IRQ1
#define PFS_P102PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 2 * 4))) // D5 - RSPCKA
#define PFS_P103PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 3 * 4))) // D4 - SSLA0
#define PFS_P104PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 4 * 4))) // D3 - IRQ1
#define PFS_P105PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 5 * 4))) // D2 - IRQ0
#define PFS_P106PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 6 * 4))) // D6
#define PFS_P107PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 7 * 4))) // D7
#define PFS_P108PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 8 * 4))) // SWD p2 SWDIO
#define PFS_P109PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 9 * 4))) // D11 / MOSI
#define PFS_P110PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (10 * 4))) // D12 / MISO - IRQ3
#define PFS_P111PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (11 * 4))) // D13 / SCLK - IRQ4
#define PFS_P112PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (12 * 4))) // D10 / CS
#define PFS_P113PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (13 * 4))) // N/C
#define PFS_P114PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (14 * 4))) // N/A
#define PFS_P115PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (15 * 4))) // N/A

#define P200PFS 0x0880  // Port 2 Pin Function Select Register
#define PFS_P200PFS ((volatile unsigned int *)(PORTBASE + P200PFS))           // NMI
#define PFS_P201PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (1 * 4))) // MD
#define PFS_P202PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (2 * 4))) // N/A
#define PFS_P203PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (3 * 4))) // N/A
#define PFS_P204PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (4 * 4))) // LOVE (Heart Pad on underside of board)
#define PFS_P205PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (5 * 4))) // N/C - IRQ1 - CLKOUT
#define PFS_P206PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (6 * 4))) // N/C - IRQ0
// Pins P212, P213, P214, and P215 are Crystal functions, or N/C

#define PORTBASE 0x40040000 /* Port Base */
#define PFS_P100PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843))   // 8 bits - A5
#define PFS_P101PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 1 * 4))) // A4
#define PFS_P102PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 2 * 4))) // D5
#define PFS_P103PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 3 * 4))) // D4
#define PFS_P104PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 4 * 4))) // D3
#define PFS_P105PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 5 * 4))) // D2
#define PFS_P106PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 6 * 4))) // D6
#define PFS_P107PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 7 * 4))) // D7
#define PFS_P108PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 8 * 4))) // SWDIO
#define PFS_P109PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 9 * 4))) // D11 / MOSI
#define PFS_P110PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (10 * 4))) // D12 / MISO
#define PFS_P111PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (11 * 4))) // D13 / SCLK
#define PFS_P112PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (12 * 4))) // D10 / CS
#define PFS_P300PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3))            // SWCLK (P300)
#define PFS_P301PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (01 * 4))) // D0 / RxD (P301)
#define PFS_P302PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (02 * 4))) // D1 / TxD (P302) 
#define PFS_P303PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (03 * 4))) // D9
#define PFS_P304PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (04 * 4))) // D8

// ==== Local Defines ====

#define TRUE 1
#define FALSE 0

// 48MHz HOCO measurment against an external reference clock
#define WINDOW_SIZE 1024      // Number of readings to measure over - also changes the output rate to print values 
volatile uint16_t cac_count;
volatile bool cacAveTickFlag = FALSE;


void setup()
  {                                                   // Pins for interrupts: 0, 1, 2, 3, 8, 12, 13, (15?), 16, 17, 18 and 19
  attachInterrupt(15, cacMeasurementEndInterrupt, RISING); // This IRQ will be asigned to Slot 05 IELSR05 as 0x002 PORT_IRQ1 - Table 13.4
  *PFS_P000PFS = 0x00000000;                            // Clear A1/D15 ISEL pin assigned Interrupt Enable
  *ICU_IELSR05 = IRQ_CAC_MENDI;                         // Assign Slot 05 IELSR05 for CAC Interrupt
	asm volatile("dsb");                                  // Data bus Synchronization instruction
  *NVIC_IPR05_BY = 0x80;                                // Bounce the priority up from 0xC0 to 0x80
  
  *PFS_P107PFS_BY = 0x04;                            // Set D7 output low - IRQ time flag pin

  Serial.begin(115200);
  while (!Serial){};

  sys_clock_cac_setup();
  sys_clock_cac_en_check();

  print_icu_event_links();
  }
  
void loop()
  {
  if(cacAveTickFlag == true)
    {
    cacAveTickFlag = FALSE;
    sys_clock_cac_val_print();
    }
  }

// A quick implimentation of a running average filter to remove SigGen pulse jitter 
// https://maker.pro/arduino/tutorial/how-to-clean-up-noisy-sensor-data-with-a-moving-average-filter

void cacMeasurementEndInterrupt(void)  // This interrupt gets a double call (for some unknow reason)
  {
  static uint32_t INDEX = 0;
  static uint32_t VALUE = 0;
  static uint32_t SUM = 0;
  static uint32_t READINGS[WINDOW_SIZE];
  static uint32_t AVERAGED = 0;
  static bool cacTickTock = false;

  *PFS_P107PFS_BY = 0x05;            // D7 start of CAC Interrupt
  *CAC_CAICR |= (0x1 << CAICR_MENDFCL);
  if(cacTickTock == false)           // First time interrupt called, process count
    {
    VALUE = (uint32_t)*CAC_CACNTBR;    // Read the next CAC value
    SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
    READINGS[INDEX] = VALUE;           // Add the newest reading to the window
    SUM = SUM + VALUE;                 // Add the newest reading to the sum
    INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size
    cac_count = SUM / WINDOW_SIZE;     // Divide the sum of the window by the window size for the result
    if(INDEX == 0) 
      cacAveTickFlag = true;
    cacTickTock = true;
    }
  else                               // Second time interrupt is called, some 4uS later
    {
    cacTickTock = false;  
    }
  *PFS_P107PFS_BY = 0x04;            // D7 end of CAC Interrupt
  }

void sys_clock_cac_setup(void)
  {
  *MSTP_MSTPCRC &= ~(0x01 << MSTPC0);      // Enable CAC module in Low Power Modes registers
  *CAC_CACR1   |= (0x1 << CACR1_CACREFE);  // CACREF Pin Input Enable
  *CAC_CACR1   |= (0b010 << CACR1_FMCS_2_0); // 010: HOCO clock
  *CAC_CACR2    =  0x00;                   // CACREF Pin Input Select
  *CAC_CAICR   |= (0x1 << CAICR_MENDIE);   // Measurement End Interrupt Request Enable
//  *NVIC_IPR04 |= 0x00800000;               // Raise Interrupt Priority from C to A (where 0 is highest)
  *CAC_CACR0   |= (0x1 << CACR0_CFME);     // Enable CAC module
  *PFS_P204PFS   = (0b01010 << 24);  // Select PSEL[4:0] for GTIOC4B - See Table 19.8
  *PFS_P204PFS  |= (0x1 << 16);      // Port Mode Control - Used as an I/O port for peripheral function
  }

void sys_clock_cac_en_check(void)
  {
  Serial.print("CAC_CACR0  - ");
  Serial.println(*CAC_CACR0, HEX); 
  }

void sys_clock_cac_val_print(void)
  {
  Serial.println(cac_count); 
  }


// Function: print_icu_event_links();
// The following is to determin which interrupts are in use
// RA4M1 Group ICU Event Number Table 13.4 
//
// Slot - Event Number - Name of the Interrupt
// 0 - 33 - USBFS_USBI
// 1 - 34 - USBFS_USBR
// 2 - 31 - USBFS_D0FIFO
// 3 - 32 - USBFS_D1FIFO
// 4 - 1E - AGT0_AGTI
//
// The above 5 entries are always present before the code gets to setup()

// Use PROGMEM structures to place strings into program memory 
// https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

const char string_00[] PROGMEM = "No_Event";
const char string_01[] PROGMEM = "PORT_IRQ0";
const char string_02[] PROGMEM = "PORT_IRQ1";
const char string_03[] PROGMEM = "PORT_IRQ2";
const char string_04[] PROGMEM = "PORT_IRQ3";
const char string_05[] PROGMEM = "PORT_IRQ4";
const char string_06[] PROGMEM = "PORT_IRQ5";
const char string_07[] PROGMEM = "PORT_IRQ6";
const char string_08[] PROGMEM = "PORT_IRQ7";
const char string_09[] PROGMEM = "PORT_IRQ8";
const char string_0A[] PROGMEM = "PORT_IRQ9";
const char string_0B[] PROGMEM = "PORT_IRQ10";
const char string_0C[] PROGMEM = "PORT_IRQ11";
const char string_0D[] PROGMEM = "PORT_IRQ12";
const char string_0E[] PROGMEM = "PORT_UNUSED";
const char string_0F[] PROGMEM = "PORT_IRQ14";
const char string_10[] PROGMEM = "PORT_IRQ15";
const char string_11[] PROGMEM = "DMAC0_INT";
const char string_12[] PROGMEM = "DMAC1_INT";
const char string_13[] PROGMEM = "DMAC2_INT";
const char string_14[] PROGMEM = "DMAC3_INT";
const char string_15[] PROGMEM = "DTC_COMPLETE";
const char string_16[] PROGMEM = "UNUSED";
const char string_17[] PROGMEM = "ICU_SNZCANCEL";
const char string_18[] PROGMEM = "FCU_FRDYI";
const char string_19[] PROGMEM = "LVD_LVD1";
const char string_1A[] PROGMEM = "LVD_LVD2";
const char string_1B[] PROGMEM = "VBATT_LVD";
const char string_1C[] PROGMEM = "MOSC_STOP";
const char string_1D[] PROGMEM = "SYSTEM_SNZREQ";
const char string_1E[] PROGMEM = "AGT0_AGTI";
const char string_1F[] PROGMEM = "AGT0_AGTCMAI";
const char string_20[] PROGMEM = "AGT0_AGTCMBI";
const char string_21[] PROGMEM = "AGT1_AGTI";
const char string_22[] PROGMEM = "AGT1_AGTCMAI";
const char string_23[] PROGMEM = "AGT1_AGTCMBI";
const char string_24[] PROGMEM = "IWDT_NMIUNDF";
const char string_25[] PROGMEM = "WDT_NMIUNDF";
const char string_26[] PROGMEM = "RTC_ALM";
const char string_27[] PROGMEM = "RTC_PRD";
const char string_28[] PROGMEM = "RTC_CUP";
const char string_29[] PROGMEM = "ADC140_ADI";
const char string_2A[] PROGMEM = "ADC140_GBADI";
const char string_2B[] PROGMEM = "ADC140_CMPAI";
const char string_2C[] PROGMEM = "ADC140_CMPBI";
const char string_2D[] PROGMEM = "ADC140_WCMPM";
const char string_2E[] PROGMEM = "ADC140_WCMPUM";
const char string_2F[] PROGMEM = "ACMP_LP0";
const char string_30[] PROGMEM = "ACMP_LP1";
const char string_31[] PROGMEM = "USBFS_D0FIFO";
const char string_32[] PROGMEM = "USBFS_D1FIFO";
const char string_33[] PROGMEM = "USBFS_USBI";
const char string_34[] PROGMEM = "USBFS_USBR";
const char string_35[] PROGMEM = "IIC0_RXI";
const char string_36[] PROGMEM = "IIC0_TXI";
const char string_37[] PROGMEM = "IIC0_TEI";
const char string_38[] PROGMEM = "IIC0_EEI";
const char string_39[] PROGMEM = "IIC0_WUI";
const char string_3A[] PROGMEM = "IIC1_RXI";
const char string_3B[] PROGMEM = "IIC1_TXI";
const char string_3C[] PROGMEM = "IIC1_TEI";
const char string_3D[] PROGMEM = "IIC1_EEI";
const char string_3E[] PROGMEM = "SSIE0_SSITXI";
const char string_3F[] PROGMEM = "SSIE0_SSIRXI";
const char string_40[] PROGMEM = "UNUSED";
const char string_41[] PROGMEM = "SSIE0_SSIF";
const char string_42[] PROGMEM = "CTSU_CTSUWR";
const char string_43[] PROGMEM = "CTSU_CTSURD";
const char string_44[] PROGMEM = "CTSU_CTSUFN";
const char string_45[] PROGMEM = "KEY_INTKR";
const char string_46[] PROGMEM = "DOC_DOPCI";
const char string_47[] PROGMEM = "CAC_FERRI";
const char string_48[] PROGMEM = "CAC_MENDI";
const char string_49[] PROGMEM = "CAC_OVFI";
const char string_4A[] PROGMEM = "CAN0_ERS";
const char string_4B[] PROGMEM = "CAN0_RXF";
const char string_4C[] PROGMEM = "CAN0_TXF";
const char string_4D[] PROGMEM = "CAN0_RXM";
const char string_4E[] PROGMEM = "CAN0_TXM";
const char string_4F[] PROGMEM = "IOPORT_GROUP1";
const char string_50[] PROGMEM = "IOPORT_GROUP2";
const char string_51[] PROGMEM = "IOPORT_GROUP3";
const char string_52[] PROGMEM = "IOPORT_GROUP4";
const char string_53[] PROGMEM = "ELC_SWEVT0";
const char string_54[] PROGMEM = "ELC_SWEVT1";
const char string_55[] PROGMEM = "POEG_GROUP0";
const char string_56[] PROGMEM = "POEG_GROUP1";
const char string_57[] PROGMEM = "GPT0_CCMPA";
const char string_58[] PROGMEM = "GPT0_CCMPB";
const char string_59[] PROGMEM = "GPT0_CMPC";
const char string_5A[] PROGMEM = "GPT0_CMPD";
const char string_5B[] PROGMEM = "GPT0_CMPE";
const char string_5C[] PROGMEM = "GPT0_CMPF";
const char string_5D[] PROGMEM = "GPT0_OVF";
const char string_5E[] PROGMEM = "GPT0_UDF";
const char string_5F[] PROGMEM = "GPT1_CCMPA";
const char string_60[] PROGMEM = "GPT1_CCMPB";
const char string_61[] PROGMEM = "GPT1_CMPC";
const char string_62[] PROGMEM = "GPT1_CMPD";
const char string_63[] PROGMEM = "GPT1_CMPE";
const char string_64[] PROGMEM = "GPT1_CMPF";
const char string_65[] PROGMEM = "GPT1_OVF";
const char string_66[] PROGMEM = "GPT1_UDF";
const char string_67[] PROGMEM = "GPT2_CCMPA";
const char string_68[] PROGMEM = "GPT2_CCMPB";
const char string_69[] PROGMEM = "GPT2_CMPC";
const char string_6A[] PROGMEM = "GPT2_CMPD";
const char string_6B[] PROGMEM = "GPT2_CMPE";
const char string_6C[] PROGMEM = "GPT2_CMPF";
const char string_6D[] PROGMEM = "GPT2_OVF";
const char string_6E[] PROGMEM = "GPT2_UDF";
const char string_6F[] PROGMEM = "GPT3_CCMPA";
const char string_70[] PROGMEM = "GPT3_CCMPB";
const char string_71[] PROGMEM = "GPT3_CMPC";
const char string_72[] PROGMEM = "GPT3_CMPD";
const char string_73[] PROGMEM = "GPT3_CMPE";
const char string_74[] PROGMEM = "GPT3_CMPF";
const char string_75[] PROGMEM = "GPT3_OVF";
const char string_76[] PROGMEM = "GPT3_UDF";
const char string_77[] PROGMEM = "GPT4_CCMPA";
const char string_78[] PROGMEM = "GPT4_CCMPB";
const char string_79[] PROGMEM = "GPT4_CMPC";
const char string_7A[] PROGMEM = "GPT4_CMPD";
const char string_7B[] PROGMEM = "GPT4_CMPE";
const char string_7C[] PROGMEM = "GPT4_CMPF";
const char string_7D[] PROGMEM = "GPT4_OVF";
const char string_7E[] PROGMEM = "GPT4_UDF";
const char string_7F[] PROGMEM = "GPT5_CCMPA";
const char string_80[] PROGMEM = "GPT5_CCMPB";
const char string_81[] PROGMEM = "GPT5_CMPC";
const char string_82[] PROGMEM = "GPT5_CMPD";
const char string_83[] PROGMEM = "GPT5_CMPE";
const char string_84[] PROGMEM = "GPT5_CMPF";
const char string_85[] PROGMEM = "GPT5_OVF";
const char string_86[] PROGMEM = "GPT5_UDF";
const char string_87[] PROGMEM = "GPT6_CCMPA";
const char string_88[] PROGMEM = "GPT6_CCMPB";
const char string_89[] PROGMEM = "GPT6_CMPC";
const char string_8A[] PROGMEM = "GPT6_CMPD";
const char string_8B[] PROGMEM = "GPT6_CMPE";
const char string_8C[] PROGMEM = "GPT6_CMPF";
const char string_8D[] PROGMEM = "GPT6_OVF";
const char string_8E[] PROGMEM = "GPT6_UDF";
const char string_8F[] PROGMEM = "GPT7_CCMPA";
const char string_90[] PROGMEM = "GPT7_CCMPB";
const char string_91[] PROGMEM = "GPT7_CMPC";
const char string_92[] PROGMEM = "GPT7_CMPD";
const char string_93[] PROGMEM = "GPT7_CMPE";
const char string_94[] PROGMEM = "GPT7_CMPF";
const char string_95[] PROGMEM = "GPT7_OVF";
const char string_96[] PROGMEM = "GPT7_UDF";
const char string_97[] PROGMEM = "GPT_UVWEDGE";
const char string_98[] PROGMEM = "SCI0_RXI";
const char string_99[] PROGMEM = "SCI0_TXI";
const char string_9A[] PROGMEM = "SCI0_TEI";
const char string_9B[] PROGMEM = "SCI0_ERI";
const char string_9C[] PROGMEM = "SCI0_AM";
const char string_9D[] PROGMEM = "SCI0_RXI_OR_ERI";
const char string_9E[] PROGMEM = "SCI1_RXI";
const char string_9F[] PROGMEM = "SCI1_TXI";
const char string_A0[] PROGMEM = "SCI1_TEI";
const char string_A1[] PROGMEM = "SCI1_ERI";
const char string_A2[] PROGMEM = "SCI1_AM";
const char string_A3[] PROGMEM = "SCI2_RXI";
const char string_A4[] PROGMEM = "SCI2_TXI";
const char string_A5[] PROGMEM = "SCI2_TEI";
const char string_A6[] PROGMEM = "SCI2_ERI";
const char string_A7[] PROGMEM = "SCI2_AM";
const char string_A8[] PROGMEM = "SCI9_RXI";
const char string_A9[] PROGMEM = "SCI9_TXI";
const char string_AA[] PROGMEM = "SCI9_TEI";
const char string_AB[] PROGMEM = "SCI9_ERI";
const char string_AC[] PROGMEM = "SCI9_AM";
const char string_AD[] PROGMEM = "SPI0_SPRI";
const char string_AE[] PROGMEM = "SPI0_SPTI";
const char string_AF[] PROGMEM = "SPI0_SPII";
const char string_B0[] PROGMEM = "SPI0_SPEI";
const char string_B1[] PROGMEM = "SPI0_SPTEND";
const char string_B2[] PROGMEM = "SPI1_SPRI";
const char string_B3[] PROGMEM = "SPI1_SPTI";
const char string_B4[] PROGMEM = "SPI1_SPII";
const char string_B5[] PROGMEM = "SPI1_SPEI";
const char string_B6[] PROGMEM = "SPI1_SPTEND";
const char string_B7[] PROGMEM = "UNUSED";

const char *const interrupt_string_table[] PROGMEM = {
string_00, string_01, string_02, string_03, string_04, string_05, string_06, string_07,
string_08, string_09, string_0A, string_0B, string_0C, string_0D, string_0E, string_0F,
string_10, string_11, string_12, string_13, string_14, string_15, string_16, string_17,
string_18, string_19, string_1A, string_1B, string_1C, string_1D, string_1E, string_1F,
string_20, string_21, string_22, string_23, string_24, string_25, string_26, string_27,
string_28, string_29, string_2A, string_2B, string_2C, string_2D, string_2E, string_2F,
string_30, string_31, string_32, string_33, string_34, string_35, string_36, string_37,
string_38, string_39, string_3A, string_3B, string_3C, string_3D, string_3E, string_3F,
string_40, string_41, string_42, string_43, string_44, string_45, string_46, string_47,
string_48, string_49, string_4A, string_4B, string_4C, string_4D, string_4E, string_4F,
string_50, string_51, string_52, string_53, string_54, string_55, string_56, string_57,
string_58, string_59, string_5A, string_5B, string_5C, string_5D, string_5E, string_5F,
string_60, string_61, string_62, string_63, string_64, string_65, string_66, string_67,
string_68, string_69, string_6A, string_6B, string_6C, string_6D, string_6E, string_6F,
string_70, string_71, string_72, string_73, string_74, string_75, string_76, string_77,
string_78, string_79, string_7A, string_7B, string_7C, string_7D, string_7E, string_7F,
string_80, string_81, string_82, string_83, string_84, string_85, string_86, string_87,
string_88, string_89, string_8A, string_8B, string_8C, string_8D, string_8E, string_8F,
string_90, string_91, string_92, string_93, string_94, string_95, string_96, string_97,
string_98, string_99, string_9A, string_9B, string_9C, string_9D, string_9E, string_9F,
string_A0, string_A1, string_A2, string_A3, string_A4, string_A5, string_A6, string_A7,
string_A8, string_A9, string_AA, string_AB, string_AC, string_AD, string_AE, string_AF,
string_B0, string_B1, string_B2, string_B3, string_B4, string_B5, string_B6, string_B7
};

char message_buffer[30];  // 

void print_icu_event_links(void)
  {
  unsigned int local_icu_val = 0;
  unsigned char icu_val_index = 0;

  Serial.println("ICU event links");
  for(icu_val_index = 0; icu_val_index < 32; icu_val_index++)
    {
    Serial.print(icu_val_index);  
    Serial.print(" - ");  
    local_icu_val = *((volatile unsigned int *)(ICUBASE + IELSR + (icu_val_index * 4)));            //
    Serial.print(local_icu_val, HEX);
    strcpy_P(message_buffer, (char *)pgm_read_word(&(interrupt_string_table[local_icu_val])));  // 
    Serial.print(" - ");  
    Serial.println(message_buffer);
    if(local_icu_val == 0) break;      // Only print active allocations - these are always contigious from 0
    }
  Serial.println(" ");
  Serial.print("NVIC_ICTR - ");  
  Serial.println(*NVIC_ICTR, HEX); 
  Serial.print("NVIC_IABR0 - ");  
  Serial.println(*NVIC_IABR0, BIN); 
  Serial.println("NVIC event priority level");
  for(icu_val_index = 0; icu_val_index < 32; icu_val_index++)
    {
    Serial.print("NVIC_IPR");  
    Serial.print(icu_val_index);  
    Serial.print(" - ");  
    local_icu_val = *((volatile unsigned char *)(NVICBASE + NVICIPR + (icu_val_index)));            //
    Serial.println(local_icu_val, HEX);
    if(local_icu_val == 0) break;      // Only print active allocations - these are always contigious from 0
    }
  }

