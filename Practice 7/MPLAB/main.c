#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

int sw1_flag = 0;
int sw2_flag = 0;
bit count_flag = 0;

void delay(int num)
{
  int flags = 0;

  while (flags < num)
  {
    if (count_flag)
    {
      count_flag = 0;
      flags++;
    }
  }
}

void interrupt ISR()
{
  GIE = 0;
  if (INTF)
  {
    INTF = 0;
  }
  else if (RBIF)
  {
    RBIF = 0;

    if (RB4 == 1)
      sw1_flag = 1;
    if (RB7 == 1)
      sw2_flag = 1;
  }
  else if (T0IF)
  {
    T0IF = 0;
    count_flag = 1;
  }
  GIE = 1;
}

void main()
{

  TRISB = 0x90;
  TRISC = 0x00;

  OPTION_REG = 0x04;
  T0IE = 1;
  T0IF = 0;

  INTEDG = 1;
  INTE = 0;

  RBIE = 1;
  RBIF = 0;

  GIE = 1;

  PORTC = 0x00;
  while (1)
  {
    if (sw1_flag)
    {
      sw1_flag = 0;
      PORTC = 0x01;
    }

    if (sw2_flag)
    {
      sw2_flag = 0;
      PORTC = 0x02;
    }
  }
}
