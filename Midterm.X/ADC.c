#include "ADC.h"


unsigned int STATE;
void ADCinit(void){
    
// configure ADC settings in AD1CON1 register
    AD1CON1bits.FORM = 0b00;          //set FORM bits to integer mode    
    AD1CON1bits.SSRC = 0b111;         //Switch triggered ADC conversion
       
// configure ADC settings in AD1CON2
    AD1CON2bits.VCFG = 0b000;       //selects Vref to be from PIC VDD VSS
    AD1CON2bits.CSCNA = 0;          //do not scan inputs
    //IFS0bits.AD1IF = 0;             //reset interrupt flag just in case  
      
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
    else if (STATE == 3)    AD1CHSbits.CH0SA = 0b1100;      //channel 0 + input is AN12 (pin 15 from pulse osc)
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


void ADC_Display(void){
         
     if (STATE == 1){
        Disp2String("\rVOLTMETER Voltage   = ");        //ohms or volts
        //while (STATE == 1){
            Disp2String(ADC2mV(do_ADC()));         //find current value    
            Disp2String("V   ");
            XmitUART2(8,9);
        //}
    }
    else if (STATE == 2){ 
        Disp2String("\rOHMMETER Resistance = ");        //ohms or volts  
        //while (STATE == 2){
            ADC2ohm(do_ADC());         //find current value    
            Disp2String(" Ohms");
            XmitUART2(8,12);
       // } 
    }
    else if (STATE ==3){
        Disp2String("\rPULSEMETER Freq = ");
        Disp2String(frequency());
        Disp2String(" kHz, Amplitude = ");
        ADC2mV(amplitude());         //find current value
        Disp2String(" V");           
    }  
}

char* ADC2mV(unsigned int V){
    static char buff[5];
    sprintf(buff, "%1.3f", (V*3.25/1023));
    return buff;
    
}
void ADC2ohm(unsigned int V){
    
    // converting 10bit ADC bits into mV units
    unsigned long int R;
    unsigned int mv = 0;
    while (V >= 2){            //converts 10bit into Voltage based on Vref
        mv +=6;             
        V-=2;
    }
    unsigned int i = 0;
    R = 0;
    while (i < 1000){           // multiplication through successive addition
        R+=mv;
        i+=1;
    }R/=(3067-mv);              // R = (Ro * Vin)/(Vref-Vin)
    Disp2Dec(R);  
    //measurable range: 0 ohm -> 51344 Ohm (@ open circuit)
    
}
void clear(void){
    Disp2String("\r");
    XmitUART2(32,50);
    Disp2String("\r");
}


