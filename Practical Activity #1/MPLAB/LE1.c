#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay_ms(unsigned int time)
{
	unsigned int i, j;

	for (i = 0; i < time; i++)
	{
		for (j = 0; j < 1000; j++);
	}
}

void main()
{
	unsigned i;
	TRISA = 0xFF;	// set all bits (port) in PORTA as input
	TRISB = 0x00;	// set all bits (port) in PORTB as output
	ADCON1 = 0x06;

	for(;;)
	{
		PORTB = 0x00;	// set RB0 to 0 (LED OFF)

		if (PORTA == 0x01)
		{
			for (i = 0; i < 3; i++)
			{
				PORTB = 0x01;	// set RB0 to 1 (LED ON)
				delay_ms(50);
				PORTB = 0x00;	// set RB0 to 0 (LED OFF)
				delay_ms(50);
			}
		}
	}
}