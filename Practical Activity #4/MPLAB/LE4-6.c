#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>
#include <math.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit count_flag = 0;

int global_period = 0;
char digit[10] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9'};

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

void instCtrl(unsigned char INST)
{
  PORTD = INST;
  RB5 = 0; // RS = 0 to accept data
  RB6 = 0;
  RB7 = 1; // E = 1 (write)
  delay(1);
  RB7 = 0; // E = 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
  PORTD = DATA;
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

void interrupt ISR()
{
  int period = 0;
  GIE = 0;
  if (INTF)
  {
    INTF = 0;
  }
  else if (T0IF)
  {
    T0IF = 0;
    count_flag = 1;
  }
  else if (CCP1IF == 1) // checks CCP1 interrupt flag
  {
    CCP1IF = 0;            // clears interrupt flag
    TMR1 = 0;              // resets TMR1
    period = CCPR1 / 1000; // transfers captured TMR1 value
                           // normalize the value (make the number smaller)
    period = period * 8;   // multiply by the normalized TMR1 timeout
    global_period = period;
  }
  GIE = 1;
}

void main()
{
  TRISB = 0x01;
  TRISC = 0x04;
  TRISD = 0x00;

  OPTION_REG = 0x04;
  T0IE = 1;
  T0IF = 0;

  // Capture Mode
  T1CON = 0x30;   // 1:8 prescaler, Timer1 off
  CCP1CON = 0x05; // capture mode: every rising edge
  CCP1IE = 1;     // enable TMR1/CCP1 match interrupt (PIE1 reg)
  CCP1IF = 0;     // reset interrupt flag (PIR1 reg)
  PEIE = 1;       // enable all peripheral interrupt (INTCON reg)
  GIE = 1;        // enable all unmasked interrupts (INTCON reg)
  TMR1ON = 1;     // Turns on Timer1 (T1CON reg)

  initLCD();
  instCtrl(0x80);
  displayString("Period:");

  while (1)
  {
    instCtrl(0xC0);
    dataCtrl(digit[global_period / 10000]);
    dataCtrl(digit[(global_period % 10000) / 1000]);
    dataCtrl(digit[(global_period % 1000) / 100]);
    dataCtrl(digit[(global_period % 100) / 10]);
    dataCtrl(digit[global_period % 10]);
  }
}

// EOF
