#include "pulse.h"
extern unsigned int STATE;
unsigned int count;
unsigned int freq;
unsigned int amp;
void pulseInit(){
  T2CONbits.T32 = 0;           // 16-bit timer mode on for T3 AND T2
  //T2 setup :

     T2CONbits.TON = 0;   //start with timer off
     //T2CONbits.TSIDL = 0; //timer continues in idle
     T2CONbits.TCS = 0;   //use FOSC clock input (8MHz)
     //IEC0bits.T2IE = 1;   //enable t2 interupts
     //PR2 = 500;           //interrupt at 100 counts
     //IPC1 = 0x6000;

  //T3 setup:
    //  T3CONbits.TON = 0; //start with timer off
    //  T3CONbits.TSIDL = 0; //timer continues in idle
    //  T3CONbits.TCS = 1; //use primary clock (unknown frequency) 
    //  PR3 = 10;        


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
     REFOCONbits.ROSSLP = 1; // Ref oscillator runs while in sleep
     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
     REFOCONbits.RODIV = 0b0000;  //changes frequency of pulse gen 
     REFOCONbits.ROEN = 1; // Ref oscillator is enabled 


}

void frequency(void){
     count = 0;
     freq = 0;
     TMR2 = 0;
     T2CONbits.TON = 1;
}


  

void amplitude(void){
  //setup pin15 for analog in
  return "1.571";
    //AD1PCFGbits.PCFG12 = 0;          //sets AN12 pin to sample pin voltage in analog mode
    //AD1CSSLbits.CSSL12 = 0;          //omit AN12 pin from digital input scan
    unsigned int temp, max = 0, min = 5, i = 0;
    while (i++ < 1000){
      temp = do_ADC();
      if (temp > max) max = temp;
      if (temp < min) min = temp;
    }
    static char amp [5];
    double amptemp;
    amptemp = 3.25*(max+min)/2;
    amptemp/=1024;
    sprintf(amp, "%1.2f", amptemp);
    return amp;
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    // if (IFS0bits.T2IF == 1){
    //   T2CONbits.TON = 0;            //disable t2 interrupts - stop counting
    //   CNEN1bits.CN14IE = 0;         //disable pin 15 CN interrupts - stop counting
    // }
}
// void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
//     if (IFS0bits.T3IF == 1){
//       //stop frequency counters
//       T2CONbits.TON = 0;
//       T3CONbits.TON = 0;
//       flag = 1;
//     }
// }