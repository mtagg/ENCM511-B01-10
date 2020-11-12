#include "IOs.h"

uint8_t MINS;
uint8_t SECS;
uint8_t RESET;
uint8_t PB3FLAG;
uint8_t ALARM;


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
    char *SUFFIX = "";              // default suffix, will not clear alarm message
    
        if (t == 'r'){
            MINS = 0;               
            SECS = 0;    
        }
        if(ALARM == 1){             // There has been an alarm set
            SUFFIX = "        ";    // suffix to write over the alarm
            ALARM = 0;              // reset ALARM flag
        }
        else if (t == 'a'){         // Alarm to be set
            SUFFIX = " --ALARM";    
        }
        Disp2String("\r ");
        Disp2Dec(MINS);
        Disp2String("m");
        Disp2String(" :");
        Disp2Dec(SECS);
        Disp2String("s"); 
        Disp2String(SUFFIX);
        Disp2String("\r");

}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    
    
    if (IFS1bits.CNIF == 1){ 
        IEC0bits.T2IE = 0;                       // disable T2 interrupts
        
            
        //Behavior when user wants to change the countdown settings:
            if (PB1 + PB2 < 2) {                 // PB1 OR PB2 pressed:
                T2CONbits.TON = 0;               // Stop T2 timer   
                TMR2 = 0;                        // restart T2 every new countdown
                IFS0bits.T2IF = 0;               // clear T2 interrupt  
                LED_OFF;                         // start countdown with LED off 
            }
            
        //Increment minutes - PB1 pressed/held:
            while (PB1 == 0  && MINS < 59){
                MINS++;
                setTime('m');
            } 
        //Increment seconds - PB2 pressed/held:
            while (PB2 == 0 && SECS < 59){
                SECS++;
                setTime('s');        
            }
        //Start the Reset Timer - PB3 pressed/held:
            if (PB3 == 0){
                PB3FLAG = 1;                    // set the PB3 flag for code below
                IFS0bits.T3IF = 0;              // clear T3 interrupt flag
                TMR3 = 0;                       // reset T3 to 0
                T3CONbits.TON = 1;              // start timer3 to check for 3s hold 
                RESET = 0;                      // Reset flag re-set
           }
        //PB3 released: 
            else if (PB3 == 1){                 // observing PB3 has been pressed and released
                T3CONbits.TON = 0;              // Timer 3 off any time PB3 is not pressed
                
                if (PB3FLAG == 1 && RESET == 0){          //held PB3 less than 3s 
                    PB3FLAG = 0;
                    
                    if (T2CONbits.TON == 0){         // Countdown currently disabled
                        IEC0bits.T2IE = 1;           // enable T2 interrupts
                        IFS0bits.T2IF = 0;           // clear T2 interrupt flag
                        T2CONbits.TON = 1;           // Start Countdown
                    }
                    else {                           //MCU currently counting down
                        T2CONbits.TON = 0;           //pause Countdown                  
                    }
                }         
            }
        IFS1bits.CNIF = 0;      //clear CN interrupt flag bit
        Nop();    
    }
}