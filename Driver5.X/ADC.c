#include "ADC.h"

void ADCinit(void){
// Initialize CN interupt for AN5
    //CNEN2bits.CN29IE = 1;               //enable CN interrupts for CN29 - RA3/pin8/AN5
    
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
    AD1CON1bits.ADON = 1;         //ADC module on
    uint16_t ADCvalue;                //holds converted digital output
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

void ADC_Display(uint16_t V){
    //UART display function for ADC voltage
    Disp2String("\r");          //return carriage
    Disp2String(barGraph(V));   //determine & display those bars
    Disp2Hex(V);                //Display the current voltage
}

char* barGraph(uint16_t V){
    uint16_t v = V;
    //takes a voltage between 0x0 and 0x3FF
    //outputs a char (#) bar graph related to the voltage level with appropriate spacing
    // 0x3FF V will have '|####################|'
    static char graph[23];   
    graph[0] = '|';
    uint8_t i;
    for (i=1; v > 50; i++){
        v -= 50;
        graph[i] = '#';
    }
    while (i < 21){     //graph empty space loop
        graph[i++] = ' ';  
    }
    graph[i++] = '|';
    graph[i] = '\0';
    
    return graph;      //return string
}






//void __attribute__((interrupt,no_auto_psv)) _ADC1Interrupt(void){ if (IFS0bits.AD1IF == 1)IFS0bits.AD1IF = 0; }
