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

#define _XTAL_FREQ 4000000

unsigned char VAL;
unsigned int hex1;
unsigned int hex2;
const unsigned int HEX_TO_7SEG[16] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001  // F
};

void main(void)
{
  SPBRG = 0x19;
  SYNC = 0;
  SPEN = 1;
  RX9 = 0;
  BRGH = 1;
  CREN = 1;

  TRISB = 0x00;
  TRISD = 0x00;

  PORTB = 0x00;
  PORTD = 0x00;

  while (1)
  {
    while (!RCIF)
      ; // wait until receive buffer is full
    hex1 = RCREG;

    while (!RCIF)
      ; // wait until receive buffer is full
    hex2 = RCREG;

    unsigned int decimal = hex1 / 16;
    unsigned int remainder = hex2 % 16;
  }
}
