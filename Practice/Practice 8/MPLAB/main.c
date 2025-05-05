#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>

#include "I2C.h"
#include "I2C_LCD.h"

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

const char *dataToSend = "Waiting...\n";
volatile bit adc_done = 0;
volatile int adc_result = 0;

void display_char(char data)
{
  while (!TRMT)
    ;
  TXREG = data;
}

void display_string(const char *str)
{
  while (*str != '\0')
    display_char(*str++);
}

void interrupt ISR()
{
  if (ADIF) // checks CCP1 interrupt flag
  {
    ADIF = 0; // clear interrupt flag (INTCON register)
    adc_result = (ADRESH << 8) | ADRESL;
    adc_done = 1;
    GO = 1; // restart A/D conversion (ADCON0 register)
  }
}

int main()
{
  ADCON1 = 0x80; // result: right Justified, clock: FOSC/8
                 // all ports in PORTA are analog
                 // VREF+: VDD, VREF-: VSS
  ADCON0 = 0x41; // clock: FOSC/8, analog channel: AN0,
                 // A/D conversion: STOP, A/D module: ON
  ADIE = 1;      // A/C conversion complete interrupt enable (PIE1 register)
  ADIF = 0;      // reset interrupt flag (PIR1 register)
  PEIE = 1;      // enable all peripheral interrupt (INTCON register)
  GIE = 1;       // enable all unmasked interrupt (INTCON register)
  GO = 1;        // start A/D conversion (ADCON0 register)

  SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
                // (see formula in Table 10-1)
  SYNC = 0;     // asynchronous mode (TXSTA reg)
  SPEN = 1;     // enable serial port (RCSTA reg)
  TX9 = 0;      // 8-bit transmission (TXSTA reg)
  BRGH = 1;     // asynchronous high-speed (TXSTA reg)
  TXEN = 1;     // transmit enable (TXSTA reg)

  TRISA0 = 1;
  TRISB = 0x00;
  PORTB = 0x00;

  init_I2C_Master();
  LCD_Init(0x4E); // Initialize LCD module with I2C address = 0x4E

  LCD_Set_Cursor(1, 1);
  LCD_Write_String("Strat:");

  while (1) // foreground routine
  {
    if (adc_done)
    {
      adc_done = 0;

      int d_value = adc_result;
      if (d_value >= 170 && d_value <= 340)
      {
        LCD_Set_Cursor(2, 1);
        LCD_Write_String("no. 3");
        dataToSend = "strat 3\r\n";
        PORTB = 0x03;
      }
      if (d_value >= 341 && d_value <= 510)
      {
        LCD_Set_Cursor(2, 1);
        LCD_Write_String("no. 2");
        dataToSend = "strat 2\r\n";
        PORTB = 0x02;
      }
      if (d_value >= 511 && d_value <= 682)
      {
        LCD_Set_Cursor(2, 1);
        LCD_Write_String("no. 1");
        dataToSend = "strat 1\r\n";
        PORTB = 0x01;
      }
      if (d_value >= 854 && d_value <= 1024)
      {
        LCD_Set_Cursor(2, 1);
        LCD_Write_String("no. 0");
        dataToSend = "strat 0\r\n";
        PORTB = 0x00;
      }

      display_string(dataToSend);
    }

    __delay_ms(100);
  }
}
