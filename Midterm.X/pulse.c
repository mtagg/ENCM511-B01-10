#include "pulse.h"

extern unsigned int STATE;
long unsigned int counter;

void pulseInit(){
  T2CONbits.T32 = 0;           // 16-bit timer mode on for T3 AND T2
  //T2 setup :
     T2CONbits.TON = 0; //start with timer off
     T2CONbits.TSIDL = 0; //timer continues in idle
     T2CONbits.TCKPS1 = 0;//no prescale
     T2CONbits.TCKPS0 = 0;//no prescale
     T2CONbits.TCS = 1; //use FOSC clock input (8MHz)
     PR2 = 1000;
     IFS0bits.T2IF = 0; //reset interrupt flag
     IEC0bits.T2IE = 1;  //enable T1 interrupts
     IPC1 = 0x5FFF; //priority 5
   //T3 setup:
     T3CONbits.TON = 0; //start with timer off
     T3CONbits.TSIDL = 0; //timer continues in idle
     T3CONbits.TCKPS1 = 0;//no prescale
     T3CONbits.TCKPS0 = 0;//no prescale
     T3CONbits.TCS = 1; //use primary clock (unknown frequency) 
     PR3 = 1000;
     IFS0bits.T3IF = 0; //reset interrupt flag
     IEC0bits.T3IE = 1;  //enable T3 interrupts
     IPC2 = IPC2 | 0x4; //priority 4

   //PULSE GENERATION SETTINGS ON REFO/RB15/PIN18 

   //Setting #1:
    //  NewClk(8);
    //  TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
    //  REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep 
    //  REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
    //  REFOCONbits.RODIV = 0b0011;  //changes frequency of pulse gen 
    //  REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
    //  //Setting #2:
    //  NewClk(8);
    //  TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
    //  REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep 
    //  REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
    //  REFOCONbits.RODIV = 0b1111;  //changes frequency of pulse gen 
    //  REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
    //  //Setting #3:
    //  NewClk(500);
    //  TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
    //  REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep 
    //  REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
    //  REFOCONbits.RODIV = 0b1001;  //changes frequency of pulse gen 
    //  REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
     //Setting #4:
     NewClk(500);
     TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
     REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep 
     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
     REFOCONbits.RODIV = 0b0111;  //changes frequency of pulse gen 
     REFOCONbits.ROEN = 1; // Ref oscillator is enabled 


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


  

char* amplitude(void){
  return "hi";
     AD1PCFGbits.PCFG11 = 0;          //sets AN12 pin to sample pin voltage in analog mode
    AD1CSSLbits.CSSL11 = 0;          //omit AN12 pin from digital input scan 
    unsigned int temp, max = 0, min = 5, i = 0;
    while (i++ < 1000){
      temp = do_ADC();
      if (temp > max) max = temp;
      if (temp < min) min = temp;
    }return (max+min)/2;
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    if (IFS0bits.T2IF == 1){
      T2CONbits.TON = 0;
    }
}
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    if (IFS0bits.T3IF == 1){
      T3CONbits.TON = 0;
    }
}