    #include "debouncer.h"
#include <xc.h>


void T2Init(void){ 
    //Initialize Timer
    T2CONbits.T32 = 0;           // 16-bit timer mode on for T3 AND T2

        //T2 INIT:   
        T2CONbits.TSIDL = 0;     // keep timer operation during idle
        T2CONbits.TCS = 0;       // use internal clock source for timer2
        T2CONbits.TON = 0;       // Start with T2 off
        IEC0bits.T2IE = 1;       // enable T2 interrupts
        IFS0bits.T2IF = 0;       // Timer2 Interrupt Flag clear
        IPC1 = 0xE000;           // T2 interrupt priority set to level 6
        PR2 = 150;               // 10ms Debounce timer
}
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    if (IFS0bits.T2IF == 1){
       IFS0bits.T2IF = 0;   //reset T2 debounce flag
       T2CONbits.TON = 0;   //turn T2 debounce timer off
    }
}