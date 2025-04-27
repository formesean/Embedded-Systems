#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void init_I2C_Slave(unsigned char slave_add)
{
  TRISC3 = 1;         // set RC3 (SCL) to input
  TRISC4 = 1;         // set RC4 (SDA) to input
  SSPCON = 0x36;      // SSP enabled, SCK release clock
                      // I2C slave mode 7-bot address
  SSPCON2 = 0x01;     // start condition idle, stop condition idle
                      // receive idle
  SSPSTAT = 0x80;     // slew rate control disabled
  SSPADD = slave_add; // 7-bit slave address
  SSPIE = 1;          // enable SSP interrupt
  SSPIF = 0;          // clear interrupt flag
  PEIE = 1;           // enable peripheral interrupt
  GIE = 1;            // enable unmasked interrupt
}

void interrupt ISR(void)
{
  unsigned char temp;
  CKP = 0;           // hold clock low (SSPCON reg)
  if (WCOL || SSPOV) // check if overflow or data collision (SSPCON reg)
  {
    temp = SSPBUF; // read SSPBUF to clear buffer
    WCOL = 0;      // clear data collision flag
    SSPOV = 0;     // clear overflow flag
    CKP = 1;       // release clock (SSPCON reg)
  }
  /* check operation if “write” or "read"*/
  if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) // write to slave
  {
    temp = SSPBUF; // read SSPBUF to clear buffer
    while (!BF)
      ;             // wait until receive is complete (SSPSTAT reg)
    PORTB = SSPBUF; // write data from master to PORTB
    CKP = 1;        // release clock (SSPCON reg)
  }
  else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) // read from slave
  {
    temp = SSPBUF;  // read SSPBUF to clear buffer
    BF = 0;         // clear buffer status bit (SSPSTAT reg)
    SSPBUF = PORTD; // send data from PORTD to master
    CKP = 1;        // release clock (SSPCON reg)
    while (BF)
      ; // wait until transmit is complete (SSPSTAT reg)
  }
  SSPIF = 0; // clear interrupt flag
}

void main(void)
{
  TRISB = 0x00;         // set all bits in PORTB to output
  PORTB = 0x00;         // all LEDs in PORTB are off
  TRISD = 0xFF;         // set all bits in PORTD to input
  init_I2C_Slave(0x10); // initialize I2C as slave with address 0x01

  for (;;)
  {
  }
}
