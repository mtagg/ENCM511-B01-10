#ifndef DELAY_S_H
#define	DELAY_S_H

#ifdef	__cplusplus
extern "C" {
#endif

    

void T2Init (void);
void delay_ms (int t);
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void);
        
#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_sS_H */

