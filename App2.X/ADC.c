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


void ADC_Display(void){
    //UART display function for ADC voltage
    NewClk(500);
    char* prefix;
    char* suffix;
    if      (STATE == 1){ 
        prefix = "VOLTMETER Voltage   = ";
        suffix = "V             "; 
            while (STATE == 1){
                Disp2String("\r");          //return carriage
                Disp2String(prefix);        //ohms or volts
                ADC2mV(do_ADC());         //find current value    
                Disp2String(suffix);
            }

    }
    else if (STATE == 2){ 
        prefix = "OHMMETER Resistance = ";
        suffix = " Ohms";  
            while (STATE == 2 ){
                Disp2String("\r");          //return carriage
                Disp2String(prefix);        //ohms or volts
                ADC2ohm(do_ADC());         //find current value    
                Disp2String(suffix);
            }
    }
}

void ADC2mV(unsigned int V){
    // converting 10bit ADC bits into mV units
    unsigned int mv = 0;
    while (V > 24){
        mv += 2;
        V -= 2;
    } mv *= 3.25;

    //setting up/ executing UART display code
    static char buff[5];
    buff[4] = '\0';
    int i = 3;   
    while (i >= 0){
        buff[i--] = mv%10;
        mv  /= 10;
    }
    while(i < 4){
        if (i == 1) XmitUART2('.',1);
        XmitUART2(buff[i++] + 0x30 ,1);
    }
}
void ADC2ohm(unsigned int V){
    // converting 10bit ADC bits into mV units
    unsigned int R = 0;
    unsigned int mv = 0;
    while (V > 24){
        mv += 2;
        V -= 2;
    }
    mv *= 3;
    int temp = 3000 - mv;
    R = 1000 * mv/temp;
    
    //setting up/ executing UART display code
    static char buff[7];
    buff[6] = '\0';
    int i = 5;   
    while (i >= 0){
        buff[i--] = R%10;
        R  /= 10;
    }
    while(i < 6){
        XmitUART2(buff[i++] + 0x30 ,1);
    }
}



