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

void init_I2C_Master(void)
{
  TRISC3 = 1;     // set RC3 (SCL) to input
  TRISC4 = 1;     // set RC4 (SDA) to input
  SSPCON = 0x28;  // SSP enabled, I2C master mode
  SSPCON2 = 0x00; // start condition idle, stop condition idle
                  // receive idle
  SSPSTAT = 0x00; // slew rate enabled
  SSPADD = 0x09;  // clock frequency at 100 KHz (FOSC = 4MHz)
}

void I2C_Wait(void)
{
  /* wait until all I2C operation are finished*/
  while ((SSPCON2 & 0x1F) || (SSPSTAT & 0x04))
    ;
}

void I2C_Start(void)
{
  /* wait until all I2C operation are finished*/
  I2C_Wait();
  /* enable start condition */
  SEN = 1; // SSPCON2
}

void I2C_Stop(void)
{
  /* wait until all I2C operation are finished*/
  I2C_Wait();
  /* enable stop condition */
  PEN = 1; // SSPCON2
}

void I2C_RepeatedStart(void)
{
  /* wait until all I2C operation are finished*/
  I2C_Wait();
  /* enable repeated start */
  RSEN = 1; // SSPCON2
}

void I2C_Send(unsigned char data)
{
  /* wait until all I2C operation are finished*/
  I2C_Wait();
  /* write data to buffer and transmit */
  SSPBUF = data;
}

unsigned char I2C_Receive(unsigned char ack)
{
  unsigned char temp;
  I2C_Wait();            // wait until all I2C operation are finished
  RCEN = 1;              // enable receive (SSPCON2 reg)
  I2C_Wait();            // wait until all I2C operation are finished
  temp = SSPBUF;         // read SSP buffer
  I2C_Wait();            // wait until all I2C operation are finished
  ACKDT = (ack) ? 0 : 1; // set acknowledge (ACK) or not acknowledge (NACK)
  ACKEN = 1;             // enable acknowledge sequence
  return temp;
}

void main(void)
{
  TRISB = 0x00;      // set all bits in PORTB to output
  PORTB = 0x00;      // all LEDs in PORTB are off
  TRISD = 0xFF;      // set all bits in PORTD to input
  init_I2C_Master(); // initialize I2C as masters

  for (;;)
  {
    I2C_Start();            // initiate start condition
    I2C_Send(0x10);         // send the slave address + write
    I2C_Send(PORTD);        // send 8-bit data frame
    I2C_Stop();             // initiate stop condition
    __delay_ms(200);        // delay before next operation
    I2C_Start();            // initiate repeated start condition
    I2C_Send(0x11);         // send the slave address + read
    PORTB = I2C_Receive(0); // read data and not acknowledge (NACK)
                            // end of read operation
                            // write received data to PORTB
    I2C_Stop();             // initiate stop condition
    __delay_ms(200);        // delay before next operation
  }
}
