#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void interrupt ISR()
{
  GIE = 0;         // disable all unmasked interrupts (INTCON reg)
  if (CCP1IF == 1) // checks CCP1 interrupt flag
  {
    CCP1IF = 0;    // clears interrupt flag
    RA0 = RA0 ^ 1; // toggles the output signal at RA0
  }
  GIE = 1; // enable all unmasked interrupts (INTCON reg)
}

int main()
{
  ADCON1 = 0x06; // set all pins in PORTA as digital I/O
  TRISA = 0x00;  // sets all of PORTA to output
  RA0 = 0;       // initialize RA0 to 0

  T1CON = 0x20;   // 1:4 prescaler, Timer1 off
  CCP1CON = 0x0A; // compare mode: generate interrupt on match
  CCP1IE = 1;     // enable TMR1/CCP1 match interrupt (PIE1 reg)
  CCP1IF = 0;     // reset interrupt flag (PIR1 reg)
  CCPR1 = 0x4E2;  // set the match value to TMR1
  PEIE = 1;       // enable all peripheral interrupt (INTCON reg)
  GIE = 1;        // enable all unmasked interrupts (INTCON reg)
  TMR1ON = 1;     // Turns on Timer2 (T1CON reg

  for (;;) // foreground routine
  {
  }

  return 0;
}
