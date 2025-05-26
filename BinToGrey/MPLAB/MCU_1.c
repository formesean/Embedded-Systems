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

unsigned char VAL;
unsigned char toSend;

void interrupt ISR()
{
  GIE = 0;
  if (INTF)
  {
    INTF = 0;
  }
  else if (T0IF)
  {
    T0IF = 0;

    if (RB0 == 1)
    {
      VAL = (PORTB >> 4) & 0x0F;
      while (RB0)
        ;
      toSend = VAL;
    }
    GIE = 1;
  }
}

void main(void)
{
	OPTION_REG = 0x04;
  T0IE = 1;
  T0IF = 0;
  GIE = 1;
  
  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  TX9 = 0;      // 8-bit transmission (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  TXEN = 1;     // transmit enable (TXSTA reg)
  
  TRISB = 0xFF;
  // OPTION_REGbits.nRBPU = 0x01;

  //init_I2C_Master(); // initialize I2C as masters

  for (;;)
  {
	  while (!TRMT)
      ;          // wait until transmit shift register is empty
    TXREG = toSend;
    //I2C_Start();          // initiate start condition
    //I2C_Send(0x10);       // send the slave address + write
    //I2C_Send(toSend); // send 8-bit data frame
    //I2C_Stop();           // initiate stop condition
    //__delay_ms(200);      // delay before next operation
  }
}
