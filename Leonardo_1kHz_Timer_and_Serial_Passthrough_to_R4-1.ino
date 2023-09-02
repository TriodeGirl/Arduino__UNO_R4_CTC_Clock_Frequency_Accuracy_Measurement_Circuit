/* Hardware Serial Passthrough sketch to couple R4 to USB serial, with 1.000kHz timer

 Susan Parker - 10 July 2023
   Hardware Serial Passthrough sketch to couple R4 to USB serial
   
 Susan Parker -  2 September 2023
   Add 1 kHz CTC timer for R4 clock stability testing - Output on Pin D6
   Use Timer 4 as designed for high-speed operation...
   My board measures   999.9863 Hz with some flicker on the least significant digit, with stable phase
   My Sig-Gen measures 999.9987 Hz BUT the output has significant single cycle-period phase-noise
*/

/*
Board Pin Allocation:

                  = D13;   // PC7 ICP3   / OC4A / CLK0 
                  = D12;   // PD6 T1     /#OC4D /      / ADC9
                  = D11;   // PB7 PCINT7 / OC0A / OC1C / #RTS
                  = D10;   // PB6 PCINT6 / OC1B / OC4B / ADC13
                  =  D9;   // PB5 PCINT5 / OC1A /#OC4B / ADC12
                  =  D8;   // PB4 PCINT4 /      /      / ADC11
                  =  D7;   // PE6        /      / INT4 / AIN0
                  =  D6;   // PD7 T0     / OC4D /      / ADC10
                  =  D5;   // PC6        / OC3A /#0C4A
                  =  D4;   // PD4 ICP1   /      / ADC8
                  =  D3;   // PD0 SCL    / OC0B / INT0
                  =  D2;   // PD1 SDA    /      / INT1
  Serial Tx (R4)  =  D1;   // PD3 TXD1   /      / INT3
  Serial Rx (R4)  =  D0;   // PD2 RXD1   /      / INT2 / AIN1

                  =  A5;   // PF0
                  =  A4;   // PF1
                  =  A3;   // PF4
                  =  A2;   // PF5
                  =  A1;   // PF6
                  =  A0;   // PF7


Leonardo External Interrupts: 
  32u4 3 (Interrupt 0), 
  32u4 2 (Interrupt 1), 
  32u4 0 (Interrupt 2), 
  32u4 1 (Interrupt 3) and 
  32u4 7 (Interrupt 4). 
  
  See attachInterrupt() function for details.
  https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

*/
 
#include <stdlib.h>
#include <Arduino.h>
#include "avr/pgmspace.h"

// Direct register bit set/clear macros:
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

// #define TIMER4_DIAG         // Enable to print out Timer 4 register setups
// #define TIMER4_ALL_OUTPUTS  // Enable to have all 3 phase outputs

void setup()
  {
  Serial.begin(115200);
  Serial1.begin(115200);
  while(!Serial);
  
  Serial.println(F(" "));
  Serial.println(F("Serial Link + 1kHz Timing Clock Source"));

  delay(100);
 
#ifdef TIMER4_ALL_OUTPUTS
  pinMode(13, OUTPUT);  //  OC4A - This output works without pinMode
  digitalWrite(13, LOW);
  pinMode(10, OUTPUT);  //  OC4B - This output does NOT work without pinMode
  digitalWrite(10, LOW);
#endif
  pinMode( 6, OUTPUT);  //  OC4D - This output does NOT work without pinMode
  digitalWrite( 6, LOW);

  setup_timer4();
  }

void loop()
  {
  if(Serial.available())            // If anything comes in Serial (USB),
    {
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
    }
  if (Serial1.available())          // If anything comes in Serial1 (pins 0 & 1)
    {
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
    }
  }


