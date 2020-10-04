#include <xc.h>

void delay_s(int t){
    long int d = t *(800000/3); 
    //1 loop iteration =~3 clk cycles--8MHz processor.
    while (d>0){d--;}
    return;
}
void IOinit(void){
    TRISB = 0x0010;        // set RB4 to digi input, rest RB output (RB8 for LED)
    TRISA = 0x0014;        // set RA2 and RA4 to digi input, rest output ()
	CNPU2bits.CN30PUE = 1; // pull up resistor for RA2/CN30
	CNPU1bits.CN0PUE  = 1; // pull up resistor for RB4/CN1
	CNPU1bits.CN1PUE  = 1; // pull up resistor for RA4/CN0
	return;
}

void CheckPushButtons(void){
    LATBbits.LATB8 = 0; //LED output off by default - no buttons pressed
    while(PORTAbits.RA4 == 0){ //RA4 button switched to ground       
        	LATBbits.LATB8 = 1;             // LED output on
            delay_s(1);                     // 1 second delay 
        	LATBbits.LATB8 = 0;             // LED output off once again
	}
	while(PORTAbits.RA2 == 0){              // RA2 button switched to ground
        	LATBbits.LATB8 = 1;             // Turns on led connected to RB8
        	delay_s(2);                     // 2 second delay
        	LATBbits.LATB8 = 0;             // turns off led connected to RB8
	}  
	while(PORTBbits.RB4 == 0){              // RB4 button switched to ground	
        	LATBbits.LATB8 = 1;             // Turns on led connected to RB8
        	delay_s(3);                     // 3 second delay
        	LATBbits.LATB8 = 0;             // turns off led connected to RB8
	}
	while( (PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){ //2 or 3 buttons pressed 
        	LATBbits.LATB8 = 1; //Turns on led connected to RB8
	}
    return;
}


