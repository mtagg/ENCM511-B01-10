// MPLAB header libraries
#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "ChangeClk.h"
#include "UART2.h"
#include "ADC.h"


//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled

// MACROS for Idle, Sleep modes
#define Nop()       {__asm__ volatile ("nop");}                 //
#define ClrWdt()    {__asm__ volatile ("clrwdt");}              //
#define Sleep()     {__asm__ volatile ("pwrsav #0");}           //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle()      {__asm__ volatile ("pwrsav #1");}           //Idle() - put MCU in idle mode - only CPU off
#define dsen()      {__asm__ volatile ("BSET DSCON, #15");}     //

//MAIN
void main(void) {
     
    // Change Clock
    NewClk(500); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
    
    ADCinit();
    while(1)
    {
        //display the 16 bit unsigned int returned by "do_ADC"
        ADC_Display(do_ADC());    
    }
    
    
    return;
}
