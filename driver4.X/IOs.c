#include "xc.h"
#include "IOs.h"
#include "delay_ms.h"
#include "UART2.h"
#include "MACROS.h"

extern unsigned int StopBlink;

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
        StopBlink = 1;
        IFS1bits.CNIF = 0;      //clear CN interrupt flag bit
        Nop();
        

    //No buttons pressed:
        if      (ALL3BUTTONS == 3){
            LED_OFF;
            Disp2String("\n\rNothing pressed");         
        }
    //three buttons pressed:    
        else if (ALL3BUTTONS == 0){                     
            LED_ON;
            Disp2String("\n\rAll PBs Pressed");

        }
    //two buttons pressed:   
        else if (ALL3BUTTONS == 1){                     
            LED_ON;
            if      (PB1 + PB2 == 0) Disp2String("\n\rPB1 and PB2 are pressed");
            else if (PB1 + PB3 == 0) Disp2String("\n\rPB1 and PB3 are pressed");
            else if (PB2 + PB3 == 0) Disp2String("\n\rPB2 and PB3 are pressed");
        }
    //only one button pressed:
        else if (ALL3BUTTONS == 2){  
        //Blink prep:
            StopBlink = 0;      // reset StopBlink
            LED_ON;               
        //Blink Behavior:           
            if      (PB1 == 0){
                //PR2 = 16*500;
                Disp2String("\n\rPB1 is pressed");
                delay_ms(500);            
            }
            else if (PB2 == 0){ 
                //PR2 = 16*2000;
                Disp2String("\n\rPB2 is pressed");
                delay_ms(2000);            
            }
            else if (PB3 == 0){
                //PR2 = 16*3000;
                Disp2String("\n\rPB3 is pressed"); 
                delay_ms(3000);            
            }               
        }              
    }
}