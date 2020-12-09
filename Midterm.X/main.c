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

//MAIN
int main(void) {
   //STATE = 1;     //voltmeter mode
   IOinit();  
   ADCinit();     //driver 5 init
   T1init();      //debounce timer
   pulseInit();   //driver 6 init

   while(1){
      clear();
      Idle();
   }
   return 1;
}
