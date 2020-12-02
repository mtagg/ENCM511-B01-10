#ifndef IOSTUFF_H
#define IOSTUFF_H

#include "xc.h"
#include "ADC.h"
#include "UART2.h"

void IOinit(void);
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif	/* IOSTUFF_H */

