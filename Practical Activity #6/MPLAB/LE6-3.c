#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char VAL;
unsigned char currentVAL;
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

void display_char(char data)
{
  while (!TRMT)
    ;
  TXREG = data;
}

void display_string(const char *str)
{
  while (*str != '\0')
    display_char(*str++);
}

void interrupt ISR()
{
  GIE = 0;  // disables all unmasked interrupts to prevent overlap
  if (INTF) // check the interrupt flag
  {
    INTF = 0; // clears the interrupt flag
  }
  else if (T0IF)
  {
    T0IF = 0;

    if (RB0 == 1)
    {
      VAL = (PORTB & 0xF0) >> 4;
      while (RB0);

      if (VAL == 0x00)
        currentVAL = 0x01;
      if (VAL == 0x01)
        currentVAL = 0x02;
      if (VAL == 0x02)
        currentVAL = 0x03;
      if (VAL == 0x04)
        currentVAL = 0x04;
      if (VAL == 0x05)
        currentVAL = 0x05;
      if (VAL == 0x06)
        currentVAL = 0x06;
      if (VAL == 0x08)
        currentVAL = 0x07;
      if (VAL == 0x09)
        currentVAL = 0x08;
      if (VAL == 0x0A)
        currentVAL = 0x09;
      if (VAL == 0x0C)
        currentVAL = 0x0C;
      if (VAL == 0x0D)
        currentVAL = 0x0A;
      if (VAL == 0x0E)
        currentVAL = 0x0E;
    }

    count_flag = 1;
  }
  GIE = 1; // enable interrupt again
}

void main(void)
{
  OPTION_REG = 0x05; // PS2:PS0 - prescaler 1:64
                     // PSA - prescaler assignment to TMR0
                     // T0CS - internal instruction cycle clock
                     // T0SE - not used since clock is internal
  T0IE = 1;          // enable Timer0 overflow interrupt
  T0IF = 0;          // clears the interrupt flag
  GIE = 1;           // enables all unmasked interrupt

  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  TX9 = 0;      // 8-bit transmission (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  TXEN = 1;     // transmit enable (TXSTA reg)

  TRISB = 0xFF;

  while (1) // foreground routine
  {
    if (currentVAL == 0x01)
    {
      display_string("You pressed 1.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x02)
    {
      display_string("You pressed 2.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x03)
    {
      display_string("You pressed 3.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x04)
    {
      display_string("You pressed 4.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x05)
    {
      display_string("You pressed 5.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x06)
    {
      display_string("You pressed 6.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x07)
    {
      display_string("You pressed 7.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x08)
    {
      display_string("You pressed 8.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x09)
    {
      display_string("You pressed 9.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x0A)
    {
      display_string("You pressed 0.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x0C)
    {
      display_string("You pressed *.\r\n");
      currentVAL = 0xFF;
    }

    if (currentVAL == 0x0E)
    {
      display_string("You pressed #.\r\n");
      currentVAL = 0xFF;
    }

    delay(10);
  }
}
