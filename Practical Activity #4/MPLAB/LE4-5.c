#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

int main()
{
  /* following the steps in setting up PWM */
  PR2 = 0x7C;     // set value for PR2
  CCPR1L = 0x57;  // set value for (8 MSBs)
  CCP1CON = 0x2C; // set value for (2 LSBs), PWM mode
  TRISC = 0x00;   // sets all of PORTC (RC2) to output
  RC2 = 0;        // initialize RC2 to 0
  T2CON = 0x06;   // 1:16 prescaler, Timer2 on

  for (;;) // foreground routine
  {
  }

  return 0;
}
