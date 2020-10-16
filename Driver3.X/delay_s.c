#include "delay_s.h"
#include <xc.h>

void delay_s (int t){
   
//16 - bit timer notes:
        //TxCON   -->   timer configuration register
        //IEC0 ; IFS0 ; IPC0    -->   Timer interrupt registers
        //PR2 is the number of half clk periods to wait before triggering an interrupt
        //Timer 2 Setup:
            //T2CON<3> set to 0
            //T2CON (timer2control) bit 15: 0 - stop 16bit timer ; 1 - start 16bit timer
            //T2CON bit 13: 0 - Continue module operation in idle mode ; 1 - discontinue """
            //      bit 6 ignored when TCS = 1
            //      bit 3 set to 0 to set Timer2 and Timer3 as two 16bit timers
        //Timer 2 Interrupt op:
            //IPC1 = 0xFF ->sets priority level to 7 on interrupt priority control register 1
            //IEC0 = IECO | 0x0080 -> set bit 7 to enable timer interrupt
            //IFS0 = IFS0 & 0xFF7F -> set bit 7 low to clear intrrupt flag in the case it was set previously
    
        //Timer 2 Interrupt subroutine
            /*void__attribute__((interrupt,no_auto_psv))_T2Intrrupt(void)
             {
                IFSObits.T2IF = 0; //cleared timer2 interrupt flag
                T2CONbits.TON = 0; //stopped 16bit timer2
                //TMR2flag = 1;    //global variable created by user - for code debugging
             }
            */
        
        //Steps to set up the timer (SLIDES 12-13 of 16bit timer notes!)
          /*    1. select appropriate clock speed
           *    2. configure T2CON register as shown above - reference data sheet
           *    3. Clear TMR2 ****Is this just the coded flag value??????
           *    4. Configure Timer2 specific bits in IPC1 ; IEC0 ; IFS1
           *    5. Compute PR2 based on time delay (formula on slides 6)
           *    6. Either put program into idle (using the MACRO Idle()) or jump to another part of code
           *    7. Upon Interrupt, if MCU is in idle, it will exit, 
           *    8. Start timer2 interrupt subroutine
           * 
           *    CODE ORGANIZATION ON SLIDE 13 - pretty helpful ***************************
           */
//END 16bit timer notes
    
    
//START Delay Code
    //1: Clock set to 32kHz
    //2:        --timer2 settings
        T2CONbits.TON = 1;       // start 16bit timer2
        T2CONbits.TSIDL = 0;     // keep timer operation during idle
        T2CONbits.T32 = 0;       // 32-bit timer mode off - 16-bit timer mode on
        T2CONbits.TCKPS1 = 1;    // pre-scale bit1 = 1 
        T2CONbits.TCKPS0 = 0;    // pre-scale bit0 = 0   -- pre-scale set to 1:64
        T2CONbits.TCS = 0;       // use internal clock source for timer2
    //3:
        TMR2 = 0x0000;           // TMR2 starts counting at 0
    //4:        --  interrupt settings
        IPC1 = 0xFFFF;           // interrupt priority set to level 7
        IEC0bits.T2IE = 1;       // enable T2 interrupts
        IFS0bits.T2IF = 0;       // clear T2 interrupt flag - in case of previous flag set
    //5:        --  set PR2 to work with pre-scaler for 1s base delay multiplied by input argument
        PR2 = 250 * t;           //250 * pre-scaler(1:64) * 2Period = 1second 
        //--NOTE: PR2 is a register set to hold a timer maximum before triggering interrupt
    //6:        --  chose what to do whilst waiting for timer to get interrupted by PR2 value
        Idle();
    //7:        -- MCU should exit Idle upon a timer interrupt, i honestly am not sure about this
    //8:        -- perform timer2 subroutine
        IFS0bits.T2IF = 0;       // clear timer 2 interrupt flag
        T2CONbits.TON = 0;       // timer2 stopped
        //add a flag bit here if needed, for sharing timer2 interrupt with other code/functions
        
   
//END Delay Code     
}

