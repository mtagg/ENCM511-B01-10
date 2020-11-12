#include "timers.h"

extern uint8_t MINS;
extern uint8_t SECS;
extern uint8_t RESET;
extern uint8_t SET;

void T2Init (void){
    
    //Initialize Timer Display:
    setTime('r');
    
    //Initialize Timers
    T2CONbits.T32 = 0;       // 16-bit timer mode on for T3 AND T2
            
        //T2 INIT:   
        T2CONbits.TSIDL = 0;     // keep timer operation during idle

        T2CONbits.TCS = 0;       // use internal clock source for timer2
        T2CONbits.TON = 0;       // Star with T2 off
        IEC0bits.T2IE = 1;       // enable T2 interrupts
        IFS0bits.T2IF = 0;       // Timer2 Interrupt Flag clear
        IPC1 = 0xD000;           // T2 interrupt priority set to level 5
        PR2 = 16000;
        
        //T3 INIT:
        T3CONbits.TSIDL = 0;     // T3 timer may run in Idle
        T3CONbits.TCS = 0;       // T3 uses internal clock
        T3CONbits.TON = 0;       // Start with T3 off
        IEC0bits.T3IE = 1;       // Enable T3 interrupts
        IFS0bits.T3IF = 0;       // clear T3 interrupt flag
        IPC2 = IPC2 | 0x5;       //T3 interrupt priority 6
        PR3  =  48000;           // 3 seconds on 32kHz T2 -> PR2 = 48000 (0xBB80))
        RESET = 0;
        
    return;
}



//Timer 2 Interrupt subroutine 
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void){
        if (IFS0bits.T2IF == 1){            // If timer interrupt occurs:
            LED_TOGGLE;   
            TMR2 = 0;                       // restart T2
            if (SECS == 0 && MINS == 0 && RESET == 0){
                LED_ON;
                setTime('a');
                T2CONbits.TON = 0;
            }else if(SECS > 0){
                SECS--;
                setTime('s');
            }else if (MINS > 0){
                MINS--;
                SECS = 59;
                setTime('m');
            }
        }
        IFS0bits.T2IF = 0;              // clear T2 interrupt flag to continue countdown 
}



//Timer 3 Interrupt subroutine -- used for reset behavior
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void){
    if (IFS0bits.T3IF == 1){        
            IFS0bits.T3IF = 0;         // reset T3 Flag     
            T3CONbits.TON = 0;         // stop T3
            T2CONbits.TON = 0;         // stop T2 
            IEC0bits.T2IE = 0;         // stop T2 interrupts 
            TMR2 = 0;                  
            LED_OFF;
            setTime('r');              // reset time display
            RESET = 1;                 // set "RESET" flag 

    }   
}
    

