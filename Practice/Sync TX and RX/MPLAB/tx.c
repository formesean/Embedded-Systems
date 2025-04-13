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
  if (TXIF && TXIE) // Transmit interrupt
  {
    TXREG = '2'; // Load data
    TXIE = 0;    // Disable TX interrupt after sending one byte
  }
}

void main(void)
{
  SPBRG = 0x67; // 9.6K baud rate @ FOSC=4MHz, synchronous low speed
                // (see formula in Table 10-1)
  SYNC = 1;     // synchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  CSRC = 1;     // master mode (TXSTA reg)
  TXIE = 1;     // Enable transmit interrupt
  TX9 = 0;      // 8-bit transmission (TXSTA reg)
  TXEN = 1;     // transmit enable (TXSTA reg)
  PEIE = 1;     // Enable peripheral interrupts
  GIE = 1;      // Global interrupt enable

  TRISC6 = 0; // CK output (TX)
  TRISC7 = 0; // Data output (RX)

  while (1) // foreground routine
  {
    // Trigger TX only when no interrupt is ongoing
    if (!TXIE)
    {
      delay(10); // Delay before next character
      TXIE = 1;  // Re-enable interrupt to send next byte
    }
  }
}
