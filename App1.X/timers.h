#ifndef DELAY_S_H
#define	DELAY_S_H

#include "xc.h"
#include "IOs.h"
#include "MACROS.h"


#ifdef	__cplusplus
extern "C" {#include "UART2.h"
#endif

void T2Init (void);
void countdown(void);//logic that needs to happen every 1 second - aided by ISR
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void);
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void);
        
#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_sS_H */

