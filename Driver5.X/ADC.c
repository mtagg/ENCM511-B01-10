#include "ADC.h"

void ADCinit(void){
    
// configure ADC settings in AD1CON1 register
    AD1CON1bits.FORM = 0b00;          //set FORM bits to integer mode    
    AD1CON1bits.SSRC = 0b111;         //Switch triggered ADC conversion
       
// configure ADC settings in AD1CON2
    AD1CON2bits.VCFG = 0b000;       //selects Vref to be from PIC VDD VSS
    AD1CON2bits.CSCNA = 0;          //do not scan inputs
    //AD1CON2bits.SMPI  = 0b0000;     //interrupts after completing each conversion sample
    IFS0bits.AD1IF = 0;             //reset interrupt flag just in case  
      
//configure ADC sampling
    AD1CON3bits.ADRC = 0;           //using system clock
    AD1CON3bits.SAMC = 0b11111;     //slowest sampling time, == 31*Period/2
    //AD1CON3bits.ADCS = 0b000000;    //clock pre-scalar if using external clock
    
// configure ADC input
    AD1CON2bits.ALTS = 0;           //use MUX A input settings
    AD1CHSbits.CH0NB - 0;           //channel 0 negative input is VR- for MUX B
    AD1CHSbits.CH0SB = 0b0101;      //Channel 0 positive input is AN5 for MUX B
    AD1CHSbits.CH0NA = 0;           //channel 0 negative input is VR- for MUX A
    AD1CHSbits.CH0SA = 0b0101;      //channel 0 positive input is AN5 for MUX A  
    AD1PCFGbits.PCFG5 = 0;          //sets AN5 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL5 = 0;          //omit AN5 pin from digital input scan
    
    
}
uint16_t do_ADC(void){    
    AD1CON1bits.ADON = 1;            //ADC module on
    
    uint16_t ADCvalue;              //holds converted digital output
    AD1CON2bits.BUFM = 0;           //buffer configured to 16-words

    AD1CON1bits.ASAM = 0;             //sampling will start when SAMP set
   
    //unneeded?? MCU should clear status bit after conversion is finished
    //AD1CON1bits.DONE = 1;             //ADC status bit

    
//start sampling
    AD1CON1bits.SAMP = 1;           //starts sampling after SSRC and SAMC are set
    while (AD1CON1bits.DONE == 0){} 
    ADCvalue = ADC1BUF0;            //link return value to ADC output
    AD1CON1bits.SAMP = 0;           //Stop sampling
    AD1CON1bits.ADON = 0;           //Turn off ADC, ADC value stored in ADC1BUF0
    return ADCvalue;                //10bit digital out
}

void ADC_Display(uint16_t V){
    
    Disp2Hex(V);
    Disp2String("\r");
    
}






//void __attribute__((interrupt,no_auto_psv)) _ADC1Interrupt(void){ if (IFS0bits.AD1IF == 1)IFS0bits.AD1IF = 0; }
