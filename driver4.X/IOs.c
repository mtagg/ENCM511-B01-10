#include <xc.h>
#include "IOs.h"
#include "delay_ms.h"
#include "UART2.h"
// MACRO REFERENCE:
// LED_ON:           {LATBbits.LATB8 = 1;}    
// LED_OFF:          {LATBbits.LATB8 = 0;}
// LED_TOGGLE:       {LATBbits.LATB8 = ~LATBbits.LATB8;}
// PB1:              PORTAbits.RA4 (FOR THE 0.5S DELAY)
// PB2:              PORTBbits.RB4 (FOR THE 2S DELAY)
// PB3:              PORTAbits.RA2 (FOR THE 3S DELAY)
// ALL3BUTTONS:      PORTAbits.RA4 + PORTBbits.RB4 + PORTAbits.RA2
void IOinit(void){
    
    AD1PCFG = 0xFFFF;            // set all analog pins as digital 
    TRISBbits.TRISB4 = 1;        // set RB4 to digi input
    TRISAbits.TRISA2 = 1;        // set RA2 to digi input
    TRISAbits.TRISA4 = 1;        // set RA4 to digi input
    TRISBbits.TRISB8 = 0;        // set RB8 to digi output for LED
    CNPU1bits.CN0PUE  = 1;       // pull up resistor for RA4/CN0
    CNPU1bits.CN1PUE  = 1;       // pull up resistor for RB4/CN1
	CNPU2bits.CN30PUE = 1;       // pull up resistor for RA2/CN30
	return; 
}
void IOcheck(void){
    
    while (PB1==0 && PB2==1 && PB3==1){         //RA4 button switched to ground 
            Disp2String("\n\rPB1 is pressed");
            LED_TOGGLE;
            delay_ms(500);
    }   
    while (PB1==1 && PB2==0 && PB3==1){                           // RB4 button switched to ground	
            Disp2String("\n\rPB2 is pressed");
        	LED_TOGGLE;
            delay_ms(2000);            
    }
    while (PB1==1 && PB2==1 && PB3==0){                           // RA2 button switched to ground
            Disp2String("\n\rPB3 is pressed");
        	LED_TOGGLE; 
            delay_ms(3000);              
    } 
    while (PB1==0 && PB2==0 && PB3==1){                           // RA4 and RB4 switched to ground
            Disp2String("\n\rPB1 and PB2 are pressed");
            LED_ON;
    }
    while (PB1==0 && PB2==1 && PB3==0){                           // RA4 and RA2 switched to ground                  
            Disp2String("\n\rPB1 and PB3 are pressed");             
            LED_ON;
    }
    while (PB1==1 && PB2==0 && PB3==0){                           // RB4 and RA2 switched to ground  
            Disp2String("\n\rPB2 and PB3 are pressed");             
            LED_ON;
    }
    while (ALL3BUTTONS == 0){                                     //all pressed - ground switched x 3 
            Disp2String("\n\rAll PBs pressed");
            LED_ON;
    }
    while (ALL3BUTTONS == 3){                                     //all un-pressed - normally high x 3 
            Disp2String("\n\rNothing pressed");
            LED_OFF;
    }
    return;
     
}