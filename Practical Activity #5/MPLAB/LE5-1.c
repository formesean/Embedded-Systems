#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay(int cnt)
{
  while (cnt--)
    ;
}

int readADC()
{
  int temp = 0;
  delay(1000); // delay to get the hold capacitor charged
  GO = 1;      // start conversion

  while (GO_DONE == 1)
    ; // wait for conversion to finish

  temp = ADRESH;        // read ADRESH
  temp = temp << 8;     // move to correct position
  temp = temp | ADRESL; // read ADRESL

  return temp;
}

void interrupt ISR()
{
  int d_value = 0;
  GIE = 0;
  if (ADIF == 1)  // checks CCP1 interrupt flag
  {
    ADIF = 0; // clear interrupt flag (INTCON register)

    d_value = readADC(); // read result register

    if (d_value >= 0 && d_value <= 169)
      PORTB = 0x00;
    if (d_value >= 170 && d_value <= 340)
      PORTB = 0x10;
    if (d_value >= 341 && d_value <= 511)
      PORTB = 0x18;
    if (d_value >= 512 && d_value <= 682)
      PORTB = 0x1C;
    if (d_value >= 683 && d_value <= 853)
      PORTB = 0x1E;
    if (d_value >= 854 && d_value <= 1024)
      PORTB = 0x1F;
  }
  GO = 1; // restart A/D conversion (ADCON0 register)
  GIE = 1;
}

int main()
{
  TRISB = 0x00;  // set all PORTB as output
  PORTB = 0x00;  // all LEDs are off
  ADCON1 = 0x80; // result: right Justified, clock: FOSC/8
                 // all ports in PORTA are analog
                 // VREF+: VDD, VREF-: VSS
  ADCON0 = 0x41; // clock: FOSC/8, analog channel: AN0,
                 // A/D conversion: STOP, A/D module: ON
  ADIE = 1; // A/C conversion complete interrupt enable (PIE1 register)
  ADIF = 0; // reset interrupt flag (PIR1 register)
  PEIE = 1; // enable all peripheral interrupt (INTCON register)
  GIE = 1; // enable all unmasked interrupt (INTCON register)
  GO = 1; // start A/D conversion (ADCON0 register)

  while (1) // foreground routine
  {
  }
}
