#ifndef PULSE_H
#define PULSE_H

#include "xc.h"
#include "ADC.h"
// #include "IOs.h"
// #include <stdio.h>
// #include <math.h>


void TimerInit(void);
void frequency(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);



#endif