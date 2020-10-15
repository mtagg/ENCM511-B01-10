#include "xc.h"
#include "IOs.h"

void delay_ms (uint8_t t){

    //16 - bit timer notes:
        //TxCON   -->   timer configuration register
        //IEC0 ; IFS0 ; IPC0    -->   Timer interrupt registers
        //PR2 is the number of half clk periods to wait before triggering an interrupt
        //Timer 2 Setup:
            //T2CON<3> set to 0
            //T2CON (timer2control) bit 15: 0 - stop 16bit timer ; 1 - start 16bit timer
            //T2CON bit 13: 0 - Continue module operation in idle mode ; 1 - discontinue """
            //      bit 6 ignored when TCS = 1
            //      bit 3 set to 0 to set Timer2 and Timer3 as two 16bit timers
        //Timer 2 Interrupt op:
            //IPC1 = 0xFF ->sets priority level to 7 on interrupt priority control register 1
            //IEC0 = IECO | 0x0080 -> set bit 7 to enable timer interrupt
            //IFS0 = IFS0 & 0xFF7F -> set bit 7 low to clear intrrupt flag in the case it was set previously
    
        //Timer 2 Interrupt subroutine
            /*void__attribute__((interrupt,no_auto_psv))_T2Intrrupt(void)
             {
                IFSObits.T2IF = 0; //cleared timer2 interrupt flag
                T2CONbits.TON = 0; //stopped 16bit timer2
                //TMR2flag = 1;    //global variable created by user - for code debugging
             }
            */
        
        //Steps to set up the timer (SLIDES 12-13 of 16bit timer notes!)
          /*    1. select appropriate clock speed
           *    2. configure T2CON register as shown above - reference data sheet
           *    3. Clear TMR2 ****Is this just the coded flag value??????
           *    4. Configure Timer2 specific bits in IPC1 ; IEC0 ; IFS1
           *    5. Compute PR2 based on time delay (formula on slides 6)
           *    6. Either put program into idle (using the MACRO Idle()) or jump to another part of code
           *    7. Upon Interrupt, if MCU is in idle, exit, and jump to timer2 interrupt routine.
           *    8. Stop timer
           * 
           *    CODE ORGANIZATION ON SLIDE 13 - pretty helpful ***************************
           */
       
}

void IOinit(void){
    
    //same as driver 2 setup
    
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

void CheckPushButtons(void){
    
    //same as driver2 setup, albeit changing delay_s(); to delay_ms(); and appropriate arguments  
    
    if (PORTAbits.RA4 == 0){ //RA4 button switched to ground       
        	LATBbits.LATB8 = 1;             // LED output on
            delay_ms(1000);                     // 1 second delay 
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){}
                //will pause with LED on if 2 or more buttons pressed
        	LATBbits.LATB8 = 0;             // LED output off once again
            delay_ms(1000);                  
	} else
	if (PORTAbits.RA2 == 0){                // RA2 button switched to ground
        	LATBbits.LATB8 = 1;             // Turns on led connected to RB8
        	delay_ms(3000);                     // 3 second delay
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){}
                //will pause with LED on if 2 or more buttons pressed
        	LATBbits.LATB8 = 0;             // turns off led connected to RB8
            delay_ms(3000);      	
    } else
	if (PORTBbits.RB4 == 0){                // RB4 button switched to ground	
        	LATBbits.LATB8 = 1;             // Turns on led connected to RB8
        	delay_ms(2000);                     // 2 second delay
                while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2){}
                //will pause with LED on if 2 or more buttons pressed
        	LATBbits.LATB8 = 0;             // turns off led connected to RB8
            delay_ms(2000);      
	} else{
            LATBbits.LATB8 = 0;//LED output off by default - no buttons pressed
            return;
    }
}


