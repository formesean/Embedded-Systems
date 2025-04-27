#include <xc.h> // include file for the XC8 compiler
#include <string.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

void main(void)
{
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  RX9 = 0;      // 8-bit reception (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  CREN = 1;     // enable continuous receive (RCSTA reg)

  TRISA = 0x00; // set all ports in PORTA to output
  PORTA = 0x00; // all LEDs are off
  
  while (1) // foreground routine
  {
    if (RCIF)
	  { 
		  unsigned char received = RCREG;
		  if (received == 0x00 || received == 0x01)
		  	PORTA = received;
		}
  }
}