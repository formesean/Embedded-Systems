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
unsigned char currentVAL;
const unsigned char charMap[16] = {
    '1', '2', '3', 0, // 0x00 - 0x03
    '4', '5', '6', 0, // 0x04 - 0x07
    '7', '8', '9', 0, // 0x08 - 0x0B
    0, '0', 0, 0      // 0x0C - 0x0F
};

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

      if (charMap[VAL] != 0 || (charMap[VAL] >= '0' && charMap[VAL] <= '9'))
        currentVAL = charMap[VAL];
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

  TRISB = 0xFF;
  TRISD = 0x00;
  PORTD = 0x00;

  init_I2C_Master(); // initialize I2C as masters

  for (;;)
  {
    I2C_Start();          // initiate start condition
    I2C_Send(0x10);       // send the slave address + write
    I2C_Send(currentVAL); // send 8-bit data frame
    I2C_Stop();           // initiate stop condition
    currentVAL = 0;
    __delay_ms(200);      // delay before next operation
  }
}
