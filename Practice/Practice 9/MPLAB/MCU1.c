#include <xc.h>
#include <stdio.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

float temp;
int whole, decimal;

void interrupt ISR()
{
  GIE = 0;
  if (INTF)
  {
    INTF = 0;

    if (TRMT)
    {
      TXREG = whole;
      while (!TRMT);
      TXREG = decimal 
    }
  }
  GIE = 1;
}

void instCtrl(unsigned char inst)
{
  PORTD = inst;
  RB5 = 0;
  RB7 = 1;
  RB6 = 0;
  __delay_ms(1);
  RB7 = 0;
}

void dataCtrl(unsigned char data)
{
  PORTD = data;
  RB5 = 1;
  RB7 = 1;
  RB6 = 0;
  __delay_ms(1);
  RB7 = 0;
}

void initLCD()
{
  __delay_ms(30);
  instCtrl(0x38); // function set: 8-bit; dual-line
  instCtrl(0x08); // display off
  instCtrl(0x01); // display clear
  instCtrl(0x06); // entry mode: increment; shift off
  instCtrl(0x0C); // display on; cursor on; blink off
}

void display_char(char data)
{
  dataCtrl(data);
}

void display_string(const char *str)
{
  while (*str != '\0')
    display_char(*str++);
}

unsigned int convert(unsigned int data)
{
  data += '0';
  return data;
}

int readADC(void)
{
  int temp = 0;
  __delay_ms(30); // delay to get the hold capacitor charged
  GO = 1;    // start conversion
  while (GO_DONE == 1)
    ; // wait for conversion to finish
  /* read result register */
  temp = ADRESH;        // read ADRESH
  temp = temp << 8;     // move to correct position
  temp = temp | ADRESL; // read ADRESL
  return temp;
}

void main()
{
  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
  // (see formula in Table 10-1)
  SYNC = 0; // asynchronous mode (TXSTA reg)
  SPEN = 1; // enable serial port (RCSTA reg)
  RX9 = 0;  // 8-bit reception (TXSTA reg)
  TX9 = 0;
  BRGH = 1; // asynchronous high-speed (TXSTA reg)
  CREN = 1; // enable continuous receive (RCSTA reg)
  TXEN = 1; // transmit enable (TXSTA reg)

  TRISB = 0x01;
  TRISD = 0x00;

  INTEDG = 1;
  INTF = 0;
  INTE = 1;

  GIE = 1;

  ADCON1 = 0x80; // result: right Justified, clock: FOSC/2
                 // all ports in PORTA are analog
                 // VREF+: VDD, VREF-: VSS
  ADCON0 = 0x01; // clock: FOSC/2, analog channel: AN0,
                 // A/D conversion: STOP, A/D module: ON

  initLCD();
  instCtrl(0x80);

  display_string("VOLTAGE:\n");

  int d_value = 0;
  while (1)
  {
    d_value = readADC(); // get ADC value

    temp = (float)d_value * (4.9 / 100);
    whole = (int)temp / 10;
    decimal = (int)temp - (whole * 10);

    if (decimal > 9)
    {
      decimal = 0;
      whole += 1;
    }
    instCtrl(0x88);
    dataCtrl(convert(whole));
    dataCtrl('.');
    dataCtrl(convert(decimal));
  }
}
