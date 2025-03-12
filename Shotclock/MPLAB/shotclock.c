#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char tens = 0x32;
unsigned char ones = 0x34;
unsigned char VAL;
double currentDelay = 122 * 0.25;
unsigned int isOn = 0;
bit count_flag = 0;

void delay(double num)
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

void displayString(const char *str)
{
  while (*str)
  {
    dataCtrl(*str);
    str++;
  }
}

void updateLCD()
{
  instCtrl(0xC0);
  dataCtrl(tens);
  dataCtrl(ones);
  delay(currentDelay);
}

// count up from 00 to 24
void countUp()
{
  ones++;

	if (tens == 0x32 && ones == 0x35)
  {
   tens = 0x30;
   ones = 0x30;
  }
  
  if (ones == 0x3A)
  {
    ones = 0x30;
    tens++;
  }
  
  updateLCD();
}

// count down from 24 to 00
void countDown()
{
  ones--;
  
  if (ones == 0x2F)
  {
    ones = 0x39;
    tens--;

    if (tens == 0x2F)
    {
	    tens = 0x32;
	    ones = 0x34;
    }
  }
  
  updateLCD();
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
	    while (RB0);
	    
	    if (VAL == 0x0D)
	    	isOn ^= 1;
    }

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
  instCtrl(0x80);
	displayString("Time:");
	
  updateLCD();

  while (1)
  {
	  if (isOn)
	  	countDown();
	  else
	  {
		  if (RB0 == 1)
	    {
		    VAL = PORTD & 0x0F;
		    while (RB0);
		    
		    switch (VAL)
		    {
			    case 0x0C: countUp(); break;
			    case 0x0E: countDown(); break;
		    }
	    }	
	  }
  }
}

// EOF
