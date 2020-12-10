// MPLAB header libraries
#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
// User header files
#include "IOs.h"
#include "ChangeClk.h"
#include "debouncer.h"
#include "ADC.h"
#include "UART2.h"
#include "pulse.h"
// CLOCK CONTROL  
#pragma config IESO = OFF    // 2 Speed Startup disabled 
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz 
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor off  
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation 
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn  
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO.  
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled
// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

extern unsigned int STATE;
extern unsigned int freq;
extern unsigned int amp;


//MAIN
int main(void) {

     STATE = 1;  //start in mode 1: voltmeter
     IOinit();    //initialize digital IO
     //TimerInit(); //initialize timers for frequency counting
     DBinit();    //debounce init
     ADCinit();   //initialize analog IO
     //frequency();

//PULSE GENERATION SETTINGS ON REFO/RB15/PIN18 

//  // Setting #1:
    //  NewClk(8);
    //  TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
    //  REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep 
    //  REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
    //  REFOCONbits.RODIV = 0b0011;  //changes frequency of pulse gen 
    //  REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
//  // Setting #2:
    //  NewClk(8);
    //  TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
    //  REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep 
    //  REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
    //  REFOCONbits.RODIV = 0b1111;  //changes frequency of pulse gen 
    //  REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
//  // Setting #3:
    //  NewClk(500);
    //  TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
    //  REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep 
    //  REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
    //  REFOCONbits.RODIV = 0b1001;  //changes frequency of pulse gen 
    //  REFOCONbits.ROEN = 1; // Ref oscillator is enabled 
//  // Setting #4:
     NewClk(500);
     TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO 
     REFOCONbits.ROSSLP = 1; // Ref oscillator runs while in sleep
     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching 
     REFOCONbits.RODIV = 0b0000;  //changes frequency of pulse gen 
     REFOCONbits.ROEN = 1; // Ref oscillator is enabled 


    while(1)
    {
      ADC_Display();
    }
    
    
    return 1;
}
