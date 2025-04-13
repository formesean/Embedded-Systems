#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

volatile char receivedData = 0;
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
  if (RCIF && RCIE) // Receive interrupt
  {
    if (OERR)
    {
      CREN = 0; // Reset receiver
      CREN = 1;
      return;
    }

    receivedData = RCREG;
    PORTB = receivedData;

    // if (receivedData == 'S')
    // {
    //   PORTB = 0x00; // Clear all PORTB first
    //   RB0 = 0xFF;      // Turn on LED at RB0
    //   delay(250);   // Flash duration
    //   RB0 = 0x00;      // Turn off LED
    // }
    // else
    // {
    //   PORTB = receivedData; // Default: display full value on LEDs
    // }
  }
}

void main(void)
{
  SPBRG = 0x67; // 9.6K baud rate @ FOSC=4MHz, synchronous low speed
                // (see formula in Table 10-1)
  SYNC = 1;     // synchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  CSRC = 0;     // slave mode (TXSTA reg)
  RCIE = 1;     // Enable receive interrupt
  RX9 = 0;      // 8-bit reception (TXSTA reg)
  CREN = 1;     // enable continuous receive (RCSTA reg)
  PEIE = 1;     // Enable peripheral interrupts
  GIE = 1;      // Global interrupt enable

  TRISC6 = 1;   // CK input
  TRISC7 = 1;   // DT input
  TRISB = 0x00; // Output for LEDs
  PORTB = 0x00;

  while (1) // foreground routine
  {
  }
}
