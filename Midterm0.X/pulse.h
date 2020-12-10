#ifndef PULSE_H
#define PULSE_H

#include "xc.h"
#include "ChangeClk.h"
#include "ADC.h"
#include <stdio.h>
#include <math.h>



void pulseInit(void);
void frequency(void);
void amplitude(void);
// void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);
//void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);



#endif //PULSE_H
