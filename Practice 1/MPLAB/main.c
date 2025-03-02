#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay(unsigned int MUL)
{
  unsigned int i, j;

  for (i = MUL; i != 0; i--)
  {
    for (j = 0; j < 256; j++)
      ;
  }
}

void instCtrl(unsigned char INST)
{
  PORTB = INST;
  RC0 = 0; // RS = 0 to accept data
  RC1 = 1; // E = 1 (write)
  delay(1);
  RC1 = 0; // E = 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
  PORTB = DATA;
  RC0 = 1; // RS = 1 for data control
  RC1 = 1; // E = 1 (write)
  delay(1);
  RC1 = 0; // E = 0 (strobe)
}

void initLCD()
{
  delay(1);
  instCtrl(0x38); // 0011 1000 | function set: 8-bit; dual-line
                  // 0010 1000 | function set: 4-bit; dual-line
  instCtrl(0x08); // 0000 1000 | display off
  instCtrl(0x0E); // 0000 1110 | display clear
  instCtrl(0x06); // 0000 0110 | entry mode: increment; shift off
  instCtrl(0x0E); // 0000 1110  | display on; cursor on; blink off
}

int main()
{
  TRISA = 0x00;
  TRISB = 0x00;
  TRISC = 0x00;
  TRISD = 0xFF;

  unsigned char VAL;
  unsigned int charCount = 0;
  unsigned int rowCount = 1;

  PORTA = 0x00;

  initLCD();
  instCtrl(0x80);

  while (1)
  {
    delay(100);

    PORTA = 0x01;
    delay(10);

    if (RD1 == 1)
    {
      VAL = PORTD & 0x01;

      if (charCount == 16)
      {
        if (rowCount == 1)
        {
          instCtrl(0xC0);
          rowCount++;
          charCount = 0;
        }

        else if (rowCount == 2)
        {
          instCtrl(0x01);
          instCtrl(0x80);
          rowCount = 1;
          charCount = 0;
        }
      }

      if (VAL == 0x01)
      {
        dataCtrl('1');
        charCount++;
      }

      PORTA = 0x00;
      delay(10);
    }
  }
}
