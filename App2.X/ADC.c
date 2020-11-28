#include "ADC.h"
#include "ChangeClk.h"

extern unsigned int STATE;
void ADCinit(void){
    
// configure ADC settings in AD1CON1 register
    AD1CON1bits.FORM = 0b00;          //set FORM bits to integer mode    
    AD1CON1bits.SSRC = 0b111;         //Switch triggered ADC conversion
       
// configure ADC settings in AD1CON2
    AD1CON2bits.VCFG = 0b000;       //selects Vref to be from PIC VDD VSS
    AD1CON2bits.CSCNA = 0;          //do not scan inputs
    IFS0bits.AD1IF = 0;             //reset interrupt flag just in case  
      
//configure ADC sampling
    AD1CON3bits.ADRC = 0;           //using system clock
    AD1CON3bits.SAMC = 0b11111;     //slowest sampling time, == 31*Period/2
    
// configure ADC input
    AD1CON2bits.ALTS = 0;            //use MUX A input settings
    AD1CHSbits.CH0NA = 0;            //channel 0 negative input is VR- for MUX A
    AD1CHSbits.CH0SA = 0b0101;       //channel 0 positive input is AN5 for MUX A
    AD1CHSbits.CH0NB - 0;            //channel 0 negative input is VR- for MUX B
    AD1CHSbits.CH0SB = 0b1011;       //Channel 0 positive input is AN11 for MUX B
    
    AD1PCFGbits.PCFG5 = 0;           //sets AN5 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL5 = 0;           //omit AN5 pin from digital input scan 
    AD1PCFGbits.PCFG11 = 0;          //sets AN11 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL11 = 0;          //omit AN11 pin from digital input scan 
}

unsigned int do_ADC(void){    
    if      (STATE == 1)    AD1CHSbits.CH0SA = 0b0101;       //channel 0 positive input is AN5  (pin 8) for MUX A
    else if (STATE == 2)    AD1CHSbits.CH0SA = 0b1011;       //channel 0 positive input is AN11 (pin 16) for MUX A
    
    AD1CON1bits.ADON = 1;             //ADC module on
    unsigned int ADCvalue;            //holds converted digital output
    AD1CON2bits.BUFM = 0;             //buffer configured to 16-words
    AD1CON1bits.ASAM = 0;             //sampling will start when SAMP set  
    
//start sampling
    AD1CON1bits.SAMP = 1;           //starts sampling after SSRC and SAMC are set
    while (AD1CON1bits.DONE == 0){} 
    ADCvalue = ADC1BUF0;            //link return value to ADC output
    AD1CON1bits.SAMP = 0;           //Stop sampling
    AD1CON1bits.ADON = 0;           //Turn off ADC, ADC value stored in ADC1BUF0
    return ADCvalue;                //10bit digital out
}


void ADC_Display(unsigned int V){
    NewClk(500);
    char* prefix;
    char* suffix;
    if      (STATE == 1){ 
        prefix = "VOLTMETER Voltage   = ";
        suffix = "V  ";    
    }
    else if (STATE == 2){ 
        prefix = "OHMMETER Resistance = ";
        suffix = "Ohms";        
    }
    //UART display function for ADC voltage
    while (STATE != 3){
    Disp2String("\r");          //return carriage
    Disp2String(prefix);        //ohms or volts
    Disp2Dec(do_ADC());         //find current value    
    Disp2String(suffix);
    }
}


unsigned int ADC2mV(unsigned int V){
    return V*3250/1023; //returns the mV value based on 3.25V Vref and 1023 maximum possible V value
}


//********************************************************************************************
//                      THIS STUFF IS ALL SCREWED K
//********************************************************************************************
//unsigned int ADC2ohm(unsigned int V){
//    //3.25 Vref
//    // if V = 1023/2 then R must be 1k ohm 
//    //1023 is maximum possible V value
//    
////    R = ((1000*3.25)/(V*3.25)/1023) - 1000
//
//            R = (Rref*    (Vin/Vref)    - Rref
//            R = Rref*(1023)/adc - Rref
//                    
//            (adc/1023) 
//            // 1000 is the maximum Vin we can read which would mean R is huge
//            // 0 is the minimum, which means R would be tiny0
//                    
//            3.25V = amps(1000 + R) 
//            amps = (3.25(adc/1023)) / (R))
//                    
//                    1 = (adc/1023(R)) * (1000+R)
//                   1/adc = (1000+R)/(1023(R))
//                    (1023)R/adc = 1000R + R^2
//                       0 = R^2 + 1000R - (1023R/adc)
//                    3.25*V/1023
//                    2R =  sqrt(1mill + (4)(1023^2)/(3250*V)) -1000
//}









void __attribute__((interrupt,no_auto_psv)) _ADC1Interrupt(void){ if (IFS0bits.AD1IF == 1)IFS0bits.AD1IF = 0; }
