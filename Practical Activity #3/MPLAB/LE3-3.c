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
unsigned char currentVAL;
bit count_flag = 0;
bit myINTF = 0;
bit myTMR0 = 0;

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

void interrupt ISRT()
{
	GIE = 0;		// disables all unmasked interrupts to prevent overlap
	if (INTF)		// check the interrupt flag
	{
		INTF = 0;	// clears the interrupt flag
		myINTF ^= 1;
	}
	else if (T0IF)
	{
		T0IF = 0;
		
		if (RB0 == 1)
		{
			VAL = PORTD & 0x0F;
			
			if (VAL == 0x00)
				currentVAL = 0x01;
			if (VAL == 0x01)
				currentVAL = 0x02;
			if (VAL == 0x02)
				currentVAL = 0x03;
			if (VAL == 0x04)
				currentVAL = 0x04;
			if (VAL == 0x05)
				currentVAL = 0x05;
			if (VAL == 0x06)
				currentVAL = 0x06;
			if (VAL == 0x08)
				currentVAL = 0x07;
			if (VAL == 0x09)
				currentVAL = 0x08;
			if (VAL == 0x0A)
				currentVAL = 0x09;
			if (VAL == 0x0D)
				currentVAL = 0x00;
		}
		
		count_flag = 1;
	}
	GIE = 1;		// enable interrupt again
}

int main ()
{
	TRISA = 0x00;
	TRISB = 0xFF;
	TRISC = 0x00;
	TRISD = 0xFF;
	
	OPTION_REG = 0x05; 	// PS2:PS0 - prescaler 1:64
						// PSA - prescaler assignment to TMR0
						// T0CS - internal instruction cycle clock
						// T0SE - not used since clock is internal
	T0IE = 1; 	// enable Timer0 overflow interrupt
	T0IF = 0; 	// clears the interrupt flag
	GIE = 1; 	// enables all unmasked interrupt
	
	PORTA = 0x00;
	PORTC = 0x00;
	while (1)
	{
		if (currentVAL == 0xA)
		{
			PORTC = 0x00;
			currentVAL = 0x00;
		}
		else
		{
			if (currentVAL == 0x00)
				PORTC == 0x00;
			else if (currentVAL == 0x01)
				PORTC = 0x01;
			else if (currentVAL == 0x02)
				PORTC = 0x02;
			else if (currentVAL == 0x03)
				PORTC = 0x03;
			else if (currentVAL == 0x04)
				PORTC = 0x04;
			else if (currentVAL == 0x05)
				PORTC = 0x05;
			else if (currentVAL == 0x06)
				PORTC = 0x06;
			else if (currentVAL == 0x07)
				PORTC = 0x07;
			else if (currentVAL == 0x08)
				PORTC = 0x08;
			else if (currentVAL == 0x09)
				PORTC = 0x09;
			currentVAL++;
		}
		delay(61);
	}
}

// EOF
