#include "pulse.h"

extern unsigned int STATE;
long unsigned int counter;

void pulseInit(){
    //Secondary Clock Setup
    OSCCONbits.SOSCEN = 1;
    //FOSCSEL = 0b101;
    //FOSCbits.SOSCSEL = 0;   //low power clock operation 

   //T3 setup (real-time comparator counter):
     T3CONbits.TON = 0; //start with timer off
     T3CONbits.TSIDL = 0; //timer continues in idle
     T3CONbits.TCKPS1 = 0;//no prescale
     T3CONbits.TCKPS0 = 0;//no prescale
     T3CONbits.TCS = 1; //use FOSC clock input 
     PR3 = 16000;// scaled to 1s based on clock frequency (fosc @ 32kHz)
     IFS0bits.T3IF = 0; //reset interrupt flag
     IEC0bits.T3IE = 1;  //enable T1 interrupts
     IPC2 = 0x7; //priority 7

   //PULSE GENERATION ON REFO/RB15/PIN18 
     TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
     REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
     REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep 
     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
     REFOCONbits.RODIV = 0b0000;  //changes frequency of pulse gen 
}

char* frequency(void){
    return "hi";
    AD1PCFGbits.PCFG11 = 1;          //sets AN11 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL11 = 1;          //omit AN11 pin from digital input scan 
    TRISBbits.TRISB12 = 1;        // set RB12 to digi input
    long unsigned int counter = 0; //counter at pin 15/RB12
    TMR3 = 0;
    IFS0bits.T3IF = 0;
    T3CONbits.TON = 1;  //start T3 timer (secondary clock)
    //t3 interrupt will occur after 1s of real time
    while (IFS0bits.T3IF == 0){
      counter++; //count based off of 
    }
    IFS0bits.T3IF = 0;  //reset t3 interrupt and stop counting
    static char freq [5];
    freq[4] = '\0';
    int i = 3;
    while (i>=0){
      freq[i--] = counter%10;
      counter/=10;
    }
    return freq;
}


  

unsigned int amplitude(void){
     AD1PCFGbits.PCFG11 = 0;          //sets AN12 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL11 = 0;          //omit AN12 pin from digital input scan 
    unsigned int temp, amp = 0, i = 0;
    while (i < 1000){
      temp = do_ADC();
      if (temp > amp) amp = temp;
      i++;
    }return amp;
}


void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    if (IFS0bits.T3IF == 1){
      T3CONbits.TON = 0;
    }
}