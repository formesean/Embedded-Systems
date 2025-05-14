#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

bit count_flag = 0;
unsigned int press = 0;
unsigned char VAL;
unsigned int currentMSD = 0;
unsigned int currentLSD = 0;
int msd, lsd;
const unsigned int keypadMap[16] = {
    1, 2, 3, 0,    // 0x00 - 0x03
    4, 5, 6, 0,    // 0x04 - 0x07
    7, 8, 9, 0,    // 0x08 - 0x0B
    '*', 0, '#', 0 // 0x0C - 0x0F
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
      VAL = (PORTB >> 1) & 0x0F;
      while (RB0)
        ;

      if (keypadMap[VAL] != 0 && keypadMap[VAL] >= 0 && keypadMap[VAL] <= 9)
      {
        press++;
        if (press == 1)
          currentMSD = keypadMap[VAL];
        else
          currentLSD = keypadMap[VAL];
      }
    }

    count_flag = 1;
  }
  GIE = 1;
}

void main(void)
{
  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  TX9 = 0;      // 8-bit transmission (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  TXEN = 1;     // transmit enable (TXSTA reg)

  OPTION_REG = 0x04;
  T0IE = 1;
  T0IF = 0;
  GIE = 1;

  TRISB = 0xFF;
  TRISD = 0x00;
  PORTD = 0x00;

  while (1)
  {
    // un unsigned char toSend = ((currentMSD - '0') << 4) | (currentLSD - '0');
    msd = currentMSD;
    lsd = currentLSD;
    PORTD = (msd << 4) | lsd;

    if (press == 2)
    {
      if (TRMT)
      {
        TXREG = msd;
        while (!TRMT)
          ;
        TXREG = lsd;
      }

      press = 0;
    }
  }
}