void setup_timer4() // Timer 4 on Pins D13 and D12
  {
  // Timer4 Clock Prescaler to : 1 - select the clock source to be used by the Timer/Counter
  // Table 15-15. Timer/Counter4 Prescaler Select
  /*
  CS43 CS42 CS41 CS40 Asynchronous Clocking Mode Synchronous Clocking Mode
  0 0 0 0 T/C4 stopped T/C4 stopped (provides a Timer Enable/Disable function)
  0 0 0 1 PCK CK
  0 0 1 0 PCK/2 CK/2
  0 0 1 1 PCK/4 CK/4
  0 1 0 0 PCK/8 CK/8
  0 1 0 1 PCK/16 CK/16
  0 1 1 0 PCK/32 CK/32
  0 1 1 1 PCK/64 CK/64
  1 0 0 0 PCK/128 CK/128
  1 0 0 1 PCK/256 CK/256
  1 0 1 0 PCK/512 CK/512
  1 0 1 1 PCK/1024 CK/1024
  1 1 0 0 PCK/2048 CK/2048
  1 1 0 1 PCK/4096 CK/4096
  1 1 1 0 PCK/8192 CK/8192
  1 1 1 1 PCK/16384 CK/16384
  */
  sbi (TCCR4B, CS40);
  cbi (TCCR4B, CS41);
  sbi (TCCR4B, CS42);
  cbi (TCCR4B, CS43);

  cbi (TCCR4D, WGM40);  // Mode 0  / Normal CTC mode
  cbi (TCCR4D, WGM41);

#ifdef TIMER4_ALL_OUTPUTS
  sbi (TCCR4A, PWM4A);  // Bit 1 - PWM4A: Pulse Width Modulator A Enable
  sbi (TCCR4A, PWM4B);  // Bit 0 - PWM4B: Pulse Width Modulator B Enable
#endif
  sbi (TCCR4C, PWM4D);  // Bit 0 - PWM4D: Pulse Width Modulator D Enable

  // Table 15-5. Compare Output Mode (Normal Mode (non-PWM))
  // Note Toggle is at Compare Match between TCNT4 and OCR4x, gets cleared at TCNT4 reset 

#ifdef TIMER4_ALL_OUTPUTS
  // OCW4A - COM4A1, COM4A0: Comparator A Output Mode, Bits 1 and 0 - OC4A on pin D13
  sbi (TCCR4A, COM4A0);  // 01b = Toggle on Compare Match
  cbi (TCCR4A, COM4A1);  // 

  // OCW4B - COM4B1, COM4B0: Comparator B Output Mode, Bits 1 and 0 - OC4B on pin D10
  sbi (TCCR4A, COM4B0);  // 01b = Toggle on Compare Match
  cbi (TCCR4A, COM4B1);  // 
#endif
  // OCW4D - COM4D1, COM4D0: Comparator D Output Mode, Bits 1 and 0 - OC4D on pin D6
  sbi (TCCR4C, COM4D0);  // 01b = Toggle on Compare Match
  cbi (TCCR4C, COM4D1);  // 

  TC4H  = 0x01;
#ifdef TIMER4_ALL_OUTPUTS
  OCR4A = 0xFF - 12;
  OCR4B = 0xFF - 12;
#endif
  OCR4D = 0xFF - 12;
  TC4H  = 0x03;        // High bits
  OCR4C = 0xFF - 24;   // 0x3FF = 1024
#ifdef TIMER4_DIAG
  Serial.print("TCCR4A = ");
  Serial.println(TCCR4A, BIN);
  Serial.write("TCCR4B = ");
  Serial.println(TCCR4B, BIN);
  Serial.write("TCCR4C = ");
  Serial.println(TCCR4C, BIN);
  Serial.write("TCCR4D = ");
  Serial.println(TCCR4D, BIN);
#endif
  }

ISR(TIMER4_OVF_vect)
  {
  OCR4A = 0x7F;
  OCR4D = 0x7F;
  }

/*
//******************************************************************
// timer3 setup - ATmega32u4 Leonardo
// set prescaler to 1, CTC mode, 16000000/16000 = 1000 Hz clock

void setup_timer3()
  {
  // Timer3 Clock Prescaler to : 1 - select the clock source to be used by the Timer/Counter
  // Table 15-15. Timer/Counter4 Prescaler Select
*/
  /*
  CS32 CS31 CS30 Asynchronous Clocking Mode Synchronous Clocking Mode
  0 0 0  stopped (provides a Timer Enable/Disable function)
  0 0 1 PCK CK
  0 1 0 PCK/2 CK/8
  0 1 1 PCK/4 CK/64
  1 0 0 PCK/8 CK/256
  1 0 1 PCK/16 CK/1024
  1 1 0 External clock source on Tn pin. Clock on falling edge
  1 1 1 External clock source on Tn pin. Clock on rising edge
*/
/*
  sbi (TCCR3B, CS30);   
  cbi (TCCR3B, CS31);
  cbi (TCCR3B, CS32);

  // Timer3 PWM Mode set to CTC
  cbi (TCCR3A, WGM30);  // Mode 12 
  cbi (TCCR3A, WGM31);  //
  sbi (TCCR3B, WGM32);  // 
  sbi (TCCR3B, WGM33);  // 
  
  // COM3A1, COM3A0: Comparator A Output Mode, Bits 1 and 0
  sbi (TCCR3A, COM3A0);  // Toggle
  cbi (TCCR3A, COM3A1);  // 

  OCR3AH = TIME_MS_MSB_VAL;
  OCR3AL = TIME_MS_LSB_VAL;
  }
*/
