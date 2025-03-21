#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>
#include <math.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

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

void main()
{
  TRISB = 0xFF;

  // PWM Mode
  PR2 = 0x7C;     // set value for PR2
  CCPR1L = 0x57;  // set value for (8 MSBs)
  CCP1CON = 0x2C; // set value for (2 LSBs), PWM mode
  TRISC = 0x00;   // sets all of PORTC (RC2) to output
  RC2 = 0;        // initialize RC2 to 0
  T2CON = 0x06;   // 1:16 prescaler, Timer2 on

  int freq = 1;
  int duty = 1;

  while (1)
  {
    delay(122 * 0.5);

    if (RB1 == 1)
    {
      if (freq == 3)
        freq = 1;
      else
        freq++;

      switch (freq)
      {
      case 1:
        PR2 = 0x1869;
        break; // 10 Hz
      case 2:
        PR2 = 0x270;
        break; // 100 Hz
      case 3:
        PR2 = 0x3E;
        break; // 1000 Hz
      }
    }

    if (RB2 == 1)
    {
      if (duty == 5)
        duty = 1;
      else
        duty++;

      if (freq == 1) // 10 Hz
      {
        switch (duty)
        {
        case 1:
          CCPR1L = 0x71;
          CCP1CON = 0x0C;
          break;
        case 2:
          CCPR1L = 0x1A;
          CCP1CON = 0x2C;
          break;
        case 3:
          CCPR1L = 0x35;
          CCP1CON = 0x0C;
          break;
        case 4:
          CCPR1L = 0x4F;
          CCP1CON = 0x2C;
          break;
        case 5:
          CCPR1L = 0x31;
          CCP1CON = 0x2C;
          break;
        }
      }

      if (freq == 2) // 100 Hz
      {
        switch (duty)
        {
        case 1:
          CCPR1L = 0x3E;
          CCP1CON = 0x2C;
          break;
        case 2:
          CCPR1L = 0x9C;
          CCP1CON = 0x1C;
          break;
        case 3:
          CCPR1L = 0x38;
          CCP1CON = 0x2C;
          break;
        case 4:
          CCPR1L = 0xD4;
          CCP1CON = 0x3C;
          break;
        case 5:
          CCPR1L = 0x51;
          CCP1CON = 0x3C;
          break;
        }
      }

      if (freq == 3) // 1000 Hz
      {
        switch (duty)
        {
        case 1:
          CCPR1L = 0x06;
          CCP1CON = 0x1C;
          break;
        case 2:
          CCPR1L = 0x0F;
          CCP1CON = 0x3C;
          break;
        case 3:
          CCPR1L = 0x1F;
          CCP1CON = 0x1C;
          break;
        case 4:
          CCPR1L = 0x2F;
          CCP1CON = 0x0C;
          break;
        case 5:
          CCPR1L = 0x3B;
          CCP1CON = 0x2C;
          break;
        }
      }
    }
  }
}

// EOF
