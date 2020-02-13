#include "LCD_Utilities.h"
int counter=0;
void main()
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;	        // Clear locked IO Pins
  
    LCDInit(); // Initialize LCD
    LCDCCTL0|=LCDON;
    // GPIO Setup
    P1OUT &= ~BIT0;                         // Clear LED to start
    P1DIR |= BIT0;                          // Set P1.0/LED to output
    
    //Use P8.4 (A7) for analog input
    
    P8SEL1 |= BIT5;                         // Configure P8.5 (A6)  for ADC
    P8SEL0 |= BIT5;

    // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_3 | ADC12ON;      // Sampling time, S&H=32, ADC12 on
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12CTL2 |= ADC12RES_2 ;                // 12-bit conversion results
    //REFCTL0|=REFVSEL_0+REFON;               //Vref=1.2v
    ADC12MCTL0 = ADC12INCH_6;//|ADC12VRSEL_1;              // A6 ADC input select; Vref+=Vref
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt

    while (1) 
    {
        __delay_cycles(5000);
        ADC12CTL0 |= ADC12ENC | ADC12SC;    // Start sampling/conversion

        __bis_SR_register(LPM0_bits | GIE); // LPM0, ADC12_ISR will force exit
        __no_operation();
    }   
}               

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    if (counter==2500)
    {
      counter=0;
      switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
      {
        case ADC12IV_NONE:        break;    // Vector  0:  No interrupt
        case ADC12IV_ADC12IFG0:             // Vector 12:  ADC12MEM0 Interrupt
            if (ADC12MEM0 >= 0x7ff)         // ADC12MEM0 = A1 > 0.5AVcc?
                P1OUT |= BIT0;              // P1.0 = 1
            else
                P1OUT &= ~BIT0;             // P1.0 = 0
                Display_Number((3.3/4096)*ADC12MEM0);
                // Exit from LPM0 and continue executing main
                __bic_SR_register_on_exit(LPM0_bits);
            break;
        case ADC12IV_ADC12IFG1:   break;    // Vector 14:  ADC12MEM1
        case ADC12IV_ADC12IFG2:   break;    // Vector 16:  ADC12MEM2
        case ADC12IV_ADC12IFG3:   break;    // Vector 18:  ADC12MEM3
        case ADC12IV_ADC12IFG4:   break;    // Vector 20:  ADC12MEM4
        // continue here with IFG5 to IFG31
        default: break;
      }
    }
    counter++;
}
