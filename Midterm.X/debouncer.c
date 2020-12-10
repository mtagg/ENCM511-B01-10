#include "debouncer.h"
#include <xc.h>


void DBinit(void){ 

        //T1 INIT:   
        T1CONbits.TSIDL = 0;     // keep timer operation during idle
        T1CONbits.TCS = 0;       // use internal clock source for timer1
        T1CONbits.TON = 0;       // Start with T1 off
        IEC0bits.T1IE = 1;       // enable T1 interrupts
        IFS0bits.T1IF = 0;       // Interrupt Flag clear
        IPC0 = 0x5000;           // T2 interrupt priority set to level 5
        PR1 = 150;               // 10ms Debounce timer
}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    if (IFS0bits.T1IF == 1){
       IFS0bits.T1IF = 0;       // Timer2 Interrupt Flag clear   
    }
}