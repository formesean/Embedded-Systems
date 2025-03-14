#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char VAL;
bit count_flag = 0;

const unsigned char charMap[16] = {
    '1', '2', '3', 0, // 0x00 - 0x03
    '4', '5', '6', 0, // 0x04 - 0x07
    '7', '8', '9', 0, // 0x08 - 0x0B
    '*', '0', '#', 0, // 0x0C - 0x0F
};

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

void instCtrl(unsigned char INST)
{
  PORTC = INST;
  RB5 = 0; // RS = 0 to accept data
  RB6 = 0;
  RB7 = 1; // E = 1 (write)
  delay(1);
  RB7 = 0; // E = 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
  PORTC = DATA;
  RB5 = 1; // RS = 1 for data control
  RB6 = 0;
  RB7 = 1; // E = 1 (write)
  delay(1);
  RB7 = 0; // E = 0 (strobe)
}

void initLCD()
{
  delay(1);
  instCtrl(0x38); // function set: 8-bit; dual-line
  instCtrl(0x08); // display off
  instCtrl(0x01); // display clear
  instCtrl(0x06); // entry mode: increment; shift off
  instCtrl(0x0C); // display on; cursor off; blink off
}

void interrupt ISR()
{
  GIE = 0;
  if (INTF)
  {
    INTF = 0;
  }
  else if (T0IF)
  {
    T0IF = 0;
    count_flag = 1;
  }
  GIE = 1;
}

void main()
{

  TRISB = 0x01;
  TRISC = 0x00;
  TRISD = 0x0F;

  OPTION_REG = 0x04;
  T0IE = 1;
  T0IF = 0;
  GIE = 1;

  initLCD();
  instCtrl(0x80); // Move cursor to beginning

  while (1)
  {
    if (RB0 == 1)
    {
      VAL = PORTD & 0x0F;
      while (RB0)
        ;

      if (charMap[VAL] != 0)
      {
        dataCtrl(charMap[VAL]);
        instCtrl(0x80);
      }
    }
  }
}
