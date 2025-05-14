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
unsigned char msd;
unsigned char lsd;
int digits = 1;

void instCtrl(unsigned char INST)
{
  PORTD = INST;
  RB1 = 0; // RS = 0 to accept data
  RB2 = 0;
  RB3 = 1; // E = 1 (write)
  __delay_ms(1);
  RB3 = 0; // E = 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
  PORTD = DATA;
  RB1 = 1; // RS = 1 for data control
  RB2 = 0;
  RB3 = 1; // E = 1 (write)
  __delay_ms(1);
  RB3 = 0; // E = 0 (strobe)
}

void initLCD()
{
  __delay_ms(1);
  instCtrl(0x38); // function set: 8-bit; dual-line
  instCtrl(0x08); // display off
  instCtrl(0x01); // display clear
  instCtrl(0x06); // entry mode: increment; shift off
  instCtrl(0x0C); // display on; cursor off; blink off
}

void displayString(const char *str)
{
  while (*str)
  {
    dataCtrl(*str);
    str++;
  }
}

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
      ;                // wait until receive is complete (SSPSTAT reg)
    received = SSPBUF; // write data from master to PORTB
    CKP = 1;           // release clock (SSPCON reg)
  }
  SSPIF = 0; // clear interrupt flag
}

void main(void)
{
  TRISB = 0x00;
  TRISD = 0x00;
  PORTB = 0x00;
  PORTD = 0x00;

  init_I2C_Slave(0x10); // initialize I2C as slave with address 0x01
  initLCD();
  instCtrl(0x80);
  displayString("Hex:");
  instCtrl(0xC0);
  displayString("Bin:");

  unsigned char currentValue = 0x00;
  unsigned char temp = 0x00;

  for (;;)
  {
    if (received != 0)
    {
      if (received == '*')
      {
        digits++;
        currentValue = 0;
      }
      else if (received == '#')
        currentValue = (currentValue + 1) & 0x0F;
      else if (received >= '0' && received <= '9')
        currentValue = received - '0';

      char hexChar;
      if (currentValue < 10)
        hexChar = currentValue + '0';
      else
        hexChar = currentValue - 10 + 'A';

      if (digits == 1)
      {
        instCtrl(0x85);
        dataCtrl(' ');
        instCtrl(0x85);
        dataCtrl(hexChar);
        if (hexChar >= '0' && hexChar <= '9')
          msd = hexChar - '0';
        else
          msd = hexChar + 10 - 'A';
      }
      if (digits == 2)
      {
        instCtrl(0x86);
        dataCtrl(' ');
        instCtrl(0x86);
        dataCtrl(hexChar);
        if (hexChar >= '0' && hexChar <= '9')
          lsd = hexChar - '0';
        else
          lsd = hexChar + 10 - 'A';
      }
      if (digits == 3)
      {
        digits = 1;

        char binBuffer[5];

        instCtrl(0xC5);

        for (int i = 3; i >= 0; i--)
          binBuffer[3 - i] = (msd & (1 << i)) ? '1' : '0';
        binBuffer[4] = '\0';
        displayString(binBuffer);

        for (int i = 3; i >= 0; i--)
          binBuffer[3 - i] = (lsd & (1 << i)) ? '1' : '0';
        binBuffer[4] = '\0';
        displayString(binBuffer);
      }

      received = 0;
    }
  }
}
