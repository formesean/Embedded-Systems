#include<xc.h> // include file for the XC8 compiler

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
		for (j = 0; j < 256; j++);
	}
}

void instCtrl(unsigned char INST)
{
	PORTB = INST;
	RC0 = 0;		// RS = 0 to accept data
	RC1 = 1;		// E = 1 (write)
	delay(1);
	RC1 = 0;		// E = 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
	PORTB = DATA;
	RC0 = 1;		// RS = 1 for data control
	RC1 = 1;		// E = 1 (write)
	delay(1);
	RC1 = 0;		// E = 0 (strobe)
}

void initLCD()
{
	delay(1000);	// LCD startup about 15ms
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0C); // display on; cursor off; blink off
}

void main()
{
	TRISB = 0x00;
	TRISC = 0x00;

	initLCD();

	while(1)
	{
		instCtrl(0xC6); // move cursor to 2nd line 7th column
		dataCtrl('H'); // prints ‘H’ at current cursor position
					   // then shifts the cursor to the right*
		dataCtrl('E'); // prints ‘E’
		dataCtrl('L'); // prints ‘L’
		dataCtrl('L'); // prints ‘L’
		dataCtrl('O'); // prints ‘O’
		dataCtrl('!'); // prints ‘!’
	}
}