#ifndef IOSTUFF_H
#define IOSTUFF_H

#include "xc.h"
#include "ChangeClk.h"  
#include "timers.h"
#include "UART2.h"
#include "MACROS.h"


void IOinit(void);
void setTime(char t);
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif	/* IOSTUFF_H */

