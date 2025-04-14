#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char VAL;
unsigned char toSend;
unsigned char received;
int willSend = 0;
bit count_flag = 0;

void interrupt ISR()
{
  GIE = 0;  // disables all unmasked interrupts to prevent overlap
  if (INTF) // check the interrupt flag
  {
    INTF = 0; // clears the interrupt flag
  }
  else if (T0IF)
  {
    T0IF = 0;

    if (RB0 == 1)
      willSend = 1;

    count_flag = 1;
  }
  GIE = 1; // enable interrupt again
}

void main(void)
{
  OPTION_REG = 0x05; // PS2:PS0 - prescaler 1:64
                     // PSA - prescaler assignment to TMR0
                     // T0CS - internal instruction cycle clock
                     // T0SE - not used since clock is internal
  T0IE = 1;          // enable Timer0 overflow interrupt
  T0IF = 0;          // clears the interrupt flag
  GIE = 1;           // enables all unmasked interrupt
  PEIE = 1;

  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  TX9 = 0;      // 8-bit transmission (TXSTA reg)
  RX9 = 0;      // 8-bit reception (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  TXEN = 1;     // transmit enable (TXSTA reg)
  CREN = 1;     // enable continuous receive (RCSTA reg)

	TRISA = 0x00;
  TRISB = 0xFF; // bit 0 is push button, bit 4-7 is dip switch
  TRISD = 0x00;

  while (1) // foreground routine
  {
	  toSend = (PORTB & 0xF0) >> 4;
	 	PORTA = toSend;
	  	
	  if (willSend)
	  {
		  willSend = 0;
		  while (!TXIF);
    	TXREG = toSend;
	  }
	  
	  if (RCIF)
	  {
		  if (OERR)
		  {
			  CREN = 0;
			  CREN = 1;
		  }
		  
		  received = RCREG; // read the receive register

	   	if (received == 0x00)
	    	PORTD = 0x00;    
	    else if (received == 0x01)
	    	PORTD = 0x01;
		  else if (received == 0x2)
	    	PORTD = 0x02;
	   	else if (received == 0x03)
	    	PORTD = 0x03;
		  else if (received == 0x04)
	    	PORTD = 0x04;
	   	else if (received == 0x05)
	    	PORTD = 0x05;    	
	   	else if (received == 0x06)
	    	PORTD = 0x06;    
	    else if (received == 0x07)
	    	PORTD = 0x07;
		  else if (received == 0x08)
	    	PORTD = 0x08;
	   	else if (received == 0x09)
	    	PORTD = 0x09;
		  else
		  	PORTD = 0x0A;
	  }
  }
}