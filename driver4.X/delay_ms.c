#include <xc.h>
#include "delay_ms.h"
#include "MACROS.h"
//#include "IOs.h"
//#include "ChangeClk.h"
#include "UART2.h"

unsigned int StopBlink;


void T2Init (void){

    //Initialize Timer 
        T2CONbits.TSIDL = 0;     // keep timer operation during idle
        T2CONbits.T32 = 0;       // 32-bit timer mode off - 16-bit timer mode on
        T2CONbits.TCS = 0;       // use internal clock source for timer2
        T2CONbits.TON = 0;       // Star with T2 off
        IEC0bits.T2IE = 1;       // enable T2 interrupts
        IFS0bits.T2IF = 0;       // Timer2 Interrupt Flag clear
        IPC1 = 0xE000;           // interrupt priority set to level 6
    //Timer Pre-Scaler Setup:
        //T2CONbits.TCKPS1 = 0;           //pre-scaler enabled to 1:1 to use 32kHz clock
        //T2CONbits.TCKPS0 = 0;
    return;
}
void delay_ms (int t){
    
    
//T2 Timer-Delay Setup   
    //LED_ON;                             //Start Blinking Behavior with LED on
    //NewClk(32);                         // Timer clock-speed     
    PR2 = 16*t;                         // 1:1 Pre-scaler * 16 * 2 * period = 1second  
    
    TMR2 = 0x1F3F;                      // offsets delays from Disp2String in 32kHz mode
    IFS0bits.T2IF = 0;                  // clear T2 Interrupt Flag
    T2CONbits.TON = 1;                  // start 16bit timer2
   
//T2 Timer-Delay Loop    
        while(IFS0bits.T2IF == 0){   // keep pulsing delay as long as CN interrupt does not occur             
            Idle();                  // idle whilst waiting for timer to get interrupted by PR2 value
    }   
//T2 Timer-Delay Wrap-up             
    IFS0bits.T2IF = 0;           //reset T2 IF
    LED_OFF;                     //default to LED_OFF, awaiting next interrupt
  
    return;
}


//Timer 2 Interrupt subroutine 
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void){
    if  (StopBlink == 1) { 
        IFS0bits.T2IF = 1;           //send flag to stop blink loop   
        T2CONbits.TON = 0;           //Stop T2 Timer
    }else
        if (IFS0bits.T2IF == 1){            // If timer interrupt occurs:
            IFS0bits.T2IF = 0;              // clear T2 interrupt flag to allow blinking
            LED_TOGGLE;                     // toggle LED when 1 button still held down
            TMR2 = 0x0;                     // reset timer
        }
}
    

