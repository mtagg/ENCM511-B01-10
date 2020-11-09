#include "IOs.h"

uint8_t MINS;
uint8_t SECS;
uint8_t Increment;
uint8_t RESET;
uint8_t SET;


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

void setTime(char t){
        if (t == 'r'){
            MINS = 0;
            SECS = 0;
        }    
        Disp2String("\r ");
        Disp2Dec(MINS);
        Disp2String("m");
        Disp2String(" : ");
        Disp2Dec(SECS);
        Disp2String("s        "); 

}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    
    
    if (IFS1bits.CNIF == 1){ 
            IFS1bits.CNIF = 0;      //clear CN interrupt flag bit
            
        //Increment minutes
            while (PB1 == 0  && MINS < 59){
                MINS++;
                setTime('s');
            } 
        //Increment seconds
            while (PB2 == 0 && SECS < 59){
                SECS++;
                setTime('s');        
            }
        //Start Reset Timer
            if (PB3 == 0){
                SET = 1;                        // PB3 pressed flag
                IFS0bits.T3IF = 0;              // clear T3 interrupt flag
                TMR3 = 0;                       // reset T3 to 0
                T3CONbits.TON = 1;              // start timer3, basically checking to see if its held for > 3s 
                RESET = 0;
           }
        //Countdown Start/Stop & Stop Reset Timer 
            else if (PB3 == 1){    
                T3CONbits.TON = 0;              // Stop T3 to avoid reset
                if (RESET == 1){ 
                        RESET = 0;              // If reset occurs, reset the reset flag :p
                }   
                else if (SET == 1){                 //PB3 flag was previously SET
                    
                    if (T2CONbits.TON == 1){        // Countdown on
                        LED_OFF;                    //  
                        T2CONbits.TON == 0;         // Stop Countdown 
                    }
                    else if (T2CONbits.TON == 0){   // Countdown off
                        LED_ON;                     //
                        TMR2 = 0;                   // reset T2 timer
                        T2CONbits.TON = 1;           // Start Countdown
                    }
                }          
            }    
    }Nop();        
}