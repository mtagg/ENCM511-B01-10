
#ifndef DEBOUNCER_H
#define	DEBOUNCER_H

#include "xc.h"

void T1init(void);
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);

#endif	/* DEBOUNCER_H */

