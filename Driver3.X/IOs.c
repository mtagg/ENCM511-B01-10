#include <xc.h>
#include "IOs.h"
#include "delay_s.h"

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
    
    while (PORTAbits.RA4 == 0){                           //RA4 button switched to ground 
        	LATBbits.LATB8 = ~LATBbits.LATB8;             // LED output toggle
            delay_s(1);                                   // 1 second delay 
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){
                        LATBbits.LATB8 = 1;  
                }//pause with LED on while 2 or more buttons pressed
    }   
    while (PORTAbits.RA2 == 0){                           // RA2 button switched to ground
        	LATBbits.LATB8 = ~LATBbits.LATB8;             // LED output toggle
        	delay_s(3);                                   // 3 second delay
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){
                        LATBbits.LATB8 = 1;  
                }//pause with LED on while 2 or more buttons pressed   
    } 
	while (PORTBbits.RB4 == 0){                           // RB4 button switched to ground	
        	LATBbits.LATB8 = ~LATBbits.LATB8;             // LED output toggle
        	delay_s(2);                                   // 2 second delay
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){
                        LATBbits.LATB8 = 1;  
                }//pause with LED on while 2 or more buttons pressed
    } 
    LATBbits.LATB8 = 0;//LED output off by default - no buttons pressed
    return;
     
}