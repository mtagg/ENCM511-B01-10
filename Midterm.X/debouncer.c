    #include "debouncer.h"
#include <xc.h>


void T1init(void){ 

        //T2 INIT:   
        T1CONbits.TSIDL = 0;     // keep timer operation during idle
        T1CONbits.TCS = 0;       // use internal clock source for timer1
        T1CONbits.TON = 0;       // Start with T1 off
        IEC0bits.T1IE = 1;       // enable T2 interrupts
        IFS0bits.T1IF = 0;       // Timer1 Interrupt Flag clear
        IPC0 = IPC0 | 0x6000;    // T1 interrupt priority set to level 6
        PR1 = 1000;               // Debounce timer
}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    if (IFS0bits.T1IF == 1){
       T1CONbits.TON = 0;   //turn T1 debounce timer off
    }
}