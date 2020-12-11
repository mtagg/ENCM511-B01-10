#include "pulse.h"
extern unsigned int STATE;
extern unsigned int freq;
extern unsigned int count;

void TimerInit(void){
    //Initialize Timers
    T2CONbits.T32 = 0;           //16bit timer t2
    
    //T2 setup:
    T2CONbits.TSIDL = 1;    //idle operation enabled
    T2CONbits.TON = 0;      //timer off initially
    T2CONbits.TCS = 0;      //base clock
    IFS0bits.T2IF = 0;      //reset flag bit
    // IEC0bits.T2IE = 1;      //enable t2 interrupts
    
    // IPC1 = 0x7000;        //t2 priority level
    // PR2 = 32768;             //frequency testing sample period 

    count = 0;
    TMR2 = 0;

    //RB12 (pin15) setup:
    TRISBbits.TRISB12 = 1;        // set RB12 to digi input
    //start t2 and start CN interrupts to measure frequency:
    CNEN1bits.CN14IE = 1;         //allow pin15 CN int enable
    T2CONbits.TON = 1;            //start t2
//this code is where im stumped,
// probablyt best to start fresh, my method is not working
    while(count<16)Idle();   
//the idea was to cound CN interrupts in idle until count incremented to 16, 
//then compare to TMR2  to get the ratio of clock speeds     
   

    CNEN1bits.CN14IE = 0; //disable pin15 CN interrupts
    T2CONbits.TON = 0;    //disable t2  
    return;
}

char* frequency(void){
    //double calibrate = (74);     //constant to account for counting delays
                            //determined through experimentation
                            //multiplied by 4 to adjust for TMR2 increment schedule
    //check to see base clock
    int base = 32;                                  // default osc value is 32khz
    if      (OSCCONbits.COSC == 0x0) base = 8000;   // current oscillator is 8MHz
    else if (OSCCONbits.COSC == 0x6) base = 500;    // current osc is 500kHz

    static char buff[10];
//if TMR2 and count were incremented proportional to their clock speeds,
//then this code should work
    double f =  (base*count)/(4*TMR2);//multiply TMR2 by 4 to account for built in delays
    sprintf(buff, "%.0f", f);
    return buff;
}

char* amplitude(void){
    return "TEST";
//commented out for debugging
//still need to adjust how buff[] recieves its string but that wont be an issue
    // unsigned int max = 0x0;
    // unsigned int min = 0x3ff;
    // unsigned int ctr = 0;
    // unsigned int tmp = 0;
    // while (ctr++ < 500){             //# of ADC samples
    // tmp = do_ADC();                  //update sample @ pin 15
    // if (tmp < min) min = tmp;
    // if (tmp > max) max = tmp;
    // }
    // double amp;
    // amp = (3.25*(max+min));
    // amp/=2048;
    // static char buff[10];
    // sprintf(buff, "%1.2f V", 1.67);
    // return buff;
}
// void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
//     if (IFS0bits.T2IF == 1){
//         CNEN1bits.CN14IE = 0;       //disable pin15 interrupts
//         T2CONbits.TON = 0;          //Stop T2
//         IFS0bits.T2IF = 0;          //reset IF
//         // while(1){
//         // Disp2String("\rHIHI");
//         // Disp2Dec(TMR2);
//         // Disp2Dec(count);
//         // }
//         return;
//     }
// }
// void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
//     if (IFS0bits.T3IF == 1){
//         T3CONbits.TON = 0;  //Stop T3
//         IFS0bits.T3IF = 0;  //reset IF

//     }
// }