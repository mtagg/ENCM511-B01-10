#include "xc.h"
#include "stdlib.h"
#include "string.h"
#include "IOs.h"
#include "timers.h"
#include "UART2.h"
#include "MACROS.h"



uint16_t MINS;
uint16_t SECS;
char Mbuff[2];
char Sbuff[2];

void IOinit(void){

//IO pin setup:    
    AD1PCFG = 0xFFFF;            // set all analog pins as digital 
    TRISBbits.TRISB4 = 1;        // set RB4 to digi input
    TRISAbits.TRISA2 = 1;        // set RA2 to digi input
    TRISAbits.TRISA4 = 1;        // set RA4 to digi input
    TRISBbits.TRISB8 = 0;        // set RB8 to digi output for LED
    CNPU1bits.CN0PUE  = 1;       // pull up resistor for RA4/CN0 
    CNPU1bits.CN1PUE  = 1;       // pull up resistor for RB4/CN1
    CNPU2bits.CN30PUE = 1;       // pull up resistor for RA2/CN30

//CN Registers / Interrupts Setup:
    CNEN1bits.CN0IE  = 1;         // RA4 enable CN
    CNEN1bits.CN1IE  = 1;         // RB4 enable CN             
    CNEN2bits.CN30IE = 1;         // RA2 enable CN 
    IPC4 = IPC4 | 0xF000;         // CN interrupts set to priority 7
    IEC1bits.CNIE = 1;            // CN interrupt requests enabled
    IFS1bits.CNIF = 0;            // clear T2 interrupt flag - because you never know?
    
    return; 
}



void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    
    
    if (IFS1bits.CNIF == 1){ 
        IFS1bits.CNIF = 0;      //clear CN interrupt flag bit
        Nop();
                
        while(PB1 == 0 && PB2+PB3 == 2 && MINS < 59){
            MINS++;
            itoa(MINS, Mbuff, 10);
            itoa(SECS, Sbuff, 10);
            Disp2String("\r" + Mbuff + "m : " + Sbuff + "s         ");
//            Disp2String("\r");
//            Disp2Dec(MINS);
//            Disp2String("m : ");
//            Disp2Dec(SECS);
//            Disp2String("s         "); //extra spaces to clear any previous alarm messages

        }
        while(PB2 == 0 && PB1+PB3 == 2 && SECS < 59){
            SECS++;
            itoa(MINS, Mbuff, 10);
            itoa(SECS, Sbuff, 10);
            Disp2String("\r" + Mbuff + "m : " + Sbuff + "s         ");
//            Disp2String("\r");
//            Disp2Dec(MINS);
//            Disp2String("m : ");
//            Disp2Dec(SECS);
//            Disp2String("s         "); //extra spaces to clear any previous alarm messages
        
        }
        if (PB3 == 0){
            T2CONbits.TON = ~T2CONbits.TON; // toggles timer 2
            IFS0bits.T3IF = 0;              // clear T3 interrupt flag
            TMR3 = 0;                       // reset T3 to 0
            T3CONbits.TON = 1;              // start timer3, basically checking to see if its held for > 3s 
        }else if (PB3 == 1){           
            T3CONbits.TON = 0;              // Stop T3 to avoid clearing
        }
                 
    }
}