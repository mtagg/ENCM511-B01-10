
#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include  <xc.h>
#include "UART2.h"
    
    void ADCinit(void);
    uint16_t do_ADC(void);
    void ADC_Display(uint16_t V);
    void __attribute__((interrupt,no_auto_psv)) _ADC1Interrupt(void);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

