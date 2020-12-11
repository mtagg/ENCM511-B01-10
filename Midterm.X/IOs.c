#include "IOs.h"
unsigned int STATE;
unsigned int count;
unsigned int freq;

void IOinit(void){

//IO pin setup:    
    AD1PCFG = 0xFFFF;            // set all analog pins as digital 
    TRISBbits.TRISB4 = 1;        // set RB4 to digi input
    TRISAbits.TRISA2 = 1;        // set RA2 to digi input
    TRISAbits.TRISA4 = 1;        // set RA4 to digi input
    CNPU1bits.CN0PUE  = 1;       // pull up resistor for RA4/CN0 
    CNPU1bits.CN1PUE  = 1;       // pull up resistor for RB4/CN1
    CNPU2bits.CN30PUE = 1;       // pull up resistor for RA2/CN30

//CN Registers / Interrupts Setup:
    CNEN1bits.CN0IE  = 1;         // RA4 enable CN
    CNEN1bits.CN1IE  = 1;         // RB4 enable CN             
    CNEN2bits.CN30IE = 1;         // RA2 enable CN 

    IPC4 = 0x7000;                // CN interrupts set to priority 7
    IEC1bits.CNIE = 1;            // CN interrupt requests enabled 
return; 
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    
    if (IFS1bits.CNIF == 1){ 
    //frequency calculation conditions
        if (count<16){ 
            count++;
            }

    //normal operation
        else{ 
            IFS0bits.T2IF = 1;
            T2CONbits.TON = 0;
            //debounce PBs
            // TMR1 = 0;                // reset debounce counter   
            // T1CONbits.TON = 1;       // Start debounce timer   
            // while (IFS0bits.T1IF == 0){} //wait for debounce delay
            // IFS0bits.T1IF = 0;       // Timer2 Interrupt Flag clear   

            //PB1 pressed
            if      (PB1 == 0 && PB2 == 1 && PB3 == 1){
                STATE = 1;
            }       
            //PB2 pressed
            else if (PB1 == 1 && PB2 == 0 && PB3 == 1){
                STATE = 2;
            }     
            //PB3 pressed
            else if (PB1 == 1 && PB2 == 1 && PB3 == 0){
                STATE = 3;  
            } 
        }    
    }
    IFS1bits.CNIF = 0;      //clear CN interrupt flag bit
    //Nop();
    return;
}