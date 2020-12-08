
#ifndef DEBOUNCER_H
#define	DEBOUNCER_H
void T2Init(void);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

#endif	/* DEBOUNCER_H */

