#include <xc.h>

void delay_s(uint8_t t){
    long int d;
    while (t--){ // loops an amount of times == seconds requested
        d =150000;    //150000 = 8 000 000 / 80 where 80 is the estimated amount of clock cycles per while loop iteration
        while (d-->0){} // ~ 1 second's worth of instructions
    }return;
}
void IOinit(void){
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital 
    TRISBbits.TRISB4 = 1;      // set RB4 to digi input
    TRISAbits.TRISA2 = 1;      // set RA2 to digi input
    TRISAbits.TRISA4 = 1;      // set RA4 to digi input
    TRISBbits.TRISB8 = 0;      // set RB8 to digi output for LED
    CNPU1bits.CN0PUE  = 1;       // pull up resistor for RA4/CN0
    CNPU1bits.CN1PUE  = 1;       // pull up resistor for RB4/CN1
	CNPU2bits.CN30PUE = 1;       // pull up resistor for RA2/CN30
	return;
}

void CheckPushButtons(void){
    
    if (PORTAbits.RA4 == 0){ //RA4 button switched to ground       
        	LATBbits.LATB8 = 1;             // LED output on
            delay_s(1);                     // 1 second delay 
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){}
                //will pause with LED on if 2 or more buttons pressed
        	LATBbits.LATB8 = 0;             // LED output off once again
            delay_s(1);                  
	} else
	if (PORTAbits.RA2 == 0){                // RA2 button switched to ground
        	LATBbits.LATB8 = 1;             // Turns on led connected to RB8
        	delay_s(3);                     // 3 second delay
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){}
                //will pause with LED on if 2 or more buttons pressed
        	LATBbits.LATB8 = 0;             // turns off led connected to RB8
            delay_s(3);      	
    } else
	if (PORTBbits.RB4 == 0){                // RB4 button switched to ground	
        	LATBbits.LATB8 = 1;             // Turns on led connected to RB8
        	delay_s(2);                     // 2 second delay
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){}
                //will pause with LED on if 2 or more buttons pressed
        	LATBbits.LATB8 = 0;             // turns off led connected to RB8
            delay_s(2);      
	} else{
            LATBbits.LATB8 = 0;//LED output off by default - no buttons pressed
            return;
    }
}


