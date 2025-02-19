#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char VAL;

void interrupt ISRT()
{
	GIE = 0;		// disables all unmasked interrupts to prevent overlap
	if (INTF)		// check the interrupt flag
	{
		INTF = 0;	// clears the interrupt flag
		
		if (RB0 == 1)
		{
			VAL = PORTD & 0x0F;
			
			if (VAL == 0x00)
			{
				PORTC = 0x01;
			} 
			
			if (VAL == 0x01)
			{
				PORTC = 0x02;
			}
			
			if (VAL == 0x02)
			{
				PORTC = 0x03;
			}
			
			if (VAL == 0x04)
			{
				PORTC = 0x04;
			}
			
			if (VAL == 0x05)
			{
				PORTC = 0x05;
			}
			
			if (VAL == 0x06)
			{
				PORTC = 0x06;
			}
			
			if (VAL == 0x08)
			{
				PORTC = 0x07;
			}
			
			if (VAL == 0x09)
			{
				PORTC = 0x08;
			}
			
			if (VAL == 0x0A)
			{
				PORTC = 0x09;
			}
			
			if (VAL == 0x0D)
			{
				PORTC = 0x00;
			}
		}
	}
	GIE = 1;		// enable interrupt again
}

int main ()
{
	TRISB = 0xFF;
	TRISC = 0x00;
	TRISD = 0xFF;
	
	INTEDG = 1;	// interrupt at rising edge
	INTE = 1;	// enable RB0/INT external interrupt
	INTF = 0;	// clears the flag
	GIE = 1;	// enables all unmasked interrupt
	
	PORTC = 0x00;
	while (1)
	{
	}
}

// EOF
