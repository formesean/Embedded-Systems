#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char currentVAL;

void main(void)
{
  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  RX9 = 0;      // 8-bit reception (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  CREN = 1;     // enable continuous receive (RCSTA reg)

  TRISB = 0x00; // set all ports in PORTB to output
  PORTB = 0x00;

  while (1) // foreground routine
  {
    while (!RCIF)
      ;            // wait until receive buffer is full
    currentVAL = RCREG; // read the receive register

    if (currentVAL == '1')
      PORTB = 0x10;

    if (currentVAL == '2')
      PORTB = 0x20;

    if (currentVAL == '3')
      PORTB = 0x30;

    if (currentVAL == '4')
      PORTB = 0x40;

    if (currentVAL == '5')
      PORTB = 0x50;

    if (currentVAL == '6')
      PORTB = 0x60;

    if (currentVAL == '7')
      PORTB = 0x70;

    if (currentVAL == '8')
      PORTB = 0x80;

    if (currentVAL == '9')
      PORTB = 0x90;

    if (currentVAL == '0')
      PORTB = 0x00;

    if (currentVAL == '*' || currentVAL == '#')
      PORTB = 0x01;
  }
}
