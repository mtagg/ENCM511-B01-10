#include "ADC.h"
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

//setup AN5 and AN11 (app2 requirements)  
    AD1PCFGbits.PCFG5 = 0;           //sets AN5 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL5 = 0;           //omit AN5 pin from digital input scan 
    AD1PCFGbits.PCFG11 = 0;          //sets AN11 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL11 = 0;          //omit AN11 pin from digital input scan 

//setup pin15 for analog in (to sample pulse amplitude - Midterm requirement)
    AD1PCFGbits.PCFG12 = 1;          //sets AN12 pin to sample pin voltage in digi mode
    AD1CSSLbits.CSSL12 = 1;          //AN12 ommitted from input scan
}

unsigned int do_ADC(void){  
//choose pin to sample from    
    if      (STATE == 1)    AD1CHSbits.CH0SA = 0b0101;       //channel 0 positive input is AN5  (pin 8) for MUX A
    else if (STATE == 2)    AD1CHSbits.CH0SA = 0b1011;       //channel 0 positive input is AN11 (pin 16) for MUX A
    else if (STATE == 3)    AD1CHSbits.CH0SA = 0b1100;       //channel 0 + input is AN12 (pin 15 from pulse osc)

//initialize ADC module
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
    //UART display function for ADC voltage
    if      (STATE == 1){ 

        Disp2String("\rVOLTMETER Voltage = ");     
        while (STATE == 1){
            ADC2mV(do_ADC());         //find current value    
            XmitUART2(8,7);
        }
    }
    else if (STATE == 2){ 

        Disp2String("\rOHMMETER Resistance = ");
        while (STATE == 2){
            ADC2ohm(do_ADC());         //find current value    
            XmitUART2(8,10);
        }
    }
    else if (STATE == 3){ 

        while(STATE == 3){
            Disp2String("\rPULSEMETER Freq = ");
            Disp2String(frequency());
            Disp2String(" kHz, Amplitude = ");
            Disp2String(amplitude());     
        }  
    }
    //clear line and return to main after every IO change 
    clearLine(); 
    return; 
}

void ADC2mV(unsigned int V){
// converting 10bit ADC bits into V units
    double v = (3.25*V)/1024;

// convert voltage to string & display
    char buff[5];
    sprintf(buff, "%1.3f V", v);
    Disp2String(buff);

    return;
}
void ADC2ohm(unsigned int V){
//convert 10bit ADC to V
    double v = (3.25*V)/1024;

//calculate R through Voltage divider maths    
    double temp = 3.26-v;
    double R = (1000*v)/temp;

//convert R into string and display
    char buff[10];
    sprintf(buff, "%5.0f Ohms", R);
    Disp2String(buff);

    return;
}

void clearLine(void){
    Disp2String("\r");
    XmitUART2(32,75);
    Disp2String("\r");

    return;
}
