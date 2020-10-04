/* 
 * File:   main.c
 * Author: mmmta
 *
 * Created on September 28, 2020, 2:46 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

/*
 * 
 */
int main(int argc, char** argv) {
    TRISBbits.TRISB8 = 0; // GPIO RB8 output
    TRISAbits.TRISA4 = 1; // GPIO RA4 input
    CNPU1bits.CN0PUE = 1; //Enable pull up resistor on RA4/CN0
                          //accomodates for a grounding push button  
    
    while(1){
        while (PORTAbits.RA4 == 0) LATBbits.RB8 = 1;
        LATBbits.RB8 = 0; //if button is not pushed
    }
    return (EXIT_SUCCESS);
}

