#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>

#include "I2C.h"

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

unsigned char received;

//void interrupt ISR(void)
//{
//  unsigned char temp;
//  CKP = 0;           // hold clock low (SSPCON reg)
//  if (WCOL || SSPOV) // check if overflow or data collision (SSPCON reg)
//  {
//    temp = SSPBUF; // read SSPBUF to clear buffer
//    WCOL = 0;      // clear data collision flag
//    SSPOV = 0;     // clear overflow flag
//    CKP = 1;       // release clock (SSPCON reg)
//  }
//  /* check operation if “write” or "read"*/
//  if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) // write to slave
//  {
//    temp = SSPBUF; // read SSPBUF to clear buffer
//    while (!BF)
//      ;                // wait until receive is complete (SSPSTAT reg)
//    received = SSPBUF; // write data from master to PORTB
//    CKP = 1;           // release clock (SSPCON reg)
//  }
//  SSPIF = 0; // clear interrupt flag
//}

void main(void)
{
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  RX9 = 0;      // 8-bit reception (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  CREN = 1;     // enable continuous receive (RCSTA reg)
  
  TRISB = 0x00;         // set all bits in PORTB to output
  PORTB = 0x00;         // all LEDs in PORTB are off
  //init_I2C_Slave(0x10); // initialize I2C as slave with address 0x01

  for (;;)
  {
	  while (!RCIF)
      ; 
      
	  unsigned char org = RCREG;
	  unsigned char temp = org >> 1;
	  
	  PORTB = org ^ temp;
  }
}
