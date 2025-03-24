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
        PR2 = 0x3E;
        break; // 1000 Hz 0011 1110
      case 2:
        PR2 = 0x29;
        break; // 1500 Hz
      case 3:
        PR2 = 0x0C;
        break; // 5000 Hz
      }
    }

    if (RB2 == 1)
    {
      if (duty == 5)
        duty = 1;
      else
        duty++;

      if (freq == 1) // 1000 Hz
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
          CCPR1L = 0x38;
          CCP1CON = 0x1C;
          break;
        }
      }

      if (freq == 2) // 1500 Hz
      {
        switch (duty)
        {
        case 1:
          CCPR1L = 0x04;
          CCP1CON = 0x1C;
          break;
        case 2:
          CCPR1L = 0x0A;
          CCP1CON = 0x2C;
          break;
        case 3:
          CCPR1L = 0x14;
          CCP1CON = 0x3C;
          break;
        case 4:
          CCPR1L = 0x1F;
          CCP1CON = 0x1C;
          break;
        case 5:
          CCPR1L = 0x27;
          CCP1CON = 0x2C;
          break;
        }
      }

      if (freq == 3) // 5000 Hz
      {
        switch (duty)
        {
        case 1:
          CCPR1L = 0x01;
          CCP1CON = 0x1C;
          break;
        case 2:
          CCPR1L = 0x03;
          CCP1CON = 0x1C;
          break;
        case 3:
          CCPR1L = 0x06;
          CCP1CON = 0x1C;
          break;
        case 4:
          CCPR1L = 0x09;
          CCP1CON = 0x2C;
          break;
        case 5:
          CCPR1L = 0x0C;
          CCP1CON = 0x0C;
          break;
        }
      }
    }
  }
}

// EOF
