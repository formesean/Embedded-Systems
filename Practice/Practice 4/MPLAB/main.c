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
  instCtrl(0x38); // function set: 8-bit; dual-line
  instCtrl(0x08); // display off
  instCtrl(0x01); // display clear
  instCtrl(0x06); // entry mode: increment; shift off
  instCtrl(0x0C); // display on; cursor off; blink off
}

void main() {
    unsigned char DATA;
    unsigned char counter = 0;

    TRISB = 0x00;
    TRISD = 0xFF;
    TRISC = 0x00;

    initLCD();
    instCtrl(0x80); // Move cursor to beginning

    const unsigned char charMap[16] = {
        '1', '2', '3', 0,  // 0x00 - 0x03
        '4', '5', '6', 0,  // 0x04 - 0x07
        '7', '8', '9', 0,  // 0x08 - 0x0B
        '*', '0', '#', 0   // 0x0C - 0x0F
    };

    while (1) {
        if (RD4 == 1) {
            DATA = PORTD & 0x0F;
            while (RD4);

            if (charMap[DATA] != 0) {
                dataCtrl(charMap[DATA]);
                counter++;

                switch (counter) {
                    case 0x14: instCtrl(0xC0); break;
                    case 0x28: instCtrl(0x94); break;
                    case 0x3C: instCtrl(0xD4); break;
                    case 0x51:
                        counter = 0x00;
                        initLCD();
                        instCtrl(0x80);
                        break;
                }
            }
        }
    }
}
