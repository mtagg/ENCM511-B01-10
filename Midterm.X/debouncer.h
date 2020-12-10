
#ifndef DEBOUNCER_H
#define	DEBOUNCER_H
void DBinit(void);
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);

#endif	/* DEBOUNCER_H */

