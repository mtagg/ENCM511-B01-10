#define  FCY    8000000UL    // Instruction cycle frequency, Hz - required for __delayXXX() to work
#include <libpic30.h>

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


void IOinit(void){
	TRISBbits.TRISB8 = 0; //make GPIO RB8 as a digital Output
	TRISBbits.TRISB4 = 1; //make GPIO RB4 as a digital input
	TRISAbits.TRISA2 = 1; //make GPIO RA2 as a digital input
	TRISAbits.TRISA4 = 1; //make GPIO RA4 as a digital input
	CNPU2bits.CN30PUE = 1; //enables pull up resistor on RA2/CN30
	CNPU1bits.CN0PUE = 1; //enables pull up resistor on RB4/CN1
	CNPU1bits.CN1PUE = 1; //enables pull up resistor on RA4/CN0

	return;
}

void IOcheck(){
    LATBbits.LATB8 = 0; // if no button is pressed turn off LED connected to RB8
	while(PORTAbits.RA4 == 0) //while RA4 pb is pressed
	{
		LATBbits.LATB8 = 1; //Turns on led connected to RB8
		 __delay_ms(1000); //waits 1 second
		 LATBbits.LATB8 = 0; // Turns off led connected to RB8

	}

	while(PORTAbits.RA2 == 0) //while RA2 pb is pressed
	{
		LATBbits.LATB8 = 1; //Turns on led connected to RB8
		 __delay_ms(2000); // waits 2 seconds
		 LATBbits.LATB8 = 0; // turns off led connected to RB8

	}

	while(PORTBbits.RB4 == 0) //while RB4 pb is pressed
	{
		LATBbits.LATB8 = 1; //Turns on led connected to RB8
		 __delay_ms(3000); // waits 3 seconds
		 LATBbits.LATB8 = 0; // turns off led connected to RB8

	}
	while((PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)|| (PORTAbits.RA4 == 0 && PORTAbits.RA2== 0) || (PORTBbits.RB4 == 0 && PORTAbits.RA2 == 0) ||(PORTAbits.RA2==0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0))
		//while at least 2 or 3 buttons are pressed
	{

		LATBbits.LATB8 = 1; //Turns on led connected to RB8

	}




}


int main(void){
	IOinit();
	while(1){
		IOcheck();
	}
    return 0;
}
