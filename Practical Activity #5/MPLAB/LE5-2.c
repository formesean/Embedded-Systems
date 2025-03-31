#include <xc.h> // include file for the XC8 compiler
#include <math.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

int display_toggle = 0;

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

float round_to_one_dec(float num)
{
  return round(num * 10.0) / 10.0;
}

void interrupt ISR()
{
  int d_value = 0;
  GIE = 0;
  if (ADIF == 1)  // checks CCP1 interrupt flag
  {
    ADIF = 0; // clear interrupt flag (INTCON register)

    d_value = readADC(); // read result register

    float result_of_conversion = (float) d_value * 0.004911;
    float formatted_result = round_to_one_dec(result_of_conversion);
    
    int whole = (int) formatted_result;
    int decimal = (int) ((formatted_result - whole) * 10);
    
    if (display_toggle)
    {
	    PORTB = whole & 0x0F;
	    RB4 = 0;
	    RB5 = 1;
	    RB6 = 1;
    }
    else
    {
	    PORTB = decimal & 0x0F;
	    RB4 = 1;
	    RB5 = 0;
	    RB6 = 0;
    }
    
    display_toggle = !display_toggle;
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
