#ifndef PULSE_H
#define PULSE_H

#include "xc.h"
#include "ChangeClk.h"
#include "ADC.h"
void pulseInit(void);
char* frequency(void);
char* amplitude(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);



#endif //PULSE_H
