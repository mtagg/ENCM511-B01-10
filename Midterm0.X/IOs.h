#ifndef IOSTUFF_H
#define IOSTUFF_H

#include "xc.h"
#include "ADC.h"
#include "UART2.h"
#include "pulse.h"

#define PB1 PORTAbits.RA4
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA2

void IOinit(void);
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif	/* IOSTUFF_H */

