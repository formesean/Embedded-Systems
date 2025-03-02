#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char ledBinCount = 0x00;
int isPaused = 0;
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
  GIE = 0;  // disables all unmasked interrupts to prevent overlap
  if (INTF) // check the interrupt flag
  {
    INTF = 0; // clears the interrupt flag
    isPaused ^= 1;
  }
  else if (T0IF)
  {
    T0IF = 0;
    count_flag = 1;
  }
  GIE = 1; // enable interrupt again
}

int main()
{
  TRISA = 0x00;
  TRISB = 0xFF;

  OPTION_REG = 0x04;  // 1:32 prescaler TMR0
  T0IE = 1;
  T0IF = 0;
  GIE = 1;

  PORTA = ledBinCount;

  while (1)
  {

    if (isPaused)
    {
      if (ledBinCount > 0x0F)
        ledBinCount = 0x00;
      else
        ledBinCount++;
    }

    PORTA = ledBinCount;
    delay(122);
  }
}

// EOF
