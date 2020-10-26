#include "delay_ms.h"
#include <xc.h>

void T2Init (void){
    //setting T2CON register bit that wont change after initialization
        T2CONbits.TSIDL = 0;     // keep timer operation during idle
        T2CONbits.T32 = 0;       // 32-bit timer mode off - 16-bit timer mode on
        T2CONbits.TCS = 0;       // use internal clock source for timer2
        return;
}
void delay_ms (int t){

    //1: Clock previously set to 32kHz
    //2:        --  timer2 settings
        T2CONbits.TON = 1;       // start 16bit timer2
    //3:
        
        TMR2 = 0x0000;           // TMR2 starts counting at 0
    //4:        --  interrupt settings
        IPC1 = 0xFFFF;           // interrupt priority set to level 7
        IEC0bits.T2IE = 1;       // enable T2 interrupts
        IFS0bits.T2IF = 0;       // clear T2 interrupt flag - in case of previous flag set
    //5:        --  set PR2 to work with pre-scaler for 1s base delay multiplied by input argument
        PR2 = 16*t;           //16000 * 2Period = 1second 
    //6:        --  chose what to do whilst waiting for timer to get interrupted by PR2 value
        Idle();
    //7:        -- MCU exits Idle after PR2 == TMR2 and Performs timer2 interrupt sub-routine
    return;
}

//Timer 2 Interrupt subroutine  --  function header format allows auto-interrupt sub-routine for 24F16KA101
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void){
    T2CONbits.TON = 0; //stopped 16bit timer2
    IFS0bits.T2IF = 0; //cleared timer2 interrupt flag
    //TMR2flag = 1;  //global variable created by user - for code debugging
}