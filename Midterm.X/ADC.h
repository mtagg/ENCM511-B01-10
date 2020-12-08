
#ifndef ADC_H
#define	ADC_H

#include <xc.h>
#include "UART2.h"
#include "ChangeClk.h"
#include "pulse.h"
    
    void ADCinit(void);
    unsigned int do_ADC(void);
    void ADC_Display(void);
    void ADC2mV(unsigned int V);
    void ADC2ohm(unsigned int V);
    //void __attribute__((interrupt,no_auto_psv)) _ADC1Interrupt(void);


#endif	/* ADC_H */

