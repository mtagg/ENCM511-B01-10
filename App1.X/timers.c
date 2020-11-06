#include "timers.h"

extern uint8_t MINS;
extern uint8_t SECS;
extern char *Mbuff;
extern char *Sbuff;
extern char DispBuffer[20];

void T2Init (void){

    //Initialize Timers
    T2CONbits.T32 = 0;       // 16-bit timer mode on for T3 AND T2
            
        //T2 INIT:   
        T2CONbits.TSIDL = 0;     // keep timer operation during idle

        T2CONbits.TCS = 0;       // use internal clock source for timer2
        T2CONbits.TON = 0;       // Star with T2 off
        IEC0bits.T2IE = 1;       // enable T2 interrupts
        IFS0bits.T2IF = 0;       // Timer2 Interrupt Flag clear
        IPC1 = 0xD000;           // T2 interrupt priority set to level 5
        
        
        //T3 INIT:
        T3CONbits.TSIDL = 0;     // T3 timer may run in Idle
        T3CONbits.TCS = 0;       // T3 uses internal clock
        T3CONbits.TON = 0;       // Start with T3 off
        IEC0bits.T3IE = 1;       // Enable T3 interrupts
        IFS0bits.T3IF = 0;       // clear T3 interrupt flag
        IPC2 = IPC2 | 0x5;       //T3 interrupt priority 6
        PR3  =  48000;           // 3 seconds on 32kHz T2 -> PR2 = 48000 (0xBB80))

    return;
}

void countdown(void){

    return;
}



//Timer 2 Interrupt subroutine 
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void){
    // T2 interrupt used for count-down functionality
    // controls blinking LED & decrementing timer values
        if (IFS0bits.T2IF == 1){            // If timer interrupt occurs:
            IFS0bits.T2IF = 0;              // clear T2 interrupt flag to allow blinking
            TMR2 = 0;                       // restart T2
        }
}

void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void){
    if (IFS0bits.T3IF == 1){
        IFS0bits.T3IF = 0;
        MINS = 0;
        SECS = 0;
        Mbuff = itoa(MINS);
        Sbuff = itoa(SECS);
        //DispBuffer
        //Disp2String("\r" + Mbuff + "m : " + Sbuff + "s         ");
//        Disp2String("\r");
//        Disp2Dec(MINS);
//        Disp2String("m : ");
//        Disp2Dec(SECS);
//        Disp2String("s         "); //extra spaces to clear any previous alarm messages        
//       
        T3CONbits.TON = 0;         // stop T3
        TMR3 = 0;                  // reset T3 to 0
        
    }   
}
    

