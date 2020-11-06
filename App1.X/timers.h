#ifndef DELAY_S_H
#define	DELAY_S_H

#include "xc.h"
#include "itoa.h"
#include "IOs.h"
#include "MACROS.h"
#include "UART2.h"

#ifdef	__cplusplus
extern "C" {
#endif

void T2Init (void);
void countdown(void);//logic that needs to happen every 1 second - aided by ISR
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void);
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void);
        
#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_sS_H */

