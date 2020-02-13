#include "LCD_Utilities.h"

void LCDInit ()
{

    PJSEL0 = BIT4 | BIT5;                   // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
    LCDCCTL0 |= LCDON;

 }

void LCD_All_On(void)
{

	int i;
	char *ptr = 0;
	ptr += 0x0A20;		// LCD memory starts at 0x0A20
	for (i = 0;i<21;i++)
		*ptr++ = 0xFF;
}

void LCD_all_off(void)
{
	int i;
	char *ptr = 0;
	ptr += 0x0A20;		// LCD memory starts at 0x0A20
	for (i = 0;i<21;i++)
		*ptr++ = 0x00;
}

void Display_Number (double n)
{
  int intpart=0;
  double decpart1=0;
  double decpart2=0;
  const unsigned char lcd_num[10] = {0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE4, 0xFF, 0xF7};
  
  char *Ptr2Num[6] = {0};
  Ptr2Num[0] +=0xA29;
  Ptr2Num[1] +=0xA25;
  Ptr2Num[2] +=0xA23;
  Ptr2Num[3] +=0xA32;
  Ptr2Num[4] +=0xA2E;
  Ptr2Num[5] +=0xA27;
  LCD_all_off();

  if(n<0)
    {
    	n=-n;
    	SIGN_LESS_ON;
    } 
  else
  {
    if(n<=0.1)
    {
      LCDM18 = 0x10;
      LCDM14 = 0x10;
      LCDM3 = 0x01;
    }
    if(n>0.1 && n<=1.6)
      {
          LCDM18 = 0x70;
          LCDM14 = 0x30;
      }
    if(n>1.6)
      {
          LCDM18 = 0xF0;
          LCDM14 = 0xF0;
      }
    LCDM20 = 0x08;
    LCDM19 = 0x08;
    intpart = (int)n;
    LCDM10 = lcd_num[intpart];  
    LCDM11 = 0x1;
    decpart1 = (n - intpart)*10;
    LCDM6  = lcd_num[(int)decpart1];
    decpart2 = (decpart1-(int)decpart1)*10;
    LCDM4 = lcd_num[(int)decpart2];
 }
}
 
/*void Dispaly_Digit(int d,int p)
{
  const unsigned char lcd_num[10] = {0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE4, 0xFF, 0xF7};
  char *Ptr2Num[6] = {0};
  Ptr2Num[0] +=0xA29;
  Ptr2Num[1] +=0xA25;
  Ptr2Num[2] +=0xA23;
  Ptr2Num[3] +=0xA32;
  Ptr2Num[4] +=0xA2E;
  Ptr2Num[5] +=0xA27;    
  *Ptr2Num[p] = lcd_num[d];
 }*/


