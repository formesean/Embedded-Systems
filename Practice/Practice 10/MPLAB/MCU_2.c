#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>

#include "./I2C.h"

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

unsigned char VAL;
const unsigned char HEX_TO_7SEG[16] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001  // F
};

void interrupt ISR(void)
{
  unsigned char temp;
  CKP = 0;           // hold clock low (SSPCON reg)
  if (WCOL || SSPOV) // check if overflow or data collision (SSPCON reg)
  {
    temp = SSPBUF; // read SSPBUF to clear buffer
    WCOL = 0;      // clear data collision flag
    SSPOV = 0;     // clear overflow flag
    CKP = 1;       // release clock (SSPCON reg)
  }
  /* check operation if “write” or "read"*/
  if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) // write to slave
  {
    temp = SSPBUF; // read SSPBUF to clear buffer
    while (!BF)
      ;           // wait until receive is complete (SSPSTAT reg)
    PORTD = SSPBUF; // write data from master to a variable
    CKP = 1;      // release clock (SSPCON reg)
  }
  // else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) // read from slave
  // {
  //   temp = SSPBUF;  // read SSPBUF to clear buffer
  //   BF = 0;         // clear buffer status bit (SSPSTAT reg)
  //   SSPBUF = PORTA; // send data from PORTA to master
  //   CKP = 1;        // release clock (SSPCON reg)
  //   while (BF)
  //     ; // wait until transmit is complete (SSPSTAT reg)
  // }
  SSPIF = 0; // clear interrupt flag
}

void decimalToHex(unsigned char decimal, unsigned char *highNibble, unsigned char *lowNibble)
{
  *highNibble = decimal / 16; // Divide by 16 to get the high nibble (quotient)
  *lowNibble = decimal % 16;  // Get the low nibble (remainder)
}

void main(void)
{
  TRISB = 0x00;
  TRISD = 0x00;
  PORTB = 0x00;
  PORTD = 0x00;
  init_I2C_Slave(0x10); // initialize I2C as slave with address 0x10

  unsigned char highNibble, lowNibble;

  for (;;)
  {
    highNibble = (VAL >> 4) & 0x0F;
    lowNibble = VAL & 0x0F;

    // PORTD = VAL;
    // decimalToHex(VAL, &highNibble, &lowNibble);

    // PORTB = HEX_TO_7SEG[lowNibble];
    // PORTD = HEX_TO_7SEG[highNibble];
  }
}
