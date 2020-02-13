#include "msp430.h"
#define NEG 0x04
#define ERR  0x9F
#define SIGN_LESS_ON LCDM11 |= NEG;
#define SIGN_LESS_OFF LCDM11 &= ~NEG;
#define SIGN_ERR_ON LCDM8 = ERR;

void Display_Number (double n);
void Dispaly_Digit(int d,int p);

void LCD_all_off(void);

void LCD_All_On(void);

void LCDInit ();
