#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char tens = 0x30;
unsigned char ones = 0x30;
unsigned char VAL;
unsigned char currentVAL;
double currentDelay = 122;
unsigned char counter = 0;
int count = 0;
bit count_flag = 0;

const double delayMap[16] = {
    122 * 0.1, 122 * 0.2, 122 * 0.3, 0, // 0x00 - 0x03
    122 * 0.4, 122 * 0.5, 122 * 0.6, 0, // 0x04 - 0x07
    122 * 0.7, 122 * 0.8, 122 * 0.9, 0, // 0x08 - 0x0B
    0, 122, 0, 0,                       // 0x0C - 0x0F
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

    if (RB0 == 1)
    {
      VAL = PORTD & 0x0F;

      if (VAL < 16 && delayMap[VAL] != 0)
      {
        currentDelay = delayMap[VAL];
      }

      switch (VAL)
      {
      case 0x0C:
        count = 1;
        break;
      case 0x0E:
        count = 0;
        break;
      }
    }

    count_flag = 1;
  }
  GIE = 1;
}

void updateLCD()
{
    dataCtrl(tens);
    dataCtrl(ones);
    instCtrl(0x80); // Move cursor to the beginning
    delay(currentDelay);
}

void countUp()
{
  updateLCD();
  ones++;

  if (ones == 0x3A)
  {
    ones = 0x30;
    tens++;

    if (tens == 0x3A)
      tens = 0x30;
  }
}

void countDown()
{
  updateLCD();
  ones--;

  if (ones == 0x2F)
  {
    ones = 0x39;
    tens--;

    if (tens == 0x2F)
      tens = 0x39;
  }
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
    if (count == 0)
      countUp();
    else if (count == 1)
      countDown();
  }
}
