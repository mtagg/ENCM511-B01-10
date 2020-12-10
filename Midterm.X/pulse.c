#include "pulse.h"
extern unsigned int STATE;
extern unsigned int freq;
extern unsigned int amp;

void TimerInit(void){
    //Initialize Timers
    T2CONbits.T32 = 0;           // 16-bit timer mode on for T3 AND T2
    
    //T2 setup:
    TMR2 = 0;
    T2CONbits.TCS = 1; //ext clock - pin18 pulse??
    IEC0bits.T2IE = 0; //disable t2 interrupts
    IFS0bits.T2IF = 0;
    IPC1 = 0x6000;    //t2 priority level
    PR2 = 400;

    //T3 setup:
    TMR3 = 0;
    T3CONbits.TCS = 0; //primary clock - 8MHz??
    IEC0bits.T3IE = 1; //enable T3 ISR
    IFS0bits.T3IF = 0;
    IPC2 = 0x0006;     //t3 priority level
    PR3 = 400;


    freq = 0;
    T3CONbits.TON = 1;
    T2CONbits.TON = 1;
    return;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    T3CONbits.TON = 0;
    T2CONbits.TON = 0;
    freq = 400;
}