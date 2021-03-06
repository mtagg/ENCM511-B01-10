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
#include "delay_ms.h"
#include "ChangeClk.h"
#include "UART2.h"
#include "MACROS.h"

//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled


// MACROS for Idle, Sleep modes
#define Nop()    {__asm__ volatile ("nop");}                //nop -- useful for managing switching tasks
#define ClrWdt() {__asm__ volatile ("clrwdt");}             //clear watch dog timer??????
#define Sleep()  {__asm__ volatile ("pwrsav #0");}          //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle()   {__asm__ volatile ("pwrsav #1");}          //Idle() - put MCU in idle mode - only CPU off
#define dsen()   {__asm__ volatile ("BSET DSCON, #15");}    //deep sleep enable



int main(void) {
    // Change Clock
    NewClk(32); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
                 //500 mHz better for UART interfacing   
    // Initialize IOs for low-power wake-up
    IOinit();    
    
    // Initialize T2 static settings for delay function
    T2Init();
   

    while(1){
        Idle();
    }
    return 1;
}
