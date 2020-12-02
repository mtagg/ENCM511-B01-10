/*
 * File:   main.c
 * Author: Rushi V
 *
 * Created on September 26, 2020, 9:17 PM
 */

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


//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config OSCIOFNC = ON  // CLKO output disabled on pin 8, use as IO.

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

extern unsigned int STATE;


//MAIN
void main(void) {
 
    // Change Clock
      // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
     STATE = 3;  // state 3 > PB3 pressed, Idle mode
     NewClk(500);
     //Initialize Registers:
     IOinit();
     ADCinit(); 
     T2Init();  //debounce init
     
    while(1)
    {
       NewClk(32);
       Idle();
    }
    
    
    return;
}
