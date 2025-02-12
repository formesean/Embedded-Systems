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
	instCtrl(0x0C); // display on; cursor on; blink off
}

void main()
{
	TRISA = 0x00;
	TRISD = 0xFF;
	
	unsigned char VAL;

	initLCD();
	PORTA = 0x00;

	while(1)
	{
		if (RD4 == 1)
		{
			VAL = PORTD & 0x0F;
			
			if (VAL == 0x00)
			{
				PORTA = 0x01;
			} 
			
			if (VAL == 0x01)
			{
				PORTA = 0x02;
			}
			
			if (VAL == 0x02)
			{
				PORTA = 0x03;
			}
			
			if (VAL == 0x04)
			{
				PORTA = 0x04;
			}
			
			if (VAL == 0x05)
			{
				PORTA = 0x05;
			}
			
			if (VAL == 0x06)
			{
				PORTA = 0x06;
			}
			
			if (VAL == 0x08)
			{
				PORTA = 0x07;
			}
			
			if (VAL == 0x09)
			{
				PORTA = 0x08;
			}
			
			if (VAL == 0x0A)
			{
				PORTA = 0x09;
			}
			
			if (VAL == 0x0C)
			{
				PORTA = 0x0A;
			}
			
			if (VAL == 0x0D)
			{
				PORTA = 0x00;
			}
			
			if (VAL == 0x0E)
			{
				PORTA = 0x0B;
			}
		}
	}
}