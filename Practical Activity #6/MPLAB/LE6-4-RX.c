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

    if (currentVAL == 0x01)
      PORTB = 0x10;

    if (currentVAL == 0x02)
      PORTB = 0x20;

    if (currentVAL == 0x03)
      PORTB = 0x30;

    if (currentVAL == 0x04)
      PORTB = 0x40;

    if (currentVAL == 0x05)
      PORTB = 0x50;

    if (currentVAL == 0x06)
      PORTB = 0x60;

    if (currentVAL == 0x07)
      PORTB = 0x70;

    if (currentVAL == 0x08)
      PORTB = 0x80;

    if (currentVAL == 0x09)
      PORTB = 0x90;

    if (currentVAL == 0x0A)
      PORTB = 0x00;

    if (currentVAL == 0x0C || currentVAL == 0x0E)
      PORTB = 0x01;
  }
}
